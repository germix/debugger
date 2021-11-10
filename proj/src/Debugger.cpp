#include "Debugger.h"
#include <windows.h>
#include <QDebug>
#include <QMutex>
#include <QDir>
#include <QFileInfo>

#include "utils/GetFileNameFromHandle.h"

Debugger::Debugger(QObject* parent)
	: QThread(parent)
{
	hProcess = INVALID_HANDLE_VALUE;
	dwProcessId = 0;

	isProgramActive = false;
	isProgramRunning = false;
}

Debugger::~Debugger()
{
}

bool Debugger::programRun(const QString& fileName)
{
	program = fileName;
	QThread::start();

	return true;
}

void Debugger::programKill()
{
	TerminateProcess(hProcess, 0);
}

void Debugger::run()
{
	isProgramActive = true;
	isProgramRunning = true;
	emit starting();

	DEBUG_EVENT debugEvent = {0};

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	QString currentDirectory = QFileInfo(program).absoluteDir().absolutePath();

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	si.cb = sizeof(si);
	BOOL res = CreateProcessW(
		(LPCWSTR)program.constData(),			// lpApplicationName
		NULL,									// lpCommandLine
		NULL,									// lpProcessAttributes
		NULL,									// lpThreadAttributes
		FALSE,									// bInheritHandles
		DEBUG_ONLY_THIS_PROCESS,				// dwCreationFlags
		NULL,									// lpEnvironment
		(LPCWSTR)currentDirectory.constData(),	// lpCurrentDirectory
		&si,									// lpStartupInfo
		&pi);									// lpProcessInformation
	if(!res)
	{
		emit stopped();
		return;
	}

	hProcess = pi.hProcess;
	dwProcessId = pi.dwProcessId;

	bContinueDebugging = true;

	while(bContinueDebugging)
	{
		if(!WaitForDebugEvent(&debugEvent, INFINITE))
			return;
		DWORD dwContinueStatus = DBG_CONTINUE; // exception continuation

		switch(debugEvent.dwDebugEventCode)
		{
			case EXCEPTION_DEBUG_EVENT:
				dwContinueStatus = OnExceptionDebugEvent(debugEvent);
				break;
			case CREATE_THREAD_DEBUG_EVENT:
				dwContinueStatus = OnCreateThreadDebugEvent(debugEvent);
				break;
			case CREATE_PROCESS_DEBUG_EVENT:
				dwContinueStatus = OnCreateProcessDebugEvent(debugEvent);
				break;
			case EXIT_THREAD_DEBUG_EVENT:
				dwContinueStatus = OnExitThreadDebugEvent(debugEvent);
				break;
			case EXIT_PROCESS_DEBUG_EVENT:
				dwContinueStatus = OnExitProcessDebugEvent(debugEvent);
				break;
			case LOAD_DLL_DEBUG_EVENT:
				dwContinueStatus = OnLoadDllDebugEvent(debugEvent);
				break;
			case UNLOAD_DLL_DEBUG_EVENT:
				dwContinueStatus = OnUnloadDllDebugEvent(debugEvent);
				break;
			case OUTPUT_DEBUG_STRING_EVENT:
				dwContinueStatus = OnOutputDebugStringEvent(debugEvent);
				break;
			case RIP_EVENT:
				dwContinueStatus = OnRipEvent(debugEvent);

				break;
		}

		ContinueDebugEvent(
			debugEvent.dwProcessId,
			debugEvent.dwThreadId,
			dwContinueStatus);
	}

	isProgramActive = false;
	isProgramRunning = false;

	emit stopped();
}

DWORD Debugger::OnExceptionDebugEvent(const DEBUG_EVENT& debugEvent)
{
	qDebug("OnExceptionDebugEvent");

	const EXCEPTION_DEBUG_INFO& Exception = debugEvent.u.Exception;

	switch(Exception.ExceptionRecord.ExceptionCode)
	{
		case EXCEPTION_ACCESS_VIOLATION:
			// First chance: Pass this on to the system.
			// Last chance: Display an appropriate error.

			emit log("Access violation" "\n");
			break;

		case EXCEPTION_BREAKPOINT:
			// First chance: Display the current
			// instruction and register values.

			emit log("Break point" "\n");
			break;

		case EXCEPTION_DATATYPE_MISALIGNMENT:
			// First chance: Pass this on to the system.
			// Last chance: Display an appropriate error.

			emit log("Datatype misalignment" "\n");
			break;

		case EXCEPTION_SINGLE_STEP:
			// First chance: Update the display of the
			// current instruction and register values.

			emit log("Single step" "\n");
			break;

		case DBG_CONTROL_C:
			// First chance: Pass this on to the system.
			// Last chance: Display an appropriate error.

			emit log("Control C" "\n");
			break;

		default:
			// Handle other exceptions.
			break;
	}
	return DBG_CONTINUE;
}

DWORD Debugger::OnCreateThreadDebugEvent(const DEBUG_EVENT& debugEvent)
{
	qDebug("OnCreateThreadDebugEvent");

	QString str;

	str += QString().sprintf("Thread 0x%x (Id: %d) created at: 0x%x",
			debugEvent.u.CreateThread.hThread,
			debugEvent.dwThreadId,
			debugEvent.u.CreateThread.lpStartAddress);

	emit log(str + "\n");

	return DBG_CONTINUE;
}

DWORD Debugger::OnCreateProcessDebugEvent(const DEBUG_EVENT& debugEvent)
{
	qDebug("OnCreateProcessDebugEvent");

	QString str = GetFileNameFromHandle(debugEvent.u.CreateProcessInfo.hFile);

	emit log(str + "\n");

	return DBG_CONTINUE;
}

DWORD Debugger::OnExitThreadDebugEvent(const DEBUG_EVENT& debugEvent)
{
	qDebug("OnExitThreadDebugEvent");

	QString str;

	str += QString().sprintf("The thread %d exited with code: %d",
		debugEvent.dwThreadId,
		debugEvent.u.ExitThread.dwExitCode);

	emit log(str + "\n");

	return DBG_CONTINUE;
}

DWORD Debugger::OnExitProcessDebugEvent(const DEBUG_EVENT& debugEvent)
{
	qDebug("OnExitProcessDebugEvent");

	QString str;

	str += QString().sprintf("Process exited with code:  0x%x",
		debugEvent.u.ExitProcess.dwExitCode);

	emit log(str + "\n");

	bContinueDebugging = false;

	return DBG_CONTINUE;
}

DWORD Debugger::OnLoadDllDebugEvent(const DEBUG_EVENT& debugEvent)
{
	qDebug("OnLoadDllDebugEvent");

	QString str;
	QString dllFileName = GetFileNameFromHandle(debugEvent.u.LoadDll.hFile);

	// Storing the DLL name into map. Map's key is the Base-address
	dllNameMap.insert(debugEvent.u.LoadDll.lpBaseOfDll, dllFileName);

	str += dllFileName;
	str += QString().sprintf(" - Loaded at %p", debugEvent.u.LoadDll.lpBaseOfDll);

	emit log(str + "\n");

	return DBG_CONTINUE;
}

DWORD Debugger::OnUnloadDllDebugEvent(const DEBUG_EVENT& debugEvent)
{
	qDebug("OnUnloadDllDebugEvent");

	QString str;

	str += QString().sprintf("DLL '%ls' unloaded.", dllNameMap[debugEvent.u.UnloadDll.lpBaseOfDll].constData());

	emit log(str + "\n");

	return DBG_CONTINUE;
}

DWORD Debugger::OnOutputDebugStringEvent(const DEBUG_EVENT& debugEvent)
{
	qDebug("OnOutputDebugStringEvent");

	QString str;
	const OUTPUT_DEBUG_STRING_INFO& debugString = debugEvent.u.DebugString;

	WCHAR* message = new WCHAR[debugString.nDebugStringLength];

	ReadProcessMemory(
		hProcess,						// HANDLE to Debuggee
		debugString.lpDebugStringData,	// Target process' valid pointer
		message,						// Copy to this address space
		debugString.nDebugStringLength,
		NULL);

	if(debugString.fUnicode)
		str = QString::fromUtf16((const char16_t*)message);
	else
		str = QString::fromUtf8((const char*)message);

	delete [] message;

	return DBG_CONTINUE;
}

DWORD Debugger::OnRipEvent(const DEBUG_EVENT& debugEvent)
{
	qDebug("OnRipEvent");
	return DBG_CONTINUE;
}

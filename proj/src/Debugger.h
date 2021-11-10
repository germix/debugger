#ifndef DEBUGGER_H
#define DEBUGGER_H
#include <QThread>
#include <QHash>
#include <QVector>
#include <windows.h>

struct ProcessInfo
{
public:
	QString		fileName;
	HANDLE		hProcess;
	DWORD		dwProcessId;
	LPVOID		lpBaseOfImage;
	bool 		bKernelBP;
	UINT64		dwEntryPoint;
};

class RegisterInfo
{
public:
	QString		name;
	QString		value;
};

class Debugger : public QThread
{
	Q_OBJECT

	QString					program;

	HANDLE					hDebuggeeProcess;
	DWORD					dwDebuggeeProcessId;
	QHash<LPVOID, QString>	dllNameMap;

	BOOL					bContinueDebugging;
public:
	BOOL					isProgramActive;
	bool					isProgramRunning;

	QVector<ProcessInfo>	processes;

	HANDLE					continueEvent;

	union
	{
		CONTEXT				context;
		WOW64_CONTEXT		wow64Context;
	};
public:
	explicit Debugger(QObject* parent = nullptr);
	~Debugger();
public:
	bool programRun(const QString& fileName);
	void programKill();
	void programBreak();
	void programContinue();
	
	bool isWow64() const;
private:
	void run();

	ProcessInfo* findProcess(DWORD pid);

	DWORD OnExceptionDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnCreateThreadDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnCreateProcessDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnExitThreadDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnExitProcessDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnLoadDllDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnUnloadDllDebugEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnOutputDebugStringEvent(const DEBUG_EVENT& debugEvent);
	DWORD OnRipEvent(const DEBUG_EVENT& debugEvent);
signals:
	void onLog(const QString& s);
	void onStarting();
	void onStopped();
	void onBreak();
	void onContinue();

public slots:
};

#endif // DEBUGGER_H

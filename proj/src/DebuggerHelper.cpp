#include "DebuggerHelper.h"
#include "utils/IsWindowsXP.h"

#define ASSERT(x) Q_ASSERT(x)
#define WARNING(x) qDebug() << "[WARNING] " << x

PfnIsWow64Process DebuggerHelper::pfnIsWow64Process = nullptr;
PfnWow64GetThreadContext DebuggerHelper::pfnWow64GetThreadContext = nullptr;
PfnWow64SetThreadContext DebuggerHelper::pfnWow64SetThreadContext = nullptr;
PfnIsWow64GuestMachineSupported DebuggerHelper::pfnIsWow64GuestMachineSupported = nullptr;

bool DebuggerHelper::init()
{
	bool fails = false;
	HANDLE hKernel32 = GetModuleHandleW(L"Kernel32.dll");

#ifdef _AMD64_
	pfnIsWow64Process = (PfnIsWow64Process)GetProcAddress(hKernel32, "IsWow64Process");
	pfnWow64GetThreadContext = (PfnWow64GetThreadContext)GetProcAddress(hKernel32, "Wow64GetThreadContext");
	pfnWow64SetThreadContext = (PfnWow64SetThreadContext)GetProcAddress(hKernel32, "Wow64SetThreadContext");
	pfnIsWow64GuestMachineSupported = (PfnIsWow64GuestMachineSupported)GetProcAddress(hKernel32, "IsWow64GuestMachineSupported");

	if(!pfnIsWow64Process)
	{
		fails = true;
		WARNING("Can't load Kernel32.dll::IsWow64Process()");
	}
	if(!pfnWow64GetThreadContext && !IsWindowXP())
	{
		fails = true;
		WARNING("Can't load Kernel32.dll::Wow64GetThreadContext()");
	}
	if(!pfnWow64SetThreadContext && !IsWindowsXP())
	{
		fails = true;
		WARNING("Can't load Kernel32.dll::Wow64SetThreadContext()");
	}
#endif

	return !fails;
}

bool DebuggerHelper::isWow64Process(HANDLE hProcess)
{
	if(pfnIsWow64Process)
	{
		BOOL bIsWOW64 = false;

		pfnIsWow64Process(hProcess, &bIsWOW64);

		return bIsWOW64;
	}
	return false;
}

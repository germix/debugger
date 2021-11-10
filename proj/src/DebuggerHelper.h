#ifndef DEBUGGERHELPER_H
#define DEBUGGERHELPER_H
#include <windows.h>

typedef BOOL (__stdcall * PfnIsWow64Process)(HANDLE hProcess, PBOOL Wow64Process);
typedef BOOL (__stdcall * PfnWow64GetThreadContext)(HANDLE hThread, PWOW64_CONTEXT lpContext);
typedef BOOL (__stdcall * PfnWow64SetThreadContext)(HANDLE hThread, CONST WOW64_CONTEXT *lpContext);
typedef HRESULT (__stdcall * PfnIsWow64GuestMachineSupported)(USHORT WowGuestMachine, BOOL* MachineIsSupported);

class DebuggerHelper
{
public:
	static PfnIsWow64Process					pfnIsWow64Process;
	static PfnWow64GetThreadContext				pfnWow64GetThreadContext;
	static PfnWow64SetThreadContext				pfnWow64SetThreadContext;
	static PfnIsWow64GuestMachineSupported		pfnIsWow64GuestMachineSupported;
public:
	static bool init();

	static bool isWow64Process(HANDLE hProcess);
};

#endif // DEBUGGERHELPER_H

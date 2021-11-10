#include "IsWindowsXP.h"

BOOL IsWindowsXP()
{
	OSVERSIONINFO osvi;

	memset(&osvi, 0, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	GetVersionEx(&osvi);

	return (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1);
}

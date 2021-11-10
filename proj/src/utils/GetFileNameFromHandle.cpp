#include <QString>
#include <QDebug>
#include <windows.h>
#include <psapi.h>
#include <strsafe.h>

#define BUFSIZE 512

QString GetFileNameFromHandle(HANDLE hFile)
{
	BOOL bSuccess = FALSE;
	WCHAR pszFilename[MAX_PATH+1];
	HANDLE hFileMap;

	// Get the file size.
	DWORD dwFileSizeHi = 0;
	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);

	if(dwFileSizeLo == 0 && dwFileSizeHi == 0)
	{
		qDebug() << "Cannot map a file with a length of zero.";
		return QString();
	}

	// Create a file mapping object.
	hFileMap = CreateFileMapping(
				hFile,
				NULL,
				PAGE_READONLY,
				0,
				1,
				NULL);

	if(hFileMap)
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

		if(pMem)
		{
			if(GetMappedFileNameW(GetCurrentProcess(), pMem, pszFilename, MAX_PATH))
			{
				// Translate path with device name to drive letters.
				WCHAR szTemp[BUFSIZE];
				szTemp[0] = '\0';

				if(GetLogicalDriveStrings(BUFSIZE-1, szTemp))
				{
					WCHAR szName[MAX_PATH];
					WCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					WCHAR* p = szTemp;

					do
					{
						// Copy the drive letter to the template string
						*szDrive = *p;

						// Look up each device name
						if(QueryDosDeviceW(szDrive, szName, MAX_PATH))
						{
							size_t uNameLen = wcslen(szName);

							if(uNameLen < MAX_PATH)
							{
								bFound = wcsnicmp(pszFilename, szName, uNameLen) == 0 && *(pszFilename + uNameLen) == L'\\';

								if(bFound)
								{
									// Reconstruct pszFilename using szTempFile
									// Replace device path with DOS path
									WCHAR szTempFile[MAX_PATH];
									StringCchPrintfW(szTempFile,
											MAX_PATH,
											L"%s%s",
											szDrive,
											pszFilename+uNameLen);
									StringCchCopyNW(pszFilename, MAX_PATH+1, szTempFile, wcslen(szTempFile));
								}
							}
						}
						// Go to the next NULL character.
						while(*p++);

					}
					while(!bFound && *p); // end of string
				}
			}
			bSuccess = TRUE;
			UnmapViewOfFile(pMem);
		}

		CloseHandle(hFileMap);
	}
	
#if 0
	qDebug("File name is %ls", pszFilename);
#endif
	
	return QString::fromUtf16((const char16_t*)pszFilename);
}

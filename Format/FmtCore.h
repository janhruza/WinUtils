#pragma once
#include "resource.h"
#include <stdio.h>
#include <Windows.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// definitions
#define FMT_FILE_PATH   L"C:\\WINDOWS\\System32\\format.exe"
#define FMT_BUFFER_SIZE 4096
#define FMT_MSG_WARNIG  L"Formatting this drive will erase all data on it. This action cannot be undone. Are you sure you want to proceed? Click Yes to continue, or No / Cancel to cancel the operation."

// filesystems definitions
#define FMT_FS_REFS     L"ReFS"
#define FMT_FS_NTFS     L"NTFS"
#define FMT_FS_FAT      L"FAT"
#define FMT_FS_FAT32    L"FAT32"
#define FMT_FS_EXFAT    L"exFAT"
#define FMT_FS_UDF      L"UDF"

// member definitions
HWND hDialog;
HBRUSH hbrBackground;
COLORREF hBg;
HWND hCbxDrives;
HWND hCbxFileSystems;
HWND hCbxAllocSizes;

DWORD dwAllocSizes[] = { 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 };

typedef struct tagFormatOptions
{
	/// <summary>
	/// Drive letter.
	/// </summary>
	LPWSTR wVolume;

	/// <summary>
	/// Desired filesystem.
	/// </summary>
	LPWSTR wFilesystem;

	/// <summary>
	/// New drive label.
	/// </summary>
	LPWSTR wLabel;

	/// <summary>
	/// True for /Q, false for normal format.
	/// </summary>
	BOOL bQuick;

	/// <summary>
	/// NTFS only. Enables compression by default.
	/// </summary>
	BOOL bNTFSCompression;

	/// <summary>
	/// Forces drive dismount before formatting.
	/// </summary>
	BOOL bDismount;

	/// <summary>
	/// UDF only. Duplicates the UDF metadata.
	/// </summary>
	BOOL bUDFDuplicate;

    /// <summary>
    /// NTFS only. True forces override of the default size of the record. Modifies the /L switch.
    /// </summary>
    BOOL bState;

    /// <summary>
    /// Allocation unit size in bytes.
    /// </summary>
    DWORD dwAllocSize;

} FormatOptions;

FormatOptions sOptions;

inline BOOL FmtBeginFormat(FormatOptions* sOptions)
{
    if (MessageBox(hDialog, FMT_MSG_WARNIG, L"Warning", MB_YESNOCANCEL | MB_ICONWARNING) != IDYES)
    {
        // IDCANCEL or IDNO happened
        // cancel formatting
        return FALSE;
    }

	return TRUE;
}

inline DWORD FmtCreateProcess(LPWSTR lpszApp, LPWSTR lpszCmdLine, BOOL bWait)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    if (CreateProcess(
        lpszApp, // Fixed path
        lpszCmdLine,
        NULL, NULL,
        TRUE, // Need to inherit handles (should be TRUE because you're using std handles)
        NORMAL_PRIORITY_CLASS,
        NULL, NULL,
        &si, &pi))
    {
        if (bWait == TRUE)
        {
            WaitForSingleObject(pi.hProcess, INFINITE);

            DWORD dwExit;
            if (GetExitCodeProcess(pi.hProcess, &dwExit))
            {
                // process exited successfuly
                // return success
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                return dwExit;
            }
            else
            {
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                return 0xDEAD; // Could not get exit code
            }
        }

        else
        {
            // bWait not specified
            // successful exit (no wait)
            return TRUE;
        }
    }
    else
    {
        return 0xDEAD; // CreateProcess failed
    }
}

inline void FmtRefreshDrives(HWND hCbx)
{
    if (!hCbx) return;

    if (GetLogicalDrives() == 0)
    {
        // no drives
        return;
    }

    WCHAR pDrives[FMT_BUFFER_SIZE];
    if (GetLogicalDriveStrings(FMT_BUFFER_SIZE, pDrives) > FMT_BUFFER_SIZE)
    {
        // buffer too small
        return;
    }

    SendMessage(hCbx, CB_RESETCONTENT, 0, 0);

    WCHAR* drive = pDrives;
    while (*drive)
    {
        // handle drive
        WCHAR volumeName[2048];
        DWORD dwSerial;
        DWORD dwMaxComponent;
        DWORD dwFileSystem;
        WCHAR wFSName[2048];

        GetVolumeInformation(drive, volumeName, sizeof(volumeName), &dwSerial, &dwMaxComponent, &dwFileSystem, wFSName, sizeof(wFSName));

        if (lstrlen(volumeName) == 0)
        {
            swprintf_s(volumeName, MAX_PATH, L"Untitled\n");
        }

        WCHAR wFullLabel[FMT_BUFFER_SIZE];
        swprintf_s(wFullLabel, MAX_PATH, L"%s (%s) - SN=%u", volumeName, drive, dwSerial);

        int idx = SendMessage(hCbx, CB_ADDSTRING, 0, (LPARAM)wFullLabel);
        if (idx == CB_ERR) continue;

        SendMessage(hCbx, CB_SETITEMDATA, idx, drive);

        // move within buffer
        drive += lstrlen(drive) + 1;
    }

    int count = SendMessage(hCbx, CB_GETCOUNT, 0, 0);
    if (count != CB_ERR && count > 0)
    {
        SendMessage(hCbx, CB_SETCURSEL, 0, 0);
    }

    return;
}

inline void FmtRefreshFilesystems(HWND hCbx)
{
    if (!hCbx) return;
    SendMessage(hCbx, CB_RESETCONTENT, 0, 0);
    SendMessage(hCbx, CB_ADDSTRING, 0, FMT_FS_EXFAT);
    SendMessage(hCbx, CB_ADDSTRING, 0, FMT_FS_FAT);
    SendMessage(hCbx, CB_ADDSTRING, 0, FMT_FS_FAT32);
    SendMessage(hCbx, CB_ADDSTRING, 0, FMT_FS_NTFS);
    SendMessage(hCbx, CB_ADDSTRING, 0, FMT_FS_REFS);
    SendMessage(hCbx, CB_ADDSTRING, 0, FMT_FS_UDF);
    SendMessage(hCbx, CB_SETCURSEL, 0, 0);
    return;
}

inline void FmtRefreshAllocSizes(HWND hCbx)
{
    if (!hCbx) return;
    SendMessage(hCbx, CB_RESETCONTENT, 0, 0);

    for (int x = 0; x < ARRAYSIZE(dwAllocSizes); x++)
    {
        DWORD dwValue = dwAllocSizes[x];
        WCHAR buf[10];
        swprintf_s(buf, 10, L"%d\0", dwValue);
        SendMessage(hCbx, CB_ADDSTRING, 0, buf);
    }

    SendMessage(hCbx, CB_SETCURSEL, 0, 0);
    return;
}

inline BOOL FmtValidate(FormatOptions* options)
{
    if (!options) return FALSE;
    if (!options->wVolume) return FALSE;
    if (options->dwAllocSize < 512) return FALSE;
    if (!options->wFilesystem) return FALSE;
    return TRUE;
}
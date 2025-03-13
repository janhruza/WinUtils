/*
* MemUpdate.h
* Main header file with all the important functions and definitions.
*/

#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#pragma region Data types

typedef const char* STRING;

typedef enum MURESULT
{
	MU_OK,
	MU_ERROR,
	MU_CHECK_FAILED,
	MU_INVALID_PROCESS,
	MU_INVALID_ADDRESS,
	MU_INVALID_HANDLE,
	MU_INVALID_SIZE
};

#pragma endregion

#pragma region Memory manipulation

inline MURESULT ReadMemory(HANDLE hProcess, LPVOID address, int *buffer, SIZE_T size)
{
	if (hProcess == NULL) return MU_INVALID_PROCESS;
	if (size == NULL) return MU_INVALID_HANDLE;
	if (buffer == nullptr) return MU_INVALID_HANDLE;
	return ReadProcessMemory(hProcess, address, &buffer, size, NULL) == TRUE ? MU_OK : MU_ERROR;
}

inline int ReadMemory32(HANDLE hProcess, LPVOID address)
{
	if (hProcess == nullptr) return INT_MIN;
	if (address == NULL) return INT_MIN;

	int buf;
	int size = sizeof(int);

	return ReadMemory(hProcess, address, &buf, size) == MU_OK ? buf : INT_MAX;
}

inline MURESULT WriteMemory(HANDLE hProcess, LPVOID address, int* buffer, SIZE_T size)
{
	if (hProcess == nullptr) return MU_INVALID_PROCESS;
	if (address == nullptr) return MU_INVALID_ADDRESS;
	if (buffer == nullptr) return MU_INVALID_HANDLE;
	if (size == NULL) return MU_INVALID_SIZE;
	return WriteProcessMemory(hProcess, address, buffer, size, NULL) == TRUE ? MU_OK : MU_ERROR;
}

inline MURESULT WriteMemory32(HANDLE hProcess, LPVOID address, int value)
{
	return WriteMemory(hProcess, address, &value, sizeof(int));
}

#pragma endregion

#pragma region Process maniulation
const char* ConvertWCharToChar(const WCHAR* wStr) {
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wStr, -1, NULL, 0, NULL, NULL);
	char* cStr = new char[size_needed];
	WideCharToMultiByte(CP_UTF8, 0, wStr, -1, cStr, size_needed, NULL, NULL);
	return cStr;
}

inline DWORD GetProcessID(const char* processName) {
	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		perror("Error: Unable to create snapshot.");
		return 0;
	}

	if (Process32First(snapshot, &processEntry) == TRUE) {
		do {
			const char* exeFile = ConvertWCharToChar(processEntry.szExeFile);

			if (strcmp(exeFile, processName) == 0) {
				DWORD processID = processEntry.th32ProcessID;
				CloseHandle(snapshot);
				return processID;
			}
		} while (Process32Next(snapshot, &processEntry) == TRUE);
	}

	CloseHandle(snapshot);
	return 0;
}

#pragma endregion


#pragma region Session details and logic

typedef struct MU_SESSION
{
	DWORD dwProcessID;
	HANDLE hProcess;
	PCHAR sProcessName;
};

inline void FreeSession(MU_SESSION* session)
{
	if (session == nullptr) return;
	ZeroMemory(session, sizeof(MU_SESSION));
	return;
}

inline MURESULT BeginSession(LPSTR processName, MU_SESSION* session)
{
	if (session == nullptr)
	{
		return MU_INVALID_HANDLE;
	}

	FreeSession(session);
	if (strlen(processName) == NULL) return MU_INVALID_SIZE;

	DWORD id = GetProcessID(processName);
	if (id == 0) return MU_INVALID_PROCESS;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
	session->dwProcessID = id;
	session->hProcess = hProcess;
	session->sProcessName = processName;
	return MU_OK;
}

inline MURESULT EndSession(MU_SESSION* session)
{
	if (session == nullptr) return MU_INVALID_HANDLE;
	FreeSession(session);
	return MU_OK;
}

#pragma endregion

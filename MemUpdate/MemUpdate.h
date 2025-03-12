/*
* MemUpdate.h
* Main header file with all the important functions and definitions.
*/

#pragma once
#include <Windows.h>

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
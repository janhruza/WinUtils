/*
* Source.c
* Contains the main method.
*/

#include <stdio.h>
#include <Windows.h>

static BOOL InjectDLL(DWORD processID, LPWSTR dllPath) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (!hProcess) {
		fwprintf(stderr, L"Failed to open target process.\n");
		return FALSE;
	}

	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, wcslen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (!pDllPath) {
		fwprintf(stderr, L"Failed to allocate memory in target process.\n");
		CloseHandle(hProcess);
		return FALSE;
	}

	if (!WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath, wcslen(dllPath) + 1, 0)) {
		fwprintf(stderr, L"Failed to write DLL path to target process memory.\n");
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	HMODULE hKernel32 = GetModuleHandle(L"kernel32.dll");
	if (hKernel32 == NULL) {
		fwprintf(stderr, L"Failed to get handle of kernel32.dll.\n");
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	HANDLE hThread = CreateRemoteThread(hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW"),
		pDllPath, 0, 0);

	if (!hThread) {
		fwprintf(stderr, L"Failed to create remote thread in target process.\n");
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	WaitForSingleObject(hThread, INFINITE);

	VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}


static HRESULT InjectProcess(LPWSTR application, LPWSTR library)
{
	// start new process, get its PID and inject DLL
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	if (CreateProcess(application, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi) == NULL)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		fwprintf(stderr, L"Unable to create a new process.\n");
		return E_FAIL;
	}

	// try attach dll
	BOOL value = InjectDLL(pi.dwProcessId, library);

	// clear memory
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return value == TRUE ? S_OK : E_FAIL;
}

int wmain(int argc, const LPWSTR argv[])
{
	if (argc != 3)
	{
		fwprintf(stderr, L"Invalid number of arguments passed.\nUsage: LibInject.exe [exePath] [dllPath]\n");
		return EXIT_FAILURE;
	}

	// Try injection
	return SUCCEEDED(InjectProcess(argv[1], argv[2])) ? EXIT_SUCCESS : EXIT_FAILURE;
}
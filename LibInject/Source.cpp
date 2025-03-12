/*
* Source.cpp
* Contains the main method.
*/

#include <iostream>
#include <Windows.h>

bool InjectDLL(DWORD processID, const char* dllPath) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (!hProcess) {
		std::cerr << "Failed to open target process." << std::endl;
		return false;
	}

	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (!pDllPath) {
		std::cerr << "Failed to allocate memory in target process." << std::endl;
		CloseHandle(hProcess);
		return false;
	}

	if (!WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath, strlen(dllPath) + 1, 0)) {
		std::cerr << "Failed to write DLL path to target process memory." << std::endl;
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	HANDLE hThread = CreateRemoteThread(hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA"),
		pDllPath, 0, 0);
	if (!hThread) {
		std::cerr << "Failed to create remote thread in target process." << std::endl;
		VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	WaitForSingleObject(hThread, INFINITE);

	VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return true;
}


HRESULT InjectProcess(const char* application, const char* library)
{
	// start new process, get its PID and inject DLL
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	if (CreateProcessA(application, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi) == NULL)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		std::cerr << "Unable to create a new process." << std::endl;
		return E_FAIL;
	}

	// try attach dll
	bool value = InjectDLL(pi.dwProcessId, library);

	// clear memory
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return value == true ? S_OK : E_FAIL;
}

int main(int argc, const char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Invalid number of arguments passed.\nUsage: LibInject.exe [exePath] [dllPath]" << std::endl;
		return EXIT_FAILURE;
	}

	// Try injection
	return SUCCEEDED(InjectProcess(argv[1], argv[2])) ? EXIT_SUCCESS : EXIT_FAILURE;
}
/*
* Source.cpp
* This file contains the main function.
*/
#include "MemUpdate.h"
#include <stdbool.h>
#include <stdio.h>

/// <summary>
/// Representing the console window title
/// </summary>
const LPCWSTR windowTitle = TEXT("Memory Update");

inline void ClearBuffer(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
	return;
}

int wmain(int argc, const char* argv[])
{
	// program initialization
	SetConsoleTitle(windowTitle);

	MU_SESSION session;

	if (argc == 1)
	{
		// Beginning of the main function when no arguments are passed.
	start_no_args:

		// get data with UI
		// get process name
		wchar_t processName[MAX_PATH] = { NULL };

		wprintf_s(L"Enter process name\n# ");
		(void)wscanf_s(L"%s", &processName, MAX_PATH);

		if (BeginSession(processName, &session) != MU_OK)
		{
			wprintf_s(L"Unable to initialize the session. Make sure the process \'%s\' exists.\nTry again? [Y/n]: ", processName);
			ClearBuffer();

			int c = getchar();
			if (c != (int)'y' && c != (int)'Y')
			{
				return EXIT_FAILURE;
			}

			else
			{
				// return to start
				wprintf_s(L"\n");
				goto start_no_args;
			}
		}
	}

	else
	{
		// load data from parameters
		// load process name

		wchar_t* procName = argv[1];
		if (wcscmp(procName, L"") == 0)
		{
			wprintf_s(L"\nInvalid input.\n");
			return EXIT_FAILURE;
		}

		if (BeginSession(procName, &session) != MU_OK)
		{
			wprintf_s(L"\nUnable to initialize the session. Make sure the process \'%s\' exists.\n", procName);
			return EXIT_FAILURE;
		}
	}

	// process loaded
	bool exitProcess = false;
	while (exitProcess == false)
	{
		// main code goes here
		// how it works:
		//	- get user input (list values, write memory, read memory, ...)
		//	- process input
		//	- print result

		// get user input
		wchar_t input[MAX_PATH] = { NULL };
		wprintf_s(L"[%s (%d)]# ", session.sProcessName, session.dwProcessID);
		(void)wscanf_s(L"%s", input, MAX_PATH);

		ClearBuffer();

		if (wcscmp((STRING)input, L"exit") == 0)
		{
			return EXIT_SUCCESS;
		}
	}

	return EXIT_SUCCESS;
}
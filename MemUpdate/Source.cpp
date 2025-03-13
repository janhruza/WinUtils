/*
* Source.cpp
* This file contains the main function.
*/
#include <iostream>
#include "MemUpdate.h"

/// <summary>
/// Representing the console window title
/// </summary>
const LPCSTR windowTitle = "MemUpdate";

int main(int argc, const char* argv[])
{
	// program initialization
	SetConsoleTitleA(windowTitle);

	MU_SESSION session;

	if (argc == 1)
	{
		// Beginning of the main function when no arguments are passed.
	start_no_args:

		// get data with UI
		// get process name
		char processName[MAX_PATH] = { NULL };

		printf_s("Enter process name\n# ");
		(void)scanf_s("%s", &processName, MAX_PATH);
		
		if (BeginSession(processName, &session) != MU_OK)
		{
			printf_s("Unable to initialize the session. Make sure the process \'%s\' exists.\nTry again? [Y/n]: ", processName);
			(void)getchar();

			int c = getchar();
			if (c != (int)'y' && c != (char)'Y')
			{
				return EXIT_FAILURE;
			}

			else
			{
				// return to start
				printf_s("\n");
				goto start_no_args;
			}
		}
	}

	else
	{
		// load data from parameters
		// load process name

		LPSTR procName = LPSTR(argv[1]);
		if (strcmp(procName, "") == 0)
		{
			printf_s("\nInvalid input.\n");
			return EXIT_FAILURE;
		}

		if (BeginSession(procName, &session) != MU_OK)
		{
			printf_s("\nUnable to initialize the session. Make sure the process \'%s\' exists.\n", procName);
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
		LPSTR input[MAX_PATH] = { NULL };
		printf_s("[%s (%d)]# ", session.sProcessName, session.dwProcessID);
		(void)scanf_s("%s", (STRING)input, MAX_PATH);
		(void)getchar();

		if (strcmp((STRING)input, "exit") == 0)
		{
			return EXIT_SUCCESS;
		}
	}

	return EXIT_SUCCESS;
}
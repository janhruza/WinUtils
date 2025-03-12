/*
* Source.cpp
* This file contains the main function.
*/
#include <iostream>
#include "MemUpdate.h"

int main(int argc, const char* argv[])
{
	MU_SESSION session;

	if (argc == 1)
	{
		// get data with UI
		// get process name
		char processName[MAX_PATH];
		printf_s("Enter process name\n# ");
		scanf_s("%260s", &processName, MAX_PATH);

		if (strcmp(processName, "") == 0)
		{
			printf_s("\nInvalid input.\n");
			return EXIT_FAILURE;
		}
		
		if (BeginSession(processName, &session) != MU_OK)
		{
			printf_s("\nUnable to initialize the session. Make sure the process \'%s\' exists.\n", processName);
			return EXIT_FAILURE;
		}
	}

	else
	{
		// load data from parameters
		// load process name

		PCHAR procName = PCHAR(argv[1]);
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

	printf_s("OK: Process \'%s\' with PID %d was found.\n", session.sProcessName, session.dwProcessID);
	return EXIT_SUCCESS;
}
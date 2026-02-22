#pragma once
#define WIN32_LEAN_AND_MEAN

#include "inc/WindowsUpdate.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

/// <summary>
/// Flushes the standard input stream.
/// </summary>
inline static void flushstdin()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
	return;
}

/// <summary>
/// Main entry point.
/// </summary>
/// <param name="argc">Number of command line arguments.</param>
/// <param name="argv">Command line arguments.</param>
/// <returns>Application's exit code.</returns>
INT wmain(int argc, const wchar_t* argv[])
{
	if (argc > 1)
	{
		fprintf_s(stderr, "Arguments are disabled.\n");
		return EXIT_FAILURE;
	}

	SetConsoleTitle(TEXT("WinUpdate"));

	printf_s("WinUpdate - Simple WinGet wrapper\n(c) 2026, Jan Hruza\n\n");

	for (;;)
	{
		printf_s("1. Check for updates\n");
		printf_s("2. Install pending updates\n");
		printf_s("3. Install new package(s)\n");
		printf_s("4. Remove package(s)\n");
		printf_s("5. Open config file\n");
		printf_s("6. Custom command\n");
		printf_s("0. Exit\n\n");

		int option = 0xFF; // must be initialized to prevent errors and invalid options
		printf_s("Enter option [0-6]: ");
		scanf_s("%d", &option);
		flushstdin();

		switch (option)
		{
		case 1:
			// check for updates
			printf_s("\n");
			WuCheckForUpdates();
			printf_s("\n");
			break;

		case 2:
			// install all pending updates
			printf_s("\n");
			WuInstallUpdates();
			printf_s("\n");
			break;

		case 3:
			// install new package(s)
			{
				printf_s("\n");
				CHAR packageIds[1024] = { 0 };
				printf_s("List all package(s) to install (separated by spaces)\n# ");

				if (fgets(packageIds, sizeof(packageIds), stdin) != NULL)
				{
					WuInstallPackages(packageIds);
				}

				printf_s("\n");
			}
			break;

		case 4:
			// remove package(s)
			{
				printf_s("\n");
				CHAR packageIds[1024] = { 0 };
				printf_s("List all package(s) to remove (separated by spaces)\n# ");

				if (fgets(packageIds, sizeof(packageIds), stdin) != NULL)
				{
					WuRemovePackages(packageIds);
				}

				printf_s("\n");
			}
			break;

		case 5:
			// open the config file
			{
				printf_s("\nWinUpdate: Opening the config file... ");
				if (WuOpenConfig() == TRUE)
				{
					printf_s("\nWinUpdate: Config file opened!\n\n");
				}

				else
				{
					printf_s("\nWinUpdate: Unable to start process.\n\n");
				}
			}
			break;

		case 6:
			// run a custom command
			{
				printf_s("\nEnter custom WinGet command\n# winget ");
				CHAR buf[256] = { 0 };

				if (fgets(buf, 255, stdin) != NULL)
				{
					printf_s("\n");
					if (WuCustomCommand(buf) == TRUE)
					{
						printf_s("Command completed successfully.\n");
					}

					else
					{
						printf_s("Command failed.\n");
					}

					printf_s("\n");
				}
				
			}
			break;

		case 0:
			// break from the loop and exit the program
			goto processExit;

		default:
			// option not found
			printf_s("\nOption %d not found.\n\n", option);
			break;
		}
	}

	processExit:
	return EXIT_SUCCESS;
}
#pragma once
#include <stdio.h>
#include <Windows.h>
#include <string.h>

/// <summary>
/// Starts a new process using the Windows shell.
/// </summary>
/// <param name="program">Program to be executed.</param>
/// <param name="args">Command line arguments.</param>
/// <returns>TRUE if the process exit code is 0, otherwise FALSE.</returns>
static inline BOOL WuStartProcess(LPCSTR program, LPCSTR args)
{
	LPSTR command[80];
	sprintf_s(command, 80, "%s %s\0", program, args);
	return system(command) == 0 ? TRUE : FALSE;
}

/// <summary>
/// Checks for available updates.
/// </summary>
/// <returns>Operation result.</returns>
inline static BOOL WuCheckForUpdates(void)
{
	return WuStartProcess("winget", "update");
}

/// <summary>
/// Runs the upgrade process for all upgradable packages.
/// </summary>
/// <returns>Operation result.</returns>
inline static BOOL WuInstallUpdates(void)
{
	return WuStartProcess("winget", "update --all");
}

/// <summary>
/// Installs all given packages.
/// </summary>
/// <param name="packageIds">List of all packages to install. These values must be space-separated.</param>
/// <returns>Operation result.</returns>
inline static BOOL WuInstallPackages(LPCSTR packageIds)
{
	CHAR args[1024] = { 0 };
	sprintf_s(args, sizeof(args), "install %s", packageIds);
	return WuStartProcess("winget", args);
}

/// <summary>
/// Removes all given packages.
/// </summary>
/// <param name="packageIds">List of all packages to remove. These values must be space-separated.</param>
/// <returns>Operation result.</returns>
inline static BOOL WuRemovePackages(LPCSTR packageIds)
{
	CHAR args[1024] = { 0 };
	sprintf_s(args, sizeof(args), "remove %s", packageIds);
	return WuStartProcess("winget", args);
}
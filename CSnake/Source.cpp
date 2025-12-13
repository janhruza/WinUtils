/*
* Source.cpp
* Main entry point.
*/

#pragma once
#include <Windows.h>
#include "inc/SnakeWindow.h"
#include "res/resource.h"

/* App's main entry point. */
INT WINAPI wWinMain(HINSTANCE hInstance,
			 HINSTANCE hPrevInstance,
			 PWSTR lpCommandLine,
			 int nCmdShow)
{

	// create a new window
	HMENU menu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_APP_MENU));
	SnakeWindow wnd = SnakeWindow(hInstance, nullptr);
	wnd.Show();
	return 0;
}
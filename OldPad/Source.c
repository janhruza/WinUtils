#pragma once
#include "OldPad.h"

// signatures
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Representing the main application method.
/// </summary>
/// <param name="hInstance">Handle to the instance.</param>
/// <param name="hPrevInstance">Handle to previous instance.</param>
/// <param name="lpszCmdLine">Command line arguments.</param>
/// <param name="nCmdShow">Window show mode.</param>
/// <returns>Application exit code.</returns>
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdLine, int nCmdShow)
{
	// create window class
	WNDCLASSEX wex;
	wex.cbSize = sizeof(WNDCLASSEX);
	wex.cbClsExtra = 0;
	wex.cbWndExtra = 0;
	wex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wex.hIcon = LoadIcon(hInstance, IDC_ICON);
	wex.hIconSm = LoadIcon(hInstance, IDC_ICON);
	wex.hInstance = hInstance;
	wex.lpfnWndProc = WndProc;
	wex.lpszClassName = OP_CLASS_NAME;
	wex.lpszMenuName = MAKEINTRESOURCE(IDR_APP_MENU);
	wex.style = CS_HREDRAW | CS_VREDRAW;

	// register class
	if (!RegisterClassEx(&wex))
	{
		OpShowError(L"Can't register class.");
		UnregisterClass(OP_CLASS_NAME, hInstance);
		return EXIT_FAILURE;
	}

	// create main window
	HWND hWnd = CreateWindowEx(NULL,
		OP_CLASS_NAME,
		OP_WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		OP_DEFAULT_WIDTH,
		OP_DEFAULT_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		OpShowError(L"Invalid window handle.");
		UnregisterClass(OP_CLASS_NAME, hInstance);
		return EXIT_FAILURE;
	}

	// set global variable
	hWindow = hWnd;

	RECT rect;
	GetClientRect(hWnd, &rect);

	// create text edit
	hEdit = CreateWindow(WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL, 0, 0, rect.right, rect.bottom, hWnd, NULL, hInstance, NULL);
	hDefaultFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_QUALITY, L"Consolas");
	SendMessage(hEdit, WM_SETFONT, (WPARAM)hDefaultFont, TRUE);

	// show window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return EXIT_SUCCESS;
}

/// <summary>
/// Window procedure callback method.
/// </summary>
/// <param name="hWnd">Handle to a window.</param>
/// <param name="msg">Received message.</param>
/// <param name="wParam">WPARAM value.</param>
/// <param name="lParam">LPARAM value.</param>
/// <returns>Message status.</returns>
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
	//case WM_SIZING:
		RECT rect;
		GetClientRect(hWindow, &rect);

		SetWindowPos(hEdit, NULL, rect.top, rect.left, rect.right, rect.bottom, SWP_DRAWFRAME);
		return DefWindowProc(hWnd, msg, wParam, lParam);

	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_CLOSE:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);

	case WM_COMMAND:

		switch (wParam)
		{
			// Close window
			case ID_FILE_CLOSE:
				PostQuitMessage(0);
				return 0;

			// open new window
			case ID_FILE_NEWWINDOW:
				AcNewWindow();
				return 0;

			// open file
			case ID_FILE_OPEN:
				return 0;

			// save file
			case ID_FILE_SAVE:
				return 0;

			// save file as
			case ID_FILE_SAVEAS:
				return 0;

			// change font family
			case ID_EDIT_FONT:
				AcChangeFont(hEdit);
				return 0;

			// show about dialog
			case ID_HELP_ABOUT:
				DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUT), hWnd, DlgProc);
				return 0;
		}

		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
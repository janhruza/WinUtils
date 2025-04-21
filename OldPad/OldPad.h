#pragma once
#include "resource.h"
#include "AppCommands.h"
#include <stdio.h>
#include <CommCtrl.h>
#include <Windows.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// definitions
#define OP_CLASS_NAME		L"Wnd_OldPad"
#define OP_WINDOW_TITLE		L"Oldpad"
#define OP_DEFAULT_WIDTH	800
#define OP_DEFAULT_HEIGHT	600
#define OP_UNSAVED_FORMAT	L"%s* - %s" //	filename, window title
#define OP_SAVED_FORMAT		L"%s - %s"	//	filename, window title

// License resources
#define OP_LICENSE_PART_0	L"Copyright (c) 2025 Jan Hruza"
#define OP_LICENSE_PART_1	L"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:"
#define OP_LICENSE_PART_2	L"The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software."
#define OP_LICENSE_PART_3	L"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."

/// <summary>
/// Handle to the main window.
/// </summary>
static HWND hWindow;

/// <summary>
/// Handle to the edit control (text box).
/// </summary>
static HWND hEdit;

inline void OpShowMessage(LPWSTR message)
{
	MessageBox(hWindow, message, OP_WINDOW_TITLE, MB_OK | MB_ICONINFORMATION);
	return;
}

inline void OpShowError(LPWSTR message)
{
	MessageBox(hWindow, message, OP_WINDOW_TITLE, MB_OK | MB_ICONERROR);
	return;
}

inline LPWSTR OpGetLicenseText()
{
	return OP_LICENSE_PART_0 "\n\n" OP_LICENSE_PART_1 "\n\n" OP_LICENSE_PART_2 "\n\n" OP_LICENSE_PART_3;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		HWND hStatic = GetDlgItem(hDlg, IDC_LICENSE_TEXT);
		SetWindowText(hStatic, OpGetLicenseText());
		return TRUE;

	case WM_CLOSE:
		EndDialog(hDlg, IDOK);
		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_ABOUT_OK:
			EndDialog(hDlg, IDOK);
			return TRUE;
		}
		return FALSE;

	default:
		return FALSE;
	}
}
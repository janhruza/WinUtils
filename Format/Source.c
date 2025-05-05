#pragma once
#include "FmtCore.h"

#define ID_ABOUT_FORMAT 0xAB00
#define ID_REFRESH_UI   0xAB01

INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
    hBg = RGB(0xFF, 0xFF, 0xFF);
    hbrBackground = CreateSolidBrush(hBg);

    AllocConsole();
    SetConsoleTitle(FMT_CONSOLE_TITLE);
    //FmtCreateProcess(L"C:\\WINDOWS\\System32\\cmd.exe", NULL, FALSE);
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_FORMVIEW), NULL, DialogProc);
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:

        // get dialog
        hDialog = hDlg;

        HMENU hSystemMenu = GetSystemMenu(hDlg, FALSE);
        AppendMenu(hSystemMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hSystemMenu, MF_STRING, ID_ABOUT_FORMAT, L"About Format\tF1");

		hPopupMenu = CreatePopupMenu();
		AppendMenu(hPopupMenu, MF_STRING, ID_ABOUT_FORMAT, L"About Format\tF1");
		AppendMenu(hPopupMenu, MF_STRING, ID_REFRESH_UI, L"Refresh UI\tF5");

        // get controls
        hCbxDrives = GetDlgItem(hDlg, IDC_CBX_DRIVE);
        hCbxFileSystems = GetDlgItem(hDlg, IDC_CBX_FILESYSTEM);
        hCbxAllocSizes = GetDlgItem(hDlg, IDC_CBX_ALLOC_SIZE);
        FmtRefreshAll();

        break;

    case WM_CTLCOLORDLG:
        return hbrBackground;

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT:
    {
        HDC hdc = GetDC(hDlg);
        SetBkColor(hdc, hBg);
        return hdc;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_BTN_CLOSE:
            EndDialog(hDialog, IDCLOSE);
            return TRUE;

        case ID_ABOUT_FORMAT:
            // about format dialog
            FmtAboutDialog(hDlg);
            break;

        case ID_REFRESH_UI:
            // refresh UI
            FmtRefreshAll();
            break;

        case IDC_CBX_DRIVE:
        {
            switch (HIWORD(wParam))
            {
            case CBN_SELCHANGE:
            {
                // get selected item to select target drive
                int idx = SendMessage(hCbxDrives, CB_GETCURSEL, 0, 0);
                WCHAR* data = SendMessage(hCbxDrives, CB_GETITEMDATA, idx, 0);

                // set drive letter
                sOptions.wVolume = data;
            }
            break;
            }
        }

        return TRUE;
        }
    }
    break;

    case WM_SYSCOMMAND:
        switch (wParam)
        {
            case ID_ABOUT_FORMAT:
                // about format dialog
                FmtAboutDialog(hDlg);
                break;

            case ID_REFRESH_UI:
                // refresh UI
                FmtRefreshAll();
                break;
        }
        break;

    case WM_CLOSE:
        EndDialog(hDlg, IDCLOSE);
        return TRUE;

    case WM_RBUTTONDOWN:
	{
		POINT pt;
		GetCursorPos(&pt);
		TrackPopupMenu(hPopupMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
	}
	break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_F5:
			// refresh UI
			FmtRefreshAll();
			break;
		case VK_F1:
			// about format dialog
			FmtAboutDialog(hDlg);
			break;
		default:
			break;
		}
		break;

	case WM_DESTROY:
		FmtCleanup();
		break;

    default:
        return FALSE;
    }

    return FALSE;
}
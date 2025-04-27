#pragma once
#include "FmtCore.h"

INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
    hBg = RGB(0xFF, 0xFF, 0xFF);
    hbrBackground = CreateSolidBrush(hBg);

    //AllocConsole();
    SetConsoleTitle(L"Format Console");
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_FORMVIEW), NULL, DialogProc);

}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:

        // get dialog
        hDialog = hDlg;

        // get controls
        hCbxDrives = GetDlgItem(hDlg, IDC_CBX_DRIVE);
        hCbxFileSystems = GetDlgItem(hDlg, IDC_CBX_FILESYSTEM);
        hCbxAllocSizes = GetDlgItem(hDlg, IDC_CBX_ALLOC_SIZE);
        FmtRefreshDrives(hCbxDrives);
        FmtRefreshFilesystems(hCbxFileSystems);
        FmtRefreshAllocSizes(hCbxAllocSizes);

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

    case WM_CLOSE:
        EndDialog(hDlg, IDCLOSE);
        return TRUE;

    default:
        return FALSE;
    }

    return FALSE;
}
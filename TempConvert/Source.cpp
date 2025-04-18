#include "TCCore.h"
#include "resource.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#define TEMPERATURE_CELSIUS		L"Celsius"
#define TEMPERATURE_FAHRENHEIT	L"Fahrenheit"
#define TEMPERATURE_KELVIN		L"Kelvin"

#define TEMP_INDEX_C	0
#define TEMP_INDEX_F	1
#define TEMP_INDEX_K	2

inline HWND hWindow;
inline HWND hCbxTempFrom;
inline HWND hCbxTempTo;
inline HWND hTxtResult;
inline HWND hTxtValue;

inline static void DrawResult(LPCWSTR lpszFrom, LPCWSTR lpszTo, DOUBLE dValue, DOUBLE dResult)
{
	if (hTxtResult == NULL) return;

	// draws text to the result field
	return;
}

inline void DrawError(LPCWSTR message)
{
	MessageBox(hWindow, message, L"Conversion", MB_OK | MB_ICONERROR);
	return;
}

inline static void AppConvertTemperature()
{
	int iTempFrom = SendMessage(hCbxTempFrom, CB_GETCURSEL, NULL, NULL);
	int iTempTo = SendMessage(hCbxTempFrom, CB_GETCURSEL, NULL, NULL);

	// definitions
	DOUBLE dValue = 0;
	DOUBLE dResult = 0;

	WCHAR lpszValue[MAX_PATH];
	GetWindowText(hTxtValue, lpszValue, MAX_PATH);
	dValue = _wtof(lpszValue);

	if (dValue == 0)
	{
		DrawError(L"Invalid input value.");
		return;
	}

	// pinpoint conversion units
	if (iTempFrom == TEMP_INDEX_C)
	{
		// convert from Celsius
		switch (iTempTo)
		{
			case TEMP_INDEX_C:
				// C to C
				
				break;

			case TEMP_INDEX_F:
				break;

			case TEMP_INDEX_K:
				break;

			default:
				break;
		}
	}

	else if (iTempFrom == TEMP_INDEX_F)
	{
		// convert from Fahrenheit
		switch (iTempTo)
		{
			case TEMP_INDEX_C:
				break;

			case TEMP_INDEX_F:
				break;

			case TEMP_INDEX_K:
				break;

			default:
				break;
		}
	}

	else if (iTempFrom == TEMP_INDEX_K)
	{
		// convert from Kelvin
		switch (iTempTo)
		{
			case TEMP_INDEX_C:
				break;

			case TEMP_INDEX_F:
				break;

			case TEMP_INDEX_K:
				break;

			default:
				break;
		}
	}

	else
	{
		// invalid index
	}

	return;
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			// set DWM status
			/*DWMNCRENDERINGPOLICY dwmValue = DWMNCRP_DISABLED;
			DwmSetWindowAttribute(hDlg, DWMWA_NCRENDERING_POLICY, &dwmValue, sizeof(DWMNCRENDERINGPOLICY));*/

			// get all control handles
			hWindow = hDlg;
			hCbxTempFrom = GetDlgItem(hDlg, IDC_CBX_TEMP_FROM);
			hCbxTempTo = GetDlgItem(hDlg, IDC_CBX_TEMP_TO);
			hTxtValue = GetDlgItem(hDlg, IDC_TXT_VALUE);
			hTxtResult = GetDlgItem(hDlg, IDC_TXT_RESULT);

			// set combo box items
			// Temperature From
			SendMessage(hCbxTempFrom, CB_ADDSTRING, NULL, (LPARAM)TEMPERATURE_CELSIUS);
			SendMessage(hCbxTempFrom, CB_ADDSTRING, NULL, (LPARAM)TEMPERATURE_FAHRENHEIT);
			SendMessage(hCbxTempFrom, CB_ADDSTRING, NULL, (LPARAM)TEMPERATURE_KELVIN);

			// Temperature To
			SendMessage(hCbxTempTo, CB_ADDSTRING, NULL, (LPARAM)TEMPERATURE_CELSIUS);
			SendMessage(hCbxTempTo, CB_ADDSTRING, NULL, (LPARAM)TEMPERATURE_FAHRENHEIT);
			SendMessage(hCbxTempTo, CB_ADDSTRING, NULL, (LPARAM)TEMPERATURE_KELVIN);

			// Set selected items
			SendMessage(hCbxTempFrom, CB_SETCURSEL, NULL, NULL);
			SendMessage(hCbxTempTo, CB_SETCURSEL, NULL, NULL);

			// adjust combo box sizes
			TcSetComboBoxDropdownHeight(hCbxTempFrom, 50);
			TcSetComboBoxDropdownHeight(hCbxTempTo, 50);
			return TRUE;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_BTN_CLOSE:
				{
					EndDialog(hDlg, IDCLOSE);
					return TRUE;
				}

				case IDC_BTN_CONVERT:
				{
					// convert temperature
					AppConvertTemperature();
					return TRUE;
				}
			}
			return FALSE;
		}

		case WM_SYSCOMMAND:
		{
			switch (LOWORD(wParam))
			{
			case SC_CLOSE:
				EndDialog(hDlg, IDCLOSE);
				return TRUE;
			}
			return FALSE;
		}

		default:
		{
			//DefWindowProc(hDlg, msg, wParam, lParam);
			return FALSE;
		}
	}

	return TRUE;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdLine, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_FORMVIEW), NULL, DialogProc);
	return 0;
}
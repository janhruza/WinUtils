#pragma once
#include <Windows.h>
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define ID_ITEM_REFRESH 0x1100
#define ID_ITEM_EXIT    0x1001    

typedef struct TargetWindow
{
	HWND hWnd;
	LPWSTR keys;
} TargetWindow;

inline BOOL CsValidateString(LPWSTR lpszData)
{
    if (!lpszData)
    {
        return FALSE;
    }

    int len = lstrlen(lpszData);
    if (len == 0)
    {
        return FALSE;
    }

    for (int x = 0; x < len; x++)
    {
        WCHAR c = lpszData[x];
        // exceptions
        if (c == VK_SPACE)
        {
            // space key
            continue;
        }

        if (c == VK_RETURN)
        {
            // enter key
            continue;
        }

        if (iswupper(c) == FALSE)
        {
            // is not upper
            lpszData[x] = towupper(c);
        }

        else
        {
            // is upper
            continue;
        }
    }

    return TRUE;
}

/// <summary>
/// Utilizes the SendInput function to send keystrokes to a given window.
/// </summary>
/// <param name="hWnd">Target window.</param>
/// <param name="lpszKeys">An array of keys stored as wide string.</param>
/// <returns>TRUE if keys were sent, otherwise FALSE.</returns>
inline BOOL CsSendKeys(HWND hWnd, LPCWSTR lpszKeys)
{
    if (!hWnd || !lpszKeys)
    {
        return FALSE;
    }

    int length = lstrlenW(lpszKeys);
    if (length == 0 || length > (MAX_PATH - 1))
    {
        return FALSE;
    }

    if (CsValidateString(lpszKeys) == FALSE)
    {
        // cant validate data
        return FALSE;
    }

    INPUT inputs[MAX_PATH * 2];
    ZeroMemory(inputs, sizeof(inputs));

    int inputIndex = 0;

    for (int x = 0; x < length; x++)
    {
        // Key down
        inputs[inputIndex].type = INPUT_KEYBOARD;
        inputs[inputIndex].ki.wVk = (WORD)lpszKeys[x];
        inputIndex++;

        // Key up
        inputs[inputIndex].type = INPUT_KEYBOARD;
        inputs[inputIndex].ki.wVk = (WORD)lpszKeys[x];
        inputs[inputIndex].ki.dwFlags = KEYEVENTF_KEYUP;
        inputIndex++;
    }

    // Activate the window
    SetForegroundWindow(hWnd);
    Sleep(100); // tiny delay to allow the window to come to foreground

    UINT sent = SendInput(inputIndex, inputs, sizeof(INPUT));
    if (sent != inputIndex)
    {
        return FALSE;
    }

    return TRUE;
}

#pragma once
#ifndef WNDHELPER_H
#define WNDHELPER_H

#include <Windows.h>

#define ID_CLEAR_ALL	1
#define ID_REFRESH		2
#define ID_CLOSE_WINDOW	3

void WHCreateAppMenu(HWND hDlg);
void WHShowContextMenu(HWND hDlg, int x, int y);
void WHAppendListBoxItem(HWND hListBox, LPCWSTR text);

#endif
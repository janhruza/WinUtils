#pragma once
#ifndef WNDHELPER_H
#define WNDHELPER_H

#include <Windows.h>

void WHCreateAppMenu(HWND hDlg);
void WHShowContextMenu(HWND hDlg, int x, int y);
void WHAppendListBoxItem(HWND hListBox, LPCWSTR text);

#endif
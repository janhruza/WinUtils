#pragma once
typedef struct tagTaskItem
{
	wchar_t szTitle[256];
	wchar_t szDescription[1024];
} TaskItem, *PTaskItem;

PTaskItem CreateTaskFromDialog(HWND hDlg);
void FreeTask(PTaskItem pTaskItem);
#include <Windows.h>
#include <stdio.h>

int wmain(int argc, const WCHAR *argv[])
{
	SYSTEMTIME ctm;
	GetLocalTime(&ctm);
	wprintf_s(L"Date: %02d/%02d/%04d\nTime: %2d:%2d:%2d\n", ctm.wDay, ctm.wMonth, ctm.wYear, ctm.wHour, ctm.wMinute, ctm.wSecond);
	return EXIT_SUCCESS;
}
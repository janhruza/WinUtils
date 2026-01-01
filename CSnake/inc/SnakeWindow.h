#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include "SnakeState.h"
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

//#pragma comment(linker,"\"/manifestdependency:type='win32' \
//name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
//processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

constexpr int BLOCK_SIZE = 50;
constexpr int BLOCK_WINDOW_WIDTH = 10;
constexpr int BLOCK_WINDOW_HEIGHT = 10;

constexpr int MI_REFRESH = 0x1000;
constexpr int MI_CLOSE = 0x1001;

/// <summary>
/// Simple class that creates a new window.
/// </summary>
class SnakeWindow
{
private:
	static bool bGameStarted;
	static bool bGamePaused;
	static SnakeState snake;

public:

	/// <summary>
	/// Window width (in blocks).
	/// </summary>
	int Width;

	/// <summary>
	/// Window height (in blocks).
	/// </summary>
	int Height;

	/// <summary>
	/// Empty constructor.
	/// </summary>
	SnakeWindow(HINSTANCE hInst, HMENU menu)
	{
		// perepare fields
		bGameStarted = false;
		bGamePaused = false;

		// create the window class
		wc = { 0 };
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = hInst;
		wc.lpszClassName = L"SnakeClass";
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.hCursor = LoadCursor(hInst, IDC_ARROW);
		wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);
		wc.hInstance = hInst;
		wc.lpfnWndProc = WndProc;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		RegisterClassEx(&wc);

		this->Width = BLOCK_WINDOW_WIDTH * BLOCK_SIZE;
		this->Height = BLOCK_WINDOW_HEIGHT * BLOCK_SIZE;

		// create a new window
		hWnd = CreateWindowEx(WS_EX_APPWINDOW,
			wc.lpszClassName,
			L"Snake Window",
			WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			this->Width,
			this->Height,
			nullptr,
			nullptr,
			hInst,
			this);

		// set the dwm properties
		/*COLORREF color = RGB(0x00, 0x00, 0x00);
		DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &color, sizeof(color));*/

		DWM_SYSTEMBACKDROP_TYPE sbt = DWMSBT_AUTO;
		DwmSetWindowAttribute(hWnd, DWMWA_SYSTEMBACKDROP_TYPE, &sbt, sizeof(sbt));

		BOOL darkMode = TRUE;
		DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));

		MARGINS margins = { -1 };
		DwmExtendFrameIntoClientArea(hWnd, &margins);

		BOOL ncRendering = TRUE;
		DwmSetWindowAttribute(hWnd, DWMWA_NCRENDERING_ENABLED, &ncRendering, sizeof(ncRendering));

		SetThemeAppProperties(STAP_ALLOW_NONCLIENT | STAP_ALLOW_CONTROLS | STAP_ALLOW_WEBCONTENT);

		// set app menu (if any)
		SetMenu(hWnd, menu);
	}

	/// <summary>
	/// Shows the window.
	/// </summary>
	/// <returns></returns>
	int Show() const
	{
		CenterWindow(hWnd);
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);

		MSG msg;
		while (GetMessage(&msg, nullptr, NULL, NULL))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return 0;
	}

	BOOL Resize(int rows, int cols)
	{
		this->Width = cols * BLOCK_SIZE;
		this->Height = rows * BLOCK_SIZE;
		return SetWindowPos(hWnd, nullptr, 0, 0, this->Width, this->Height, SWP_NOOWNERZORDER);
	}

	/// <summary>
	/// Destructor.
	/// </summary>
	~SnakeWindow()
	{
		CloseWindow(hWnd);
		DestroyWindow(hWnd);
	}

	HWND GetHandle()
	{
		return hWnd;
	}

private:
	WNDCLASSEX wc;
	HWND hWnd;

	static void CenterWindow(HWND hWnd)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);

		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);

		int l = (w - (rect.right - rect.left)) / 2;
		int t = (h - (rect.bottom - rect.top)) / 2;

		SetWindowPos(hWnd, nullptr, l, t, 0, 0, SWP_NOSIZE);
	}

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_SETCURSOR:
				if (LOWORD(lParam) == HTCLIENT) {
					SetCursor(LoadCursor(nullptr, IDC_ARROW));
					return TRUE;
				}
				// for non-client areas, let Windows do its job
				return DefWindowProc(hWnd, msg, wParam, lParam);

			case WM_CLOSE:
			{
				if (bGameStarted == false && bGamePaused == true)
				{
					DestroyWindow(hWnd);
					return TRUE;
				}

				if (MessageBox(hWnd, L"Do you want to exit My Snake?", L"Quit", MB_YESNOCANCEL | MB_ICONWARNING) == IDYES)
				{
					DestroyWindow(hWnd);
					return TRUE;
				}
			}
			break;

			/* Destroy window */
			case WM_DESTROY:
				OnWindowDestroy(hWnd);
				return TRUE;

			case WM_SIZING:
				UpdateWindow(hWnd);
				return DefWindowProc(hWnd, msg, wParam, lParam);

			case WM_KEYDOWN:
			{
				// handle key down
				switch (wParam)
				{
					case VK_SPACE:
						if (bGameStarted == false)
						{
							bGameStarted = true;
							SendMessage(hWnd, WM_PAINT, 0, 0);
							return TRUE;
						}

						else
						{
							// pause game
							SendMessage(hWnd, WM_PAINT, 0, 0);
							return true;
						}

					case VK_ESCAPE:
						bGamePaused = !bGamePaused;
						SendMessage(hWnd, WM_PAINT, 0, 0);
						return true;

				}

				return DefWindowProc(hWnd, msg, wParam, lParam);
			}

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);

				RECT rc = ps.rcPaint; // only the invalidated area
				int i = rc.left / BLOCK_SIZE;
				int j = rc.top / BLOCK_SIZE;

				RECT tempRect;
				tempRect.left = i * BLOCK_SIZE;
				tempRect.top = j * BLOCK_SIZE;
				tempRect.right = tempRect.left + BLOCK_SIZE;
				tempRect.bottom = tempRect.top + BLOCK_SIZE;

				POINT pt;
				GetCursorPos(&pt);
				ScreenToClient(hWnd, &pt);

				HBRUSH hBrush = (pt.x >= tempRect.left && pt.x <= tempRect.right &&
					pt.y >= tempRect.top && pt.y <= tempRect.bottom)
					? (HBRUSH)(COLOR_WINDOWTEXT + 1) // highlight
					: (HBRUSH)(COLOR_WINDOW + 1);    // default

				FillRect(hdc, &tempRect, hBrush);

				EndPaint(hWnd, &ps);
				return 0;
			}

			case WM_MOUSEMOVE:
			{
				POINT pt;
				GetCursorPos(&pt);
				ScreenToClient(hWnd, &pt);

				int blockX = pt.x / BLOCK_SIZE;
				int blockY = pt.y / BLOCK_SIZE;

				static int lastX = -1, lastY = -1;

				if (blockX != lastX || blockY != lastY)
				{
					RECT oldRect, newRect;

					if (lastX >= 0 && lastY >= 0) {
						oldRect.left = lastX * BLOCK_SIZE;
						oldRect.top = lastY * BLOCK_SIZE;
						oldRect.right = oldRect.left + BLOCK_SIZE;
						oldRect.bottom = oldRect.top + BLOCK_SIZE;
						InvalidateRect(hWnd, &oldRect, FALSE);
					}

					newRect.left = blockX * BLOCK_SIZE;
					newRect.top = blockY * BLOCK_SIZE;
					newRect.right = newRect.left + BLOCK_SIZE;
					newRect.bottom = newRect.top + BLOCK_SIZE;
					InvalidateRect(hWnd, &newRect, FALSE);

					lastX = blockX;
					lastY = blockY;
				}

				// update window title
				WCHAR text[128];
				wsprintfW(text, L"My Snake - %02dx%02d", pt.x, pt.y);
				SetWindowText(hWnd, text);

				return 0;
			}

			case WM_RBUTTONDOWN:
			{
				// create menu
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, MI_REFRESH, L"Reload\tF5");
				AppendMenu(hMenu, MF_SEPARATOR, 0, nullptr);
				AppendMenu(hMenu, MF_STRING, MI_CLOSE, L"Close\tAlt+F4");

				POINT pt;
				GetCursorPos(&pt);
				//ScreenToClient(hWnd, &pt);

				if (TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, nullptr) == TRUE)
				{
					DestroyMenu(hMenu);
				}
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}

			case WM_COMMAND:
			{
				switch (wParam)
				{
					case MI_REFRESH:
						RECT rect;
						GetClientRect(hWnd, &rect);
						InvalidateRect(hWnd, &rect, TRUE);
						break;

					case MI_CLOSE:
						SendMessage(hWnd, WM_CLOSE, 0, 0);
						break;
				}

				return DefWindowProc(hWnd, msg, wParam, lParam);
			}

			default:
				return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

protected:
	static void OnWindowDestroy(HWND hwnd)
	{
		PostQuitMessage(0);
		return;
	}
};

bool SnakeWindow::bGameStarted;
bool SnakeWindow::bGamePaused;
SnakeState SnakeWindow::snake;
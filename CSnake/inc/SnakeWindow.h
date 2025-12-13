#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include "SnakeState.h"
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

constexpr int BLOCK_SIZE = 25;
constexpr int BLOCK_WINDOW_WIDTH = 30;
constexpr int BLOCK_WINDOW_HEIGHT = 25;

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
		/*COLORREF color = RGB(255, 255, 255);
		DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &color, sizeof(color));*/

		DWM_SYSTEMBACKDROP_TYPE sbt = DWMSBT_MAINWINDOW;
		DwmSetWindowAttribute(hWnd, DWMWA_SYSTEMBACKDROP_TYPE, &sbt, sizeof(sbt));

		BOOL darkMode = TRUE;
		DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));

		MARGINS margins = { -1 };
		DwmExtendFrameIntoClientArea(hWnd, &margins);

		BOOL ncRendering = TRUE;
		DwmSetWindowAttribute(hWnd, DWMWA_NCRENDERING_ENABLED, &ncRendering, sizeof(ncRendering));

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
		this->Width = cols * BLOCK_SIZE + 31;
		this->Height = rows * BLOCK_SIZE + 2;
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
				RECT rect;
				PAINTSTRUCT ps;
				GetClientRect(hWnd, &rect);
				HDC hdc = BeginPaint(hWnd, &ps);
				RECT tempRect;

				for (int i = 0; i < BLOCK_WINDOW_WIDTH; i++)
				{
					int temp = 0;
					for (int j = 0; j < BLOCK_WINDOW_HEIGHT; j++)
					{
						tempRect.left = rect.left + (i * BLOCK_SIZE);
						tempRect.right = rect.left + (i * BLOCK_SIZE) + BLOCK_SIZE;
						tempRect.top = rect.top + (j * BLOCK_SIZE);
						tempRect.bottom = rect.top + (j * BLOCK_SIZE) + BLOCK_SIZE;

						// paint like a chess board
						if ((i + j) % 2 == 0)
						{
							// black
							FillRect(hdc, &tempRect, (HBRUSH)(COLOR_WINDOWTEXT + 1));
						}
						else
						{
							// white
							FillRect(hdc, &tempRect, (HBRUSH)(COLOR_WINDOW + 1));
						}
					}
				}

				EndPaint(hWnd, &ps);
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
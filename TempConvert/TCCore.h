#pragma once
#include <Windows.h>

typedef struct
{
	WCHAR Name[32];
	DOUBLE BoilingPoint;
	DOUBLE FreezingPoint;
} TempUnit;

inline void TcSetComboBoxDropdownHeight(HWND hComboBox, int dropdownHeight) {
	RECT rect;
	GetWindowRect(hComboBox, &rect);

	// Calculate the new height (dropdown height + edit/control height)
	int newHeight = rect.bottom - rect.top + dropdownHeight;

	// Adjust the combo box size
	SetWindowPos(hComboBox, NULL, 0, 0, rect.right - rect.left, newHeight,
		SWP_NOMOVE | SWP_NOZORDER);
}

/// <summary>
/// Converts Celsius to Fahrenheit.
/// </summary>
/// <param name="dValue">Value of Celsius.</param>
/// <returns></returns>
inline DOUBLE TcCTOF(DOUBLE dValue)
{
	return (dValue * (9.0 / 5.0)) + 32;
}

/// <summary>
/// Converts Celsius to Kelvin.
/// </summary>
/// <param name="dValue">Value of Celsius.</param>
/// <returns></returns>
inline DOUBLE TcCTOK(DOUBLE dValue)
{
	return dValue + 273.15;
}

/// <summary>
/// Converts Fahrenheit to Celsius.
/// </summary>
/// <param name="dValue">Value of Fahrenheit.</param>
/// <returns></returns>
inline DOUBLE TcFTOC(DOUBLE dValue)
{
	return (dValue - 32) * (5.0 / 9.0);
}

/// <summary>
/// Converts Fahrenheit to Kelvin.
/// </summary>
/// <param name="dValue">Value of Fahrenheit.</param>
/// <returns></returns>
inline DOUBLE TcFTOK(DOUBLE dValue)
{
	return (dValue - 32) * (5.0 / 9.0) + 273.15;
}

/// <summary>
/// Converts Kelvin to Celsius.
/// </summary>
/// <param name="dValue">Value of Kelvin.</param>
/// <returns></returns>
inline DOUBLE TcKTOC(DOUBLE dValue)
{
	return dValue - 273.15;
}

/// <summary>
/// Converts Kelvin to Fahrenheit.
/// </summary>
/// <param name="dValue">Value of Kelvin.</param>
/// <returns></returns>
inline DOUBLE TcKTOF(DOUBLE dValue)
{
	return (dValue - 273.15) * (9.0 / 5.0) + 32;
}
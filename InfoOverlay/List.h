#pragma once

#define LIST_NULL		NULL
#define LIST_MAX_ITEMS	64	// max number of items
#define LIST_OK			1
#define LIST_ERROR		0
#define LIST_INVALID	-1

void* aWindows[LIST_MAX_ITEMS];
void* aOverlays[LIST_MAX_ITEMS];

/// <summary>
/// Clears both lists of windows and associated overlay windows.
/// </summary>
/// <returns>Status code, either LIST_OK or LIST_ERROR.</returns>
inline int ListClear(void)
{
	// clear array one by one
	for (int x = 0; x < LIST_MAX_ITEMS; x++)
	{
		aWindows[x] = LIST_NULL;
		aOverlays[x] = LIST_NULL;
	}

	// return success
	return LIST_OK;
}

/// <summary>
/// Registers the window and it's overlay window.
/// </summary>
/// <param name="index">Index in the array.</param>
/// <param name="hWindow">Handle to the window.</param>
/// <param name="hOverlayWindow">Handle to the created overlay window.</param>
/// <returns>Status code - either LIST_OK or LIST_ERROR.</returns>
inline int ListSetItem(int index, void* hWindow, void* hOverlayWindow)
{
	// bounds check
	if (index >= LIST_MAX_ITEMS) return LIST_ERROR;

	// add item
	aWindows[index] = hWindow;
	aOverlays[index] = hOverlayWindow;
	return LIST_OK;
}

/// <summary>
/// Gets the unoccupied position from the windows and window overlays lists.
/// </summary>
/// <param name="No parameters."></param>
/// <returns>Index available for use, otherwise LIST_INVALID.</returns>
inline int ListGetFreeIndex(void)
{
	for (int x = 0; x < LIST_MAX_ITEMS; x++)
	{
		if (aWindows[x] == LIST_NULL || aOverlays[x] == LIST_NULL)
		{
			return x;
		}
	}

	return LIST_INVALID;
}

/// <summary>
/// Gets the index of the array of the first item of the given value.
/// </summary>
/// <param name="hWindow">The given value, representing a handle to a window.</param>
/// <returns>Index to the searching item or LIST_INVALID.</returns>
inline int ListGetIndex(void* hWindow)
{
	for (int x = 0; x < LIST_MAX_ITEMS; x++)
	{
		if (aWindows[x] == hWindow)
		{
			return x;
		}
	}

	return LIST_INVALID;
}

inline void* ListGetOverlay(int index)
{
	if (index >= LIST_MAX_ITEMS)
	{
		return LIST_NULL;
	}

	return aOverlays[index];
}
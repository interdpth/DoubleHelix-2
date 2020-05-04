

#include "MainHeader.h"
#include "GlobalVars.h"
#include "resource.h"


BOOL CALLBACK MapDataProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:	// when dialog is first created
		MapDataWIn = hwnd;


		
		break;
	case WM_CTLCOLORDLG:
		return (LONG)g_hbrBackground;
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(255, 255, 255));
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LONG)g_hbrBackground;
	}
	break;
	case BN_CLICKED:

		break;
	case WM_LBUTTONDOWN:

		break;
	case WM_COMMAND:

		break;

	case WM_VSCROLL:	// exact same idea, but V scroll instead of H scroll




	case WM_MOVE:

	case WM_SIZE:
	{
		//RECT MainWindowRect;
		//RECT thisWindowRect;
		//GetWindowRect(GetDlgItem(Main, grpBoxObj), &MainWindowRect);

		//GetWindowRect(hwnd, &thisWindowRect);
		//thisWindowRect.bottom -= thisWindowRect.top;
		//thisWindowRect.right -= thisWindowRect.left;

		//thisWindowRect.left = MainWindowRect.left + 12;
		//thisWindowRect.top = MainWindowRect.top + 10;

		///*thisWindowRect.bottom += thisWindowRect.top;
		//thisWindowRect.right += thisWindowRect.left;*/


		//MoveWindow(hwnd, thisWindowRect.left, thisWindowRect.top, thisWindowRect.right, thisWindowRect.bottom, true);
	}
	break;
	case WM_DESTROY:

		/// myBackBuffer.Destroy();
		break;
	}

	return 0;
}

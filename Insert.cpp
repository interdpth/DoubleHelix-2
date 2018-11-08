#include "MainHeader.h"
BOOL CALLBACK  InsertProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){

	

	switch (message) 
	{
	case WM_INITDIALOG:
		 hwndInsert=hWnd;
		break;
		
		
	case WM_LBUTTONDOWN:
		
		
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
			 
			case WM_DESTROY:
				ShowWindow(hWnd,SW_HIDE);
				break;	
	
		}
		break;
	case WM_VSCROLL:	// exact same idea, but V scroll instead of H scroll
		
		break;
	case WM_PAINT:
	
		break;
		case WM_SHOWWINDOW:
		 // LoadCurSprite();	
			break;
			
			
	}
	
//return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}
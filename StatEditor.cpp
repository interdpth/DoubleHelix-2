#include "MainHeader.h"
#include "GlobalVars.h"
#include "cStatEd.h"
#include "cOAMManager.h"
#include "SpriteObjectManager.h"

//May need two procs, i'm hoping for just one.
BOOL CALLBACK  StatEditorProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {



	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_INITDIALOG:
		
		GlobalVars::gblVars->StatEditor->me = hWnd;
		GlobalVars::gblVars->StatEditor->InitDialog();
		break;


	case WM_LBUTTONDOWN:


		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case lstNumbers:
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{

				GlobalVars::gblVars->StatEditor->SpritePreview->id = GlobalVars::gblVars->StatEditor->SpriteList.GetListIndex();
				GlobalVars::gblVars->StatEditor->LoadSprite();
				//Console.WriteLine("Fix LoadSprite ");
				GlobalVars::gblVars->StatEditor->SetupPreview();

			}
			break;
		case cmdSavestats:
			GlobalVars::gblVars->StatEditor->SaveSprite();
			break;
		case WM_DESTROY:
			ShowWindow(hWnd, SW_HIDE);
			break;

		}
		break;
	case WM_VSCROLL:	// exact same idea, but V scroll instead of H scroll

		break;
	case WM_PAINT:


		if (!GlobalVars::gblVars->StatEditor->SpritePreview)
		{
			break;
		}

		hdc = BeginPaint(hWnd, &ps);

		GlobalVars::gblVars->StatEditor->SpritePreview->PreviewSprite.TransBlit
		(hdc, 425, 42,
			(GlobalVars::gblVars->StatEditor->SpritePreview->Borders.right - GlobalVars::gblVars->StatEditor->SpritePreview->Borders.left),
			(GlobalVars::gblVars->StatEditor->SpritePreview->Borders.bottom - GlobalVars::gblVars->StatEditor->SpritePreview->Borders.top),
			0,
			0);
		EndPaint(hWnd, &ps);
		break;
	case WM_SHOWWINDOW:
		// LoadCurSprite();	
		break;


	}

	//return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}







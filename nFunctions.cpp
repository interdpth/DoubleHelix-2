#include "MainHeader.h"
#include "nMapBuffer.h"
#include "GlobalVars.h"
#include "DoorManager.h"
#include "MapEditingState.h"
#include "SpriteObjectManager.h"
#include "clsRoomScrolls.h"
//Scroller::Scroller()
//{
//	;
//}
//
//Scroller::~Scroller()
//{
//	;
//}



int  DrawTileRect(HDC hdc, long theColor, MousePointer mpointer, int mul)
{
	RECT            myrect;
	HBRUSH          curbrush = CreateSolidBrush(theColor);

	myrect.left = mpointer.sX * mul;
	myrect.top = mpointer.sY * mul;
	myrect.right = mpointer.Width * mul + myrect.left;
	myrect.bottom = myrect.top + mpointer.Height * mul;
	FrameRect(hdc, &myrect, curbrush);
	DeleteObject(curbrush);
	return 0;
}



int  DrawTileRect(HDC hdc, MousePointer mpointer, int mul)
{

	return DrawTileRect(hdc, RGB(255, 0, 0),  mpointer,  mul);
	 
}

LRESULT CALLBACK InputBoxProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{

	int             choice = -1;
	char            buf[200];

	switch (message)
	{

	case WM_SHOWWINDOW:
		SetWindowText(GetDlgItem(hWnd, lblEnter), procstring);
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == cmdInputOK)
		{
			GetWindowText(GetDlgItem(hWnd, txtInput), buf, 4);
			sscanf(buf, "%X", &choice);
			ShowWindow(hWnd, SW_HIDE);

			EndDialog(hWnd, choice);
		}
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int             LoadInput(long val)
{
	int             blah;

	blah = DialogBox(hGlobal, MAKEINTRESOURCE(frmInputBox), 0, (int(__stdcall *) (struct HWND__ *, unsigned int, unsigned int, long)) InputBoxProc);
	if (blah > -1)
		return blah;
	return val;
}



int DoScroll(LPARAM lParam) {
	if (!movingScroll) {
		movingScrollIndex = RD1Engine::theGame->mgrScrolls->Findmeascroll(((GetX(lParam) / 16) + MapHorizScroll->GetIndex()), ((GetY(lParam) / 16) + MapVertScroll->GetIndex()), cboScroll.GetListIndex());
		if (movingScrollIndex) {
			movingScroll = 1;

		}
		else {
			movingScroll = movingScrollIndex = 0;
		}
	}
	return 0;

}
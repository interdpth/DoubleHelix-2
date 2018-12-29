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
			sscanf(buf, "%x", &choice);
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






int             DrawClipIdent()
{
	int             currclip = Combos[cClip].GetListIndex();
	RECT            blah;
	int             thisX = 0;
	int             thisY = 0;
	int             x = 0;
	int             y = 0;

	HBRUSH          curbrush = CreateSolidBrush(RGB(255, 255, 255));

	for (thisY = 0; thisY < RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y; thisY++)
	{
		for (thisX = 0; thisX < RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X; thisX++)
		{

			if (RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata)->TileBuf2D[thisX + (thisY * (RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata)->X))] == currclip)
			{

				blah.left = thisX * 16;
				blah.right = thisX * 16 + 16;
				blah.top = thisY * 16;
				blah.bottom = thisY * 16 + 16;

				FrameRect(RD1Engine::theGame->ThisBackBuffer.DC(), &blah, curbrush);
				

			}
		}
	}
	DeleteObject(curbrush);
	InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
	return 0;
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
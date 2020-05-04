
#include "MainHeader.h"
#include "GlobalVars.h"
#include "resource.h"
#include "clsRoomScrolls.h"
void LoadScrollControls(Scroller *scroll);

BOOL CALLBACK ScrollWndProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	
	scrollData blankData;
	editingStates thisState = editingStates::MAP;
	switch (message)
	{
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
	case WM_INITDIALOG:	// when dialog is first created
		ScrollWIn = hwnd;

		
		
		break;
	case BN_CLICKED:

		break;
	case WM_LBUTTONDOWN:

		break;
	case WM_COMMAND:
	{	thisState= RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetState();

		switch (LOWORD(wParam))
		{
		
		case btnAddScroll:
			memset(&blankData.rect, 0, sizeof(MousePointer));
			blankData.rect.sX = 0;
			blankData.rect.sY = 0;
			blankData.rect.Height = 1;
			blankData.rect.Width = 1;
			blankData.unkData = 0xFFFFFFFF;
			
			RD1Engine::theGame->mgrScrolls->GetScrollInfo()->Scrolls.push_back(blankData);
			RD1Engine::theGame->mgrScrolls->GetScrollInfo()->Number = RD1Engine::theGame->mgrScrolls->GetScrollInfo()->Scrolls.size();
			RD1Engine::theGame->DrawStatus.dirty = true;
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			LoadScrollControls(RD1Engine::theGame->mgrScrolls->GetScrollInfo());
				break;
		case btnDeleteScroll:
			if (thisState == editingStates::SCROLL) {
				int scrollid = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetObjId();
				if (scrollid != 0xffffffff)
				{

					RD1Engine::theGame->mgrScrolls->GetScrollInfo()->Scrolls.erase(RD1Engine::theGame->mgrScrolls->GetScrollInfo()->Scrolls.begin() + scrollid);
				}
			}
			RD1Engine::theGame->mgrScrolls->GetScrollInfo()->Number = RD1Engine::theGame->mgrScrolls->GetScrollInfo()->Scrolls.size();
			LoadScrollControls(RD1Engine::theGame->mgrScrolls->GetScrollInfo());
			break;
		}
	}
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

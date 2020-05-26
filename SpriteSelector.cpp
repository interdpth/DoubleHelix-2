#include "MainHeader.h"
#include "cOAMManager.h"
#include "GlobalVars.h"
#include "resource.h"
#include "SpriteObjectManager.h"
#include "FrameManager.h"
#include "Frames.h"

BOOL CALLBACK  SSProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {


	Frame* targetFrame = NULL;
	PAINTSTRUCT ps;
	HDC hdc;
	char buf[256];
	switch (message)
	{
	case WM_INITDIALOG:
		hwndSS = hWnd;
		dispic = 0;
		SpriteTabIndex = 0;
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
	case WM_LBUTTONDOWN:


		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case btnAddObject:
			RD1Engine::theGame->mainRoom->mgrSpriteObjects->AddSpriteObject(comboSpriteSet.GetListIndex(), dispic);
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			UiState::stateManager->ShowObj();
			UiState::stateManager->ForceRedraw();
			break;
		case btnDeleteObj:
			RD1Engine::theGame->mainRoom->mgrSpriteObjects->DeleteSpriteObject(comboSpriteSet.GetListIndex(), RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetObjId());
		
			RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(0);
			UiState::stateManager->ShowObj();
			UiState::stateManager->ForceRedraw();
			break;

		case cmdPrev:
			if (dispic == 0) return 0;
			dispic -= 1;
			sprintf(buf, "Sprite: %d", dispic);
			SetWindowText(GetDlgItem(hWnd, lblSpriteblah), buf);
			InvalidateRect(hwndSS, 0, 1);
			break;
		case cmdNext:
			if (!(dispic < RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites.size() - 1)) return 0;
			dispic += 1;
			sprintf(buf, "Sprite: %d", dispic);
			SetWindowText(GetDlgItem(hWnd, lblSpriteblah), buf);
			InvalidateRect(hwndSS, 0, 1);
			break;
		case cmdSSOk:
			SetCurSprite();
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			UiState::stateManager->ForceRedraw();
			break;
		}
		break;
	case WM_VSCROLL:	// exact same idea, but V scroll instead of H scroll

		break;
	case WM_PAINT:
	{
		if (RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites.size() > 0) {


			targetFrame = RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites.at(dispic)->GetStaticFrame();

			hdc = BeginPaint(hWnd, &ps);



			//GetWindowRect(GetDlgItem(hWnd,fraSpriteSS),&sprdst);
			targetFrame->theSprite->PreviewSprite.GetFullImage()->Blit(hdc, 64, 100,
				targetFrame->theSprite->Borders.right - targetFrame->theSprite->Borders.left,
				targetFrame->theSprite->Borders.bottom - targetFrame->theSprite->Borders.top,
				0,
				0);
			EndPaint(hWnd, &ps);
		}
	}
	break;
	case WM_MOVE:
	case WM_SIZE:
	{
	}
	break;
	case WM_SHOWWINDOW:
		LoadCurSprite();
		break;


	}

	//return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}

int SetCurSprite() {
	int beta;
	int set = comboSpriteSet.GetListIndex();
	vector<ObjectSprite*> * list = &RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[set];

	list->at(SpriteTabIndex)->Creature = list->at(SpriteTabIndex)->Creature & 0xF0 | dispic;

	return 0;
}

int LoadCurSprite() {     //Sets dispic for starting.
	if (SpriteTabIndex == -1)
	{
		return -1;
	}

	SpriteObjectManager* mgrSpriteObjects = RD1Engine::theGame->mainRoom->mgrSpriteObjects;;
	vector<ObjectSprite*>* SpriteObjects = NULL;
	int newValue = comboSpriteSet.GetListIndex();
	if (mgrSpriteObjects)
	{
		
			SpriteObjects = &mgrSpriteObjects->SpriteObjects[newValue];
			if (SpriteObjects && SpriteObjects->size() != 0)
			{
				dispic = ((SpriteObjects->at(SpriteTabIndex)->Creature) & 0xf) - 1;
			}
		
	}
	

	return 0;
}

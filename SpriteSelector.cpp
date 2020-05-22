#include "MainHeader.h"
#include "cOAMManager.h"
#include "GlobalVars.h"
#include "resource.h"
#include "SpriteObjectManager.h"
#include "FrameManager.h"
#include "Frames.h"

BOOL CALLBACK  SSProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){


	Frame* targetFrame = NULL;
	PAINTSTRUCT ps;
	HDC hdc;
	char buf[256];
	switch (message) 
	{
	case WM_INITDIALOG:
		hwndSS = hWnd;
		dispic=0;
		SpriteTabIndex=0;
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
		switch(LOWORD(wParam)){
		case btnAddObject:
			RD1Engine::theGame->mainRoom->mgrSpriteObjects->AddSpriteObject(comboSpriteSet.GetListIndex());
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			UiState::stateManager->ShowObj();
			break;
		case btnDeleteObj:
			RD1Engine::theGame->mainRoom->mgrSpriteObjects->DeleteSpriteObject(comboSpriteSet.GetListIndex(), RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetObjId());
			UiState::stateManager->ForceRedraw();
			RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(0);
			UiState::stateManager->ShowObj();
			break;
			case cmdSSCancel:
                	ShowWindow(hWnd,SW_HIDE);
					return 0;
				break;
			case cmdPrev:
				if(dispic==0) return 0;
				  dispic-=1;
				  sprintf(buf,"Sprite: %d",dispic);
				  SetWindowText(GetDlgItem(hWnd,lblSpriteblah),buf);
				  InvalidateRect(hwndSS,0,1);
				break;
			case cmdNext:
				if(!(dispic < RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites.size()-1)) return 0;
				  dispic+=1;
				  sprintf(buf,"Sprite: %d",dispic);
				  SetWindowText(GetDlgItem(hWnd,lblSpriteblah),buf);
				  InvalidateRect(hwndSS,0,1);
				break;
			case cmdSSOk:
                 SetCurSprite();
				 RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
                 ShowWindow(hWnd,SW_HIDE);
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
		//RECT MainWindowRect;
		//RECT thisWindowRect;
		//GetWindowRect(GetDlgItem(Main, grpBoxObj), &MainWindowRect);

		//GetWindowRect(hwnd, &thisWindowRect);
		//thisWindowRect.bottom -= thisWindowRect.top;
		//thisWindowRect.right -= thisWindowRect.left;
		//
		//thisWindowRect.left = MainWindowRect.left + 12;
		//thisWindowRect.top = MainWindowRect.top + 10;

		///*thisWindowRect.bottom += thisWindowRect.top;
		//thisWindowRect.right += thisWindowRect.left;*/


		//MoveWindow(hwnd, thisWindowRect.left, thisWindowRect.top, thisWindowRect.right, thisWindowRect.bottom, true);
	}
	break;
		case WM_SHOWWINDOW:
		  LoadCurSprite();	
			break;
			
			
	}
	
//return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}
int SetCurSprite(){
int beta; 
    switch(comboSpriteSet.GetListIndex()){
			case  0:
				beta = RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[0].Enemies[SpriteTabIndex].Creature/0x10;
				RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[0].Enemies[SpriteTabIndex].Creature= beta*16 + dispic+1;
				//RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[0].Enemies[SFSS].Creature=beta;
             
			   break;
            case 1:
				beta = RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Enemies[SpriteTabIndex].Creature/0x10;
				RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Enemies[SpriteTabIndex].Creature= beta*16 + dispic+1;
			   break;
			case 2:
			beta = RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Enemies[SpriteTabIndex].Creature/0x10;
				RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Enemies[SpriteTabIndex].Creature= beta*16 + dispic+1;
			   break;
	}
return 0;
}
int LoadCurSprite(){     //Sets dispic for starting.
	if(SpriteTabIndex==-1)
	{
		return -1;
	}

	SpriteObjectManager* mgrSpriteObjects = RD1Engine::theGame->mainRoom->mgrSpriteObjects;;
	nEnemyList* SpriteObjects = NULL;
	int newValue = comboSpriteSet.GetListIndex();
	if (mgrSpriteObjects)
	{
		if(newValue && mgrSpriteObjects->SpriteObjects.size()>=newValue) {
	
			SpriteObjects = &mgrSpriteObjects->SpriteObjects[newValue];
	
		}
	}
	if (SpriteObjects && SpriteObjects->Enemies.size() != 0)
	{
		dispic = ((SpriteObjects->Enemies[SpriteTabIndex].Creature) & 0xf) - 1;
	}

	return 0;
}

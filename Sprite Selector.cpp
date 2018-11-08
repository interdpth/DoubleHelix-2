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
		SFSS=0;
		break;
		
		
	case WM_LBUTTONDOWN:
		
		
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case btnAddObject:
			RD1Engine::theGame->mainRoom->mgrSpriteObjects->AddSpriteObject(Combos[cSpriteSet].GetListIndex());
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);

			break;
		case btnDeleteObj:
			RD1Engine::theGame->mainRoom->mgrSpriteObjects->DeleteSpriteObject(Combos[cSpriteSet].GetListIndex(), RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetObjId());
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
			RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(0);
			UiState::stateManager->ShowObj();
			break;
			case cmdSSCancel:
                	ShowWindow(hWnd,SW_HIDE);
					return 0;
				break;
			case cmdPrev:
				if((dispic-1)<0) return 0;
				  dispic-=1;
				  sprintf(buf,"Sprite: %d",dispic);
				  SetWindowText(GetDlgItem(hWnd,lblSpriteblah),buf);
				  InvalidateRect(hwndSS,0,1);
				break;
			case cmdNext:
				if((dispic+1 >=RD1Engine::theGame->mgrOAM->maxsprite)) return 0;
				  dispic+=1;
				  sprintf(buf,"Sprite: %d",dispic);
				  SetWindowText(GetDlgItem(hWnd,lblSpriteblah),buf);
				  InvalidateRect(hwndSS,0,1);
				break;
			case cmdSSOk:
                 SetCurSprite();
				 RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
                 ShowWindow(hWnd,SW_HIDE);
				break;
		
		
		}
		break;
	case WM_VSCROLL:	// exact same idea, but V scroll instead of H scroll
		
		break;
	case WM_PAINT:
	{
		targetFrame = RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites.at(dispic)->GetStaticFrame();
		
		hdc = BeginPaint(hWnd, &ps);



		//GetWindowRect(GetDlgItem(hWnd,fraSpriteSS),&sprdst);
		targetFrame->theSprite->PreviewSprite.GetFullImage()->Blit(hdc, 230, 59,
			targetFrame->theSprite->Borders.right- targetFrame->theSprite->Borders.left,
			targetFrame->theSprite->Borders.bottom - targetFrame->theSprite->Borders.top,
			0,
			0);
		EndPaint(hWnd, &ps);
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
    switch(Combos[cSpriteSet].GetListIndex()){
			case  0:
				beta = RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[0].Enemies[SFSS].Creature/0x10;
				RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[0].Enemies[SFSS].Creature= beta*16 + dispic+1;
				//RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[0].Enemies[SFSS].Creature=beta;
             
			   break;
            case 1:
				beta = RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Enemies[SFSS].Creature/0x10;
				RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Enemies[SFSS].Creature= beta*16 + dispic+1;
			   break;
			case 2:
			beta = RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Enemies[SFSS].Creature/0x10;
				RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Enemies[SFSS].Creature= beta*16 + dispic+1;
			   break;
	}
return 0;
}
int LoadCurSprite(){     //Sets dispic for starting.
	if(SFSS==-1)
	{
		return -1;
	}
    switch(Combos[cSpriteSet].GetListIndex()){
			case  0:
             dispic = ((RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[0].Enemies[SFSS].Creature)&0xf)-1;
			   break;
            case 1:
				dispic = ((RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Enemies[SFSS].Creature)&0xf)-1;
			   break;
			case 2:
				dispic = ((RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[2].Enemies[SFSS].Creature)&0xf)-1;
			   break;
	}
	
	return 0;
}

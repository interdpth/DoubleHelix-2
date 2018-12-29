#include "MainHeader.h"
#include "DoorManager.h"

int HandleRightClick(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam, bool bScrollCheck)
{
	//int i=0;
	//int td=0;
	//		if(bScrollCheck){
	//		    i= BaseGame::theGame->mgrScrolls->Findmeascroll(((GetX(lParam)/16)+MapHorizScroll->GetIndex()),((GetY(lParam)/16)+ MapVertScroll->GetIndex()));
	//			movingScroll=1;
	//			movingScrollIndex=i;
	//		 }else{
	//	
	//			i = 0;// Gimmeasprite(((GetX(lParam) / 16) + MapHorizScroll->GetIndex()), ((GetY(lParam) / 16) + MapVertScroll->GetIndex()), 0);
	//		if(i!=-1){
	//		   SFSS = i; 
	//		   ShowWindow(hwndSS, SW_SHOW);
	//			   LoadCurSprite();
	//			   return 0;
	//		}
	//			td= BaseGame::theGame->mgrDoors->GimmeAdoor(((GetX(lParam)/16)+MapHorizScroll->GetIndex()),((GetY(lParam)/16)+ MapVertScroll->GetIndex()));
	//	 		if(td!=-1){
	//				ShowWindow(DoorWin,SW_SHOW);
	//				BaseGame::theGame->mgrDoors->LoadThisDoor(td);
	//				   
	//				   return 0;
	//			}
 //        
	//			if(!((mpMap.Width > 1) || (mpMap.Height >1))){
	//				mpMap.uX = mpMap.sX = (GetX(lParam)/16);
	//				mpMap.uY = mpMap.sY = (GetY(lParam)/16);
	//				mpMap.cX = mpMap.sX;
	//				mpMap.cY = mpMap.sY;
	//				mpMap.Width = 1;
	//				mpMap.Height =1;
 //   
	//			}else{//Reset
	//				mpMap.Width = 1;
	//				mpMap.Height = 1;
	//			}
	//		
	//		}
		return 0;
		
}

		

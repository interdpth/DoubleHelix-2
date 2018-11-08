#include "cClipboard.h"
#include "ResourceAndDefines.h"
#include "GlobalVars.h"
#include "nMapBuffer.h"
#include "MapManager.h"
#include "globals.h"
#define GetX(lParam) LOWORD(lParam)
#define GetY(lParam) HIWORD(lParam)
extern cClipboard ClipBoard;
/*class cClipboard{
public: 
	cClipboard();
	~cClipboard();
	unsigned char copy;
    sCombos cboCurClip;
	unsigned short thisclipboard[16][256][4];
    Image  cb[16];
    HWND me;
	HWND cb;
	int Create();
	int Erase();
    int inuse;
}
BOOL CALLBACK	    ClipBoardProc (HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK    CBImageProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
*/

BOOL CALLBACK ClipBoardProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){



	switch (message) 
	{
	case WM_INITDIALOG:
//	ClipBoard.me= hWnd;
//	ClipBoard.Create();
		break;
		
		
	case WM_LBUTTONDOWN:
		
		
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		
		
			case WM_DESTROY:
				ShowWindow(hWnd,SW_HIDE);
				break;
			case cboCBNo:
                if(HIWORD(wParam)==CBN_SELCHANGE){
                   ClipBoard.curclip= ClipBoard.cboCurClip.GetListIndex();
				   InvalidateRect(ClipBoard.cb,0,1);
				  //ClipBoard.CheckforBlanks(N);
				}
				break;
			case cmdErase:
				 ClipBoard.Erase(ClipBoard.curclip,0xFFFF);

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

int SetTiles(LPARAM lParam){//Layer to use, XY value, if it's bigger then 16x16
int baseX=((GetX(lParam)/16));
int baseY=((GetY(lParam)/16)) ;
int y=0;
int x=0;
int ny=0;
int nx=0;
int i=0;
nMapBuffer* Layer;
int width=0; 
if(!MPToUse){//Map
	for(i =0;i<4;i++){
		if(i == 0){
			if(GlobalVars::gblVars->checkBoxForeground.value()){
				Layer=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround);
			}else{
				continue;
			}
		}else if(i==1){
			if(GlobalVars::gblVars->checkBoxLevel.value()){
				Layer=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData);
			}else{
				continue;
			}

		}else if(i == 2){
			if(GlobalVars::gblVars->checkBoxBackground.value()){
				Layer=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer);
			}else{
				continue;
			}
		}else if(i==3){
			if(GlobalVars::gblVars->checkBoxClip.value()){
				Layer=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata);
			}else{
				continue;
			}

		}else{
				break;
		}
			width=Layer->X;

	  for(y=0;y<mpMap.Height;y++){
			for(x=0;x<mpMap.Width;x++){
				if((baseX +x) +  (baseY + y)* (16)>=256) continue; 
				ClipBoard.thisclipboard[ClipBoard.curclip][(baseX +x) +  (baseY + y)* (16)][i] = Layer->TileBuf2D[Tile +x + (y * width) ];	  
			    ClipBoard.DrawSlots();
			}
	  }
	}
}else if(MPToUse){//Tileset 
		if(GlobalVars::gblVars->checkBoxForeground.value()) i = 0;
		if(GlobalVars::gblVars->checkBoxLevel.value())      i = 1;
		if(GlobalVars::gblVars->checkBoxBackground.value()) i = 2;
		if(GlobalVars::gblVars->checkBoxClip.value())       i = 3;
 for(y=0;y<mpTileset.Height;y++){
		for(x=0;x<mpTileset.Width;x++){
//				ClipBoard.thisclipboard[ClipBoard.curclip][(baseX +x) +  (baseY + y)* (16)][i]= Tile +x + (y * 16);	  
		}
	}
 ClipBoard.DrawSlots();
}
 



return 0; 
}



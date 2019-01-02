#include "MainHeader.h"
#include "GlobalVars.h"
#include "BaseGame.h"
long ho;
int CreateMiniMapTileImage(int Width, int Height);
int CreateMiniMapImage(int Width, int Height);
int DrawMapLoc(HDC hdc);

void Create(int romSwitch)
{

	RECT tr;
	int i = 0;
	char buf[1024] = { 0 };
	MiniMapClass::miniMapEditor->Vert.SetCnt(GetDlgItem(hwndMM, chkVert));
	MiniMapClass::miniMapEditor->Horz.SetCnt(GetDlgItem(hwndMM, chkHorz));
	MiniMapClass::miniMapEditor->Map = new Image(2048, 2048);

	GetWindowRect(GetDlgItem(hwndMM, framt), &tr);
	//Now would be the best time to create an HWND
	CreateMiniMapTileImage(tr.right - tr.left, tr.bottom - tr.top);
	GetWindowRect(GetDlgItem(hwndMM, fraMinimap), &tr);
	CreateMiniMapImage(tr.right - tr.left, tr.bottom - tr.top);

	MiniMapClass::miniMapEditor->cboPalette.Init(GetDlgItem(hwndMM, cboMPal));
	MiniMapClass::miniMapEditor->cboPalette.Clear();
	for (i = 0; i<9; i++) {
		sprintf(buf, "%X", i);
		MiniMapClass::miniMapEditor->cboPalette.Additem(buf);
	}
	MiniMapClass::miniMapEditor->cboPalette.SetListIndex(0);
	MiniMapClass::miniMapEditor->cboMArea.Init(GetDlgItem(hwndMM, cboMAreaC));
	MiniMapClass::miniMapEditor->cboMArea.Clear();
	if (!romSwitch)
	{
		MiniMapClass::miniMapEditor->cboMArea.Additem("Brinstar");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Kraid");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Norfair");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Ridley");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Tourian");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Crateria");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Chozodia");
		MiniMapClass::miniMapEditor->cboMArea.Additem("???");
		MiniMapClass::miniMapEditor->cboMArea.Additem("???");
		MiniMapClass::miniMapEditor->cboMArea.Additem("???");
		MiniMapClass::miniMapEditor->cboMArea.Additem("???");
	}
	else {
		MiniMapClass::miniMapEditor->cboMArea.Additem("Main Deck");
		MiniMapClass::miniMapEditor->cboMArea.Additem("SRX");
		MiniMapClass::miniMapEditor->cboMArea.Additem("TRO");
		MiniMapClass::miniMapEditor->cboMArea.Additem("PYR");
		MiniMapClass::miniMapEditor->cboMArea.Additem("AQA");
		MiniMapClass::miniMapEditor->cboMArea.Additem("NOC");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Debug1");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Debug2");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Debug3");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Uknown");
		MiniMapClass::miniMapEditor->cboMArea.Additem("Uknown");
	}
	MiniMapClass::miniMapEditor->MiniMapClass::miniMapEditor->MFMap.SetListIndex(0);
	MiniMapClass::miniMapEditor->Tileset = new Image(tr.right - tr.left, tr.bottom - tr.top);
}
BOOL CALLBACK  MiniProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){

	unsigned char Width=0;
	unsigned char Height=0;
	
	switch (message) 
	{
	case WM_INITDIALOG:
		hwndMM = hWnd;
		if (MiniMapClass::miniMapEditor == NULL)
		{
			MiniMapClass::miniMapEditor = new MiniMapClass();
		}
		Create(GameConfiguration::mainCFG->RomSwitch);
		MiniMapClass::miniMapEditor->Create();
		break;
		
		
	case WM_LBUTTONDOWN:
		
		
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case cboMPal:
			MiniMapClass::miniMapEditor->DrawTileset(MiniMapClass::miniMapEditor->Tileset, MiniMapClass::miniMapEditor->cboPalette.GetListIndex());
			break;
		case cboMAreaC:
			if(HIWORD(wParam) == CBN_SELCHANGE){
				MiniMapClass::miniMapEditor->DecompressMap(MiniMapClass::miniMapEditor->cboMArea.GetListIndex());
				MiniMapClass::miniMapEditor->DrawMap(MiniMapClass::miniMapEditor->Map);
				drawmm=0;
			}
			break;
		case cmdMMSave:
			if(drawmm)MiniMapClass::miniMapEditor->SaveMap();
		
			ho=((RD1Engine::theGame->RoomOffsets[Combos[cArea].GetListIndex()]-0x8000000) + Combos[cRoom].GetListIndex()*0x3C);
		
		MemFile::currentFile->seek(ho+0x35);
	    MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomX,sizeof(unsigned char),1,GBA.ROM);
		MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomY,sizeof(unsigned char),1,GBA.ROM);
	

			break;
		case WM_DESTROY:
			ShowWindow(hWnd,SW_HIDE);
			hwndMM = 0;

			break;	
	
		}
		break;
	
		case WM_SHOWWINDOW:
			SendMessage(hwndMM,WM_COMMAND,LOWORD(cboMAreaC),0);
			break;
			
			
	}
	
	//	return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}

int DrawMiniMap(){
	
	
	return 0;
}


LRESULT CALLBACK MiniTilesetProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){
	HDC hdc=NULL;
	PAINTSTRUCT ps;
	char txt[256]={0};
	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MiniMapClass::miniMapEditor->Tileset->Blit(hdc, 0,0,128 ,232,0,0);
		EndPaint(hWnd, &ps);
		break;
		
  		case WM_LBUTTONDOWN:
			MiniMapClass::miniMapEditor->curtile= (MiniMapClass::miniMapEditor->cboPalette.GetListIndex() * 0x1000) + (GetX(lParam)/8) + ((GetY(lParam)/8)*	16);
			
			break;
		case WM_RBUTTONDOWN:
            sprintf(txt,"Tile is: %X",(GetX(lParam)/8) + ((GetY(lParam)/8)*	16));
			MessageBox(0,txt,"Tile",MB_OK);

			break;
        case WM_COMMAND:
			switch(LOWORD(wParam)){
			case IDCANCEL:
				ShowWindow(hWnd,SW_HIDE);
				break;	
			}
			break;	
	}
	
    return DefWindowProc(hWnd, message, wParam, lParam);
	
}


int CreateMiniMapTileImage(int Width, int Height) {
	WNDCLASSEX blab5;
	memset(&blab5, 0, sizeof(blab5));
	blab5.cbSize = sizeof(blab5);		   // size of structure 
	blab5.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blab5.lpfnWndProc = &MiniTilesetProc;	  // points to window procedure 
	blab5.cbClsExtra = NULL;				// no extra class memory 
	blab5.cbWndExtra = NULL; 			   // no extra window memory 
	blab5.hInstance = hGlobal;		   // handle to instance 
	blab5.hIcon = NULL; 			 // predefined app. icon 
	blab5.hCursor = NULL;				// predefined arrow 
										//blab5.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH);				   // white background brush 
	blab5.lpszMenuName = NULL;    // name of menu resource 
	blab5.lpszClassName = "MapTiles";  // name of window class 


									   // Register the window class. 
	RegisterClassEx(&blab5);

	MiniMapClass::miniMapEditor->hwndMiniTileset = CreateWindowEx(NULL, "MapTiles", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD, 2, 13, 131, 240, GetDlgItem(hwndMM, framt), 0, hGlobal, 0);


	ShowWindow(MiniMapClass::miniMapEditor->hwndMiniTileset, SW_SHOW);
	return 0;
}

int CreateMiniMapImage(int Width,int Height){
	WNDCLASSEX blab6;
	memset(&blab6,0,sizeof(blab6));
	blab6.cbSize = sizeof(blab6);		   // size of structure 
	blab6.style = CS_HREDRAW | 
		CS_VREDRAW; 				   // redraw if size changes 
	blab6.lpfnWndProc = &MiniGraphicProc;	  // points to window procedure 
	blab6.cbClsExtra = NULL;				// no extra class memory 
	blab6.cbWndExtra =NULL; 			   // no extra window memory 
	blab6.hInstance = hGlobal;		   // handle to instance 
	blab6.hIcon = NULL; 			 // predefined app. icon 
	blab6.hCursor = NULL;				// predefined arrow 
	//blab6.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH);				   // white background brush 
	blab6.lpszMenuName =  NULL;    // name of menu resource 
	blab6.lpszClassName = "MiniMap";  // name of window class 
	
	
	// Register the window class. 
	RegisterClassEx(&blab6);
	
	MiniMapClass::miniMapEditor->hwndMiniMap = CreateWindowEx(NULL,"MiniMap",NULL, WS_TABSTOP | WS_VISIBLE|WS_CHILD ,3,12,Width-3,Height-12,GetDlgItem(hwndMM,fraMinimap),0,hGlobal,0);
	
	
	ShowWindow(MiniMapClass::miniMapEditor->hwndMiniTileset,SW_SHOW);	
	return 0;
}
LRESULT CALLBACK MiniGraphicProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){
	HDC hdc=NULL;
	PAINTSTRUCT ps;
	int srcx=0;
	int srcy=0;
	char txt[256]={0};
	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if(currentRomType==1){
			if(MiniMapClass::miniMapEditor->MFMap.GetListIndex()==1){
				srcx = 15;
			    srcy=0;
			}else if(MiniMapClass::miniMapEditor->MFMap.GetListIndex() == 2){
				srcy=15;
			    srcx=0;
            }else if(MiniMapClass::miniMapEditor->MFMap.GetListIndex() == 3){
                srcy=15;
			    srcx=15;
			}
		}
		MiniMapClass::miniMapEditor->Map->Blit(hdc, 0,0,256 ,256,0,0);
		DrawMapLoc(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:

		MiniMapClass::miniMapEditor->RawMap[(GetX(lParam)/8) + ((GetY(lParam)/8)*32)] = MiniMapClass::miniMapEditor->curtile+(MiniMapClass::miniMapEditor->Vert.value() ? 0x800:0)+(MiniMapClass::miniMapEditor->Horz.value() ? 0x400:0);
		MiniMapClass::miniMapEditor->DrawMap(MiniMapClass::miniMapEditor->Map);
		drawmm=1;
		break;
	case WM_RBUTTONDOWN:
 	    RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomX=(GetX(lParam)/8);
		RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomY=(GetY(lParam)/8);

	InvalidateRect(MiniMapClass::miniMapEditor->hwndMiniMap,0,1);
		break;
		
		
		
	}
	
    return DefWindowProc(hWnd, message, wParam, lParam);
	
}
int MiniMapClass::DrawTileset(Image *tileset, int pal){
	int i =0;	
	int max=  ((currentRomType) ? 512:384);
	tileset->Clear();
	for(i= 0;i<max;i++){
		tileset->Draw(ts,(i % 16)*8,(i/16)*8,(pal*0x1000)+i);
	}
	InvalidateRect(MiniMapClass::miniMapEditor->hwndMiniTileset,0,1);
	return 0;
}

int MiniMapClass::DecompressMap(int area){
	unsigned char* DecompBuf = new unsigned char[32192];
	memset(DecompBuf, 0, 32192);
	unsigned char*  compBuffer = new unsigned char[64691];

	
    memset(compBuffer,0,64691);
	if (RawMap != NULL || RawMap != 0)
	{
		delete[] RawMap;
		RawMap = NULL;
	}
	if(GBA.ROM){
        if(currentRomType==0){
		 MemFile::currentFile->seek(MapList[area]-0x8000000);
		}else if(currentRomType==1){
		 MemFile::currentFile->seek(MapList[area]-0x8000000);
		}
		MemFile::currentFile->fread(compBuffer,1,64691,GBA.ROM);
		DSize=GBA.LZ77UnComp(compBuffer, DecompBuf);
	
		RawMap=new unsigned short[DSize/2 +1];
	  
		memcpy(RawMap,DecompBuf,DSize);
		
	}
	delete[] DecompBuf;
	delete[] compBuffer;
	return 0;
}
int MiniMapClass::DrawMap(Image* dstImage){
	unsigned short i =0;
	dstImage->Clear();
	for(i = 0; i<DSize/2;i++){
		dstImage->Draw(ts,(i%32)*8,(i/32)*8,RawMap[i]);
	}
	InvalidateRect(hwndMiniMap,0,1);
	return 0;
}


int MiniMapClass::Destroy(){
    delete[] RawMap;
	return 0;
}

int MiniMapClass::SaveMap(){
    unsigned char* DecompBuf = new unsigned char[32192];

    unsigned long size=0;
	unsigned long tsize=0;
	unsigned long offset=0;
	unsigned long moffset=0;

	if(currentRomType==0) moffset=0x7601EC;
	if(currentRomType==1) moffset=0x79BE5C;
	if(GBA.ROM){
		
		size = GBA.LZ77Comp(DSize, (unsigned char*)RawMap,sizeof(RawMap), DecompBuf);
		//Test size
	    offset=GBA.FindFreeSpace(size+30, 0xFF);
		MapList[cboMArea.GetListIndex()]=offset+0x8000000;
		MemFile::currentFile->seek(moffset);
		MemFile::currentFile->fwrite(MapList,4,0xB,GBA.ROM);
        MemFile::currentFile->seek(MapList[cboMArea.GetListIndex()]-0x8000000);
		MemFile::currentFile->fwrite(DecompBuf,1,size,GBA.ROM);
	
		
	}
	delete[] DecompBuf;
	return 0;
}
int DrawMapLoc(HDC hdc){
   RECT blah;
   int thisX=0;
   int thisY=0;
   int x=0;
   int y=0;
   int ThisArea=Combos[cArea].GetListIndex();
   int ThisMArea= MiniMapClass::miniMapEditor->cboMArea.GetListIndex();
   int ThisRoom=Combos[cRoom].GetListIndex();
                //Check to see if the areas are the same 
	                   //If they aren't do draw
   int RegDraw  =(ThisArea == ThisMArea ? 1:0);
   int DebugDraw=(ThisArea==6 && ThisMArea>=7  && ThisRoom > 0x61 ? 1:0);
   if(RegDraw||DebugDraw){
	 HBRUSH curbrush = CreateSolidBrush(RGB(255, 0, 255));
     blah.left   =RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomX*8;
     blah.right  =blah.left +((RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X+1)/16)*8;
	 blah.top    =RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomY*8;
	 blah.bottom =blah.top+((RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y)/10)*8;
     FrameRect(hdc, &blah,curbrush);
	 DeleteObject(curbrush);
   }
	return 0;
}






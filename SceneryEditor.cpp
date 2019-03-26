#include "MainHeader.h"
#include "MapManager.h"
#include "GBAGraphics.h"
#include "GlobalVars.h"
#include "MapUtils.h"
BOOL CALLBACK	SceneProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){
	
	
	char palstring[256];
	
	int i;
	
	switch (message) 
	{
	case WM_INITDIALOG:
		SceneType = -1;
		mpScene.sX=mpScene.sY=-100;
		SceneWnd  = hWnd;
		cboSBGsize.Init(GetDlgItem(hWnd,(int)MAKEINTRESOURCE(cboSSize)));
		cboSSPal.Init(GetDlgItem(hWnd,(int)MAKEINTRESOURCE(cboSPal)));
		cboSBGsize.Additem("256x256");
		cboSBGsize.Additem("512x256");
		cboSBGsize.Additem("256x512");
		cboSBGsize.SetListIndex(0);
		for(i = 0;i<16;i++){
			sprintf(palstring,"0x%X",i);
			cboSSPal.Additem(palstring);
		}
		mpScene.Width=mpScene.Height=1;
		mpScene.sX=mpScene.sY=0;
		cboSSPal.SetListIndex(6);
		InitSceneryWindow();
		InitTSWindow();
		DrawGraphicTiles();
		break;
		
	case WM_COMMAND:
		if(LOWORD(wParam) == cmdSEditForeground) {
			DrawThisScene(2);
			mpSTS.sX=mpSTS.sY=0;
		    InvalidateRect(SceneWnd,0,1); 
		}
		if(LOWORD(wParam)==	cmdBGi) {
			ShowWindow(GlobalVars::gblVars->BGi->me,1);
		return 0;
		}
			if(LOWORD(wParam) == cmdClearScene){
			switch(SceneType){
			case 0:
	        memset(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->TileBuf2D,0,RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->oSize*2);
			DrawThisScene(0);
				break;
			case 1:
				memset(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->TileBuf2D,0,RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->oSize*2);
			DrawThisScene(1);
				break;

			case 2:
			memset(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->TileBuf2D,0,RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->oSize*2);
			DrawThisScene(2);
				break;





			}

		}
		if(LOWORD(wParam) == cmdSEditBack){
			DrawThisScene(1);
			mpSTS.sX=mpSTS.sY=0;
			InvalidateRect(SceneWnd,0,1); 
		}
		if(LOWORD(wParam) == cmdSEditBG){	
			DrawThisScene(0);
			mpSTS.sX=mpSTS.sY=0;
			InvalidateRect(SceneWnd,0,1); 
		}
		if(LOWORD(wParam) == cboSPal) 			DrawGraphicTiles();
		if(LOWORD(wParam)==WM_DESTROY){
			
			ShowWindow(	TSScene,SW_HIDE);
			ShowWindow(hWnd,SW_HIDE);
		}
		if(LOWORD(wParam)==cmdSSave)SaveCurScene();
		break; 
		
	case WM_DESTROY:
		
		/// myBackBuffer.Destroy();
		break;
	}
	
	return 0;
}


int InitTSWindow(){
	// Fill in the window class structure with parameters 
	// that describe the m7ain window. 
	
	WNDCLASSEX blah6;
	memset(&blah6,0,sizeof(blah6));
	blah6.cbSize = sizeof(blah6);		   // size of structure 
	blah6.style = CS_HREDRAW | 
		CS_VREDRAW; 				   // redraw if size changes 
	blah6.lpfnWndProc = &TSWProc;	  // points to window procedure 
	blah6.cbClsExtra = NULL;				// no extra class memory 
	blah6.cbWndExtra =NULL; 			   // no extra window memory 
	blah6.hInstance = hGlobal;		   // handle to instance 
	blah6.hIcon = NULL; 			 // predefined app. icon 
	blah6.hCursor = NULL;				// predefined arrow 
	//blah6.hbrBackground = (HBRUSH)GetStockObject(15);				   // white background brush 
	blah6.lpszMenuName =  NULL;    // name of menu resource 
	blah6.lpszClassName = "cSceneTiles";  // name of window class 
	
	
	// Register the window class. 
	
	RegisterClassEx(&blah6); 
	
	TSScene = CreateWindowEx(NULL,"cSceneTiles",NULL, NULL,13,321,516,154,0,0,hGlobal,0);
	mpSTS.Width=mpSTS.Height=1;
	mpSTS.sX=mpSTS.sY=0;
	return 0;
}

int InitSceneryWindow(){
	// Fill in the window class structure with parameters 
	// that describe the main window. 
	int myerror=0;
	WNDCLASSEX penis6;
	memset(&penis6,0,sizeof(penis6));
	penis6.cbSize = sizeof(penis6);		   // size of structure 
	penis6.style = CS_HREDRAW | 
		CS_VREDRAW; 				   // redraw if size changes 
	penis6.lpfnWndProc = SWProc;	  // points to window procedure 
	penis6.cbClsExtra = NULL;				// no extra class memory 
	penis6.cbWndExtra =NULL; 			   // no extra window memory 
	penis6.hInstance = hGlobal;		   // handle to instance 
	penis6.hIcon = NULL; 			 // predefined app. icon 
	penis6.hCursor = NULL;				// predefined arrow 
	//	penis6.hbrBackground = (HBRUSH)GetStockObject(15);				   // white background brush 
	penis6.lpszMenuName =  NULL;    // name of menu resource 
	penis6.lpszClassName = "cPenis";  // name of window class 
	
	
	// Register the window class. 
	
	RegisterClassEx(&penis6); 
	myerror =GetLastError();
	SceneGraph= CreateWindowEx(NULL,"cPenis",NULL, WS_CHILD|WS_VISIBLE ,18,22,512,512,SceneWnd,0,hGlobal,0);
	return 0;
}



LRESULT CALLBACK SWProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	MapUtils* nMap = new MapUtils(RD1Engine::theGame->mainRoom->mapMgr);

	int x=0,y=0,add=0;
	switch(SceneType){
	case 0:
		x=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->X*8;
		y=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->Y*8;
		break;
	case 1:
		x=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->X*8;
		y=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->Y*8;
		break;
	case 2:
		
		x=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->X*8;
		y=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->Y*8;
		break;
	}
	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		SceneryPic.Blit(hdc, 0,0,SceneryPic.Width,SceneryPic.Height, 0,0);
		DrawTileRect(hdc,mpScene,8);
		// SceneryPic.Blit(hdc, 0,0,BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->X*8,BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->Y*8,	nHScroll[SceneHScroll]*8,nVScroll[12]*8);
		EndPaint(hWnd, &ps);
		break;
		
	case WM_LBUTTONDOWN:
		//x = GetX(lParam)/8;
		//y = GetY(lParam)/8;
		//add= (x < 31 ? 0:1024);
		//add+=(y < 31 ? 0:1024);
		switch(SceneType){
		case 0:
			
			//BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->TileBuf2D[(GetX(lParam)/8) + ((GetY(lParam)/8)*32)+add ] = SceneTile;
			nMap->EditThisLayer(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer),lParam,0,SceneTile,8);
			
			DrawThisScene(SceneType);
			break;
		case 1:
			//BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->TileBuf2D[(GetX(lParam)/8) + ((GetY(lParam)/8)*32)+add] = SceneTile;
			nMap->EditThisLayer(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer),lParam,0,SceneTile,8);
			DrawThisScene(SceneType);
			break;
		case 2:
			//BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->TileBuf2D[(GetX(lParam)/8) + ((GetY(lParam)/8)*32)+add ] = SceneTile;
			nMap->EditThisLayer(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround),lParam,0,SceneTile,8);
			DrawThisScene(SceneType);
			break;
		}
		break; 
		case WM_RBUTTONDOWN:
	    	SmP[0]=1;//Has been selected
			SmP[1]=1;//In use 
		    SceneryCopySource=1;
			mpScene.sX = (GetX(lParam)/8);// +MapHorizScroll->GetIndex())/16;
			mpScene.sY = (GetY(lParam)/8);//+ MapVertScroll->GetIndex());
			mpScene.cX = mpScene.sX;
			mpScene.cY = mpScene.sY;
			mpScene.Width = 1;
			mpScene.Height =1;
            if(mpScene.sX>31) add +=1024;
            SceneTile = (GetX(lParam)/8) + ((GetY(lParam)/8)*32)+add;
			break;
        case WM_RBUTTONUP:
	//		SmP[1] = 0;
		break;
		case WM_MOUSEMOVE:
          	 /*if(SmP[1] == 1){
						mpScene.nX = GetX(lParam)/8;
						mpScene.nY = GetY(lParam)/8;
						//Determine direction for length and width;
						if(mpScene.nX>mpScene.cX){//increase width;
							mpScene.Width+=1;
						}else if(mpScene.nX<mpScene.cX){
							mpSTS.Width-=1;
						}
						if(mpScene.nY>mpScene.cY){//increase width;
							mpScene.Height+=1;
						}else if(mpScene.nY<mpScene.cY){
							mpScene.Height-=1;
						}
						mpScene.cX = GetX(lParam)/8;
						mpScene.cY = GetY(lParam)/8;
						if(mpScene.Height<1) mpScene.Height =1;
						if(mpScene.Width <1)  mpScene.Width = 1;
						if(mpScene.Width >8)  mpScene.Width = 8;
						if(mpScene.Height>8) mpScene.Height = 8;
					
		 }else{*/
			if(SceneType!=-1){
				if(mpScene.sX!=-100)mpScene.sX= GetX(lParam)/8;
		     	if(mpScene.sY!=-100)mpScene.sY= GetY(lParam)/8;
			}
			switch(SceneType){
		case 0:
			
			if(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->X<=mpScene.sX)mpScene.sX=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->X-1;
			if(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->Y<=mpScene.sY)mpScene.sY=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->Y-1;
			break;
		case 1:
			if(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->X<=mpScene.sX)mpScene.sX=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->X-1;
			if(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->Y<=mpScene.sY)mpScene.sY=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->Y-1;
			break;
		case 2:
			if(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->X<=mpScene.sX)mpScene.sX=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->X-1;
			if(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->Y<=mpScene.sY)mpScene.sY=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->Y-1;
			break;
		}
		 //}
			InvalidateRect(SceneGraph,0,1);
		    	if(wParam == MK_LBUTTON) SendMessage(hWnd, 0x201, wParam, lParam);
			break;
		
		
		break;
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
	//return 0;
}

LRESULT CALLBACK TSWProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	char stuff[256]={0};
	
	
	switch (message) 
	{
	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		SceneryTiles.Blit(hdc, 0,0,1024,256,0,0);
        DrawTileRect(hdc,mpSTS,8);
		EndPaint(hWnd, &ps);
		break;
		
	case WM_LBUTTONDOWN:
		SceneTile = (cboSSPal.GetListIndex() * 0x1000 )+(GetX(lParam)/8 ) + ((GetY(lParam)/8)* 64);
		sprintf(stuff, "Tile: %X",SceneTile&0x3FF);
		SetWindowText(GetDlgItem(SceneWnd ,lbltst),stuff);
		SceneryCopySource=0;
		SceneTile = SceneTile;
		break; 
	case WM_RBUTTONDOWN:
	if(LOWORD(wParam) == MK_RBUTTON){
		SceneryCopySource=0;
			ScP[0]=1;//Has been selected
			ScP[1]=1;//In use 
			mpSTS.sX = (GetX(lParam))/8;
			mpSTS.sY = (GetY(lParam)/8);
			mpSTS.cX = mpTileset.sX;
			mpSTS.cY = mpTileset.sY;
			mpSTS.Width = 1;
			mpSTS.Height =1;
            //MPToUse=1;
			//Tile = (GetX(lParam)/16 ) +  ((GetY(lParam)/16)+ nVScroll[sTileset])*16;
		}
	break;
    case WM_RBUTTONUP:
			ScP[1] = 0;
		break;
	case WM_MOUSEMOVE:
		 if(ScP[1] == 1){
						mpSTS.nX = GetX(lParam)/8;
						mpSTS.nY = GetY(lParam)/8;
						//Determine direction for length and width;
						if(mpSTS.nX>mpSTS.cX){//increase width;
							mpSTS.Width+=1;
						}else if(mpSTS.nX<mpSTS.cX){
							mpSTS.Width-=1;
						}
						if(mpSTS.nY>mpSTS.cY){//increase width;
							mpSTS.Height+=1;
						}else if(mpSTS.nY<mpSTS.cY){
							mpSTS.Height-=1;
						}
						mpSTS.cX = GetX(lParam)/8;
						mpSTS.cY = GetY(lParam)/8;
						if(mpSTS.Height<1) mpSTS.Height =1;
						if(mpSTS.Width<1)mpSTS.Width = 1;
						if(mpSTS.Width>8)mpSTS.Width = 8;
						if(mpSTS.Height>8)mpSTS.Height = 8;
					
		 }else{
							mpSTS.sX= GetX(lParam)/8;
							mpSTS.sY= GetY(lParam)/8;
		 }
			InvalidateRect(hWnd,0,1);
			break;
			


		
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}


int DrawThisScene(unsigned char Scene){
	SceneType = Scene;
	int difference;
	nMapBuffer ThisBackground = *(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer));
	int mX=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->X;
	int mY=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->Y;
	difference = (mY>mX? mX:mY);
	int HorScenRep = 0;
	int VerScenRep = 0;
	
	
	nMaxHScroll[13]=0 ;
	nMaxVScroll[12]=mY/8 ;
	// for(int i =0; i < mX*mY;i++){
	
	//	 GlobalVars::gblVars->imgMap->Draw(GlobalVars::gblVars->BGImage,(i%(mY))*8,(i/(mY))*8, BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->TileBuf2D[i]);
	//}
	int rex;
	int rey;
	int i , x, y, scrX, scrY;
	i=x=y=scrX=scrY=0;//
	switch(Scene){
	case 0:
		//Background Graphics
		SceneryPic.Create(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->X*8,RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->Y*8);
		SceneryPic.SetPalette(GBAGraphics::VRAM->PcPalMem);
		rex = (mX == 64 ? 2 :  1);
		rey = (mY == 64 ? 2 :  1);
		
		for(scrY = 0; scrY < rey; scrY++)
		{	
			for(scrX = 0; scrX < rex; scrX++)
			{
				for(y = 0; y < 32; ++y)
				{
					for(x = 0; x < 32; ++x)
					{
						SceneryPic.Draw(GlobalVars::gblVars->BGImage,x*8 + scrX*256	,y*8+ scrY*256 ,  RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->TileBuf2D[i]);
						
						i++;
					}
				}
			}
		}			
		
		break;
	case 1:
		SceneryPic.Create(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->X*8,RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->Y*8);
		SceneryPic.SetPalette(GBAGraphics::VRAM->PcPalMem);
		rex = (mX == 64 ? 2 :  1);
		rey = (mY == 64 ? 2 :  1);
		
		for(scrY = 0; scrY < rey; scrY++)
		{	
			for(scrX = 0; scrX < rex; scrX++)
			{
				for(y = 0; y < 32; ++y)
				{
					for(x = 0; x < 32; ++x)
					{
						SceneryPic.Draw(GlobalVars::gblVars->BGImage,x*8 + scrX*256	,y*8+ scrY*256 ,  RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->TileBuf2D[i]);
						
						i++;
					}
				}
			}
		}			
		
		break;
	case 2:
		//Foreground
		SceneryPic.Create(RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->X*8,RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->Y*8);
		SceneryPic.SetPalette(GBAGraphics::VRAM->PcPalMem);
		rex = (mX == 64 ? 2 :  1);
		rey = (mY == 64 ? 2 :  1);
		
		for(scrY = 0; scrY < rey; scrY++)
		{	
			for(scrX = 0; scrX < rex; scrX++)
			{
				for(y = 0; y < 32; ++y)
				{
					for(x = 0; x < 32; ++x)
					{
						SceneryPic.Draw(GlobalVars::gblVars->BGImage,x*8 + scrX*256	,y*8+ scrY*256 ,  RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->TileBuf2D[i]);
						
						i++;
					}
				}
			}
		}			
		
		break;
	}
	InvalidateRect(SceneGraph,0,1);

	
	
	return 0;
}

int DrawGraphicTiles(){
	int Pal= cboSSPal.GetListIndex();
	int X = 0;
	int Y = 0;
	int i = 0;
	SceneryTiles.Create(1024,256);
	SceneryTiles.SetPalette(GBAGraphics::VRAM->PcPalMem);
	//For now all we draw is BG ;O
	//for(Y = 0; Y<32;Y++){
	for(i = 0; i <1024;i++){ 
		X = (i%64)*8;
		Y = (i/64)*8;
		SceneryTiles.Draw(GlobalVars::gblVars->BGImage,X  ,Y , (Pal*0x1000) +	(i)  );
		
	}
	//}
	InvalidateRect(TSScene,0,1);
	return 0;
}


int SaveCurScene(){//0 for BG, 1 for BackLayer, 2 for Foreground
	unsigned long newsize=0;
	unsigned long oldsize=0;
	unsigned long offset=0;
	unsigned char  filedata[4]={0};
	char stroff[256]={0};
	unsigned char*  compBuffer = new unsigned char[64691];

	
	HWND   Layer=NULL;//Used for setting text
	nMapBuffer*  thisbuf;
	switch(SceneType){//Sets the pointers and addreGlobalVars::gblVars->SSEs and sizes;
	case 0:
		//BG
		thisbuf=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer);
        // offset = BaseGame::theGame->mainRoom->roomHeader.lBackgroundTSA-0x8000000;
		Layer= GetDlgItem(hwndHeader,txtBIO);
		break;
    case 1: 
		
        thisbuf=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer);
		Layer=GetDlgItem(hwndHeader,txtBO);
		//	offset =  BaseGame::theGame->mainRoom->roomHeader.lBackLayer    -0x8000000;
		break;
	case 2: 
		
        thisbuf=RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround);
		Layer=GetDlgItem(hwndHeader,txtFO);
		
		break;
	default:
		MessageBox(0, "INVALID","INVALID", MB_OK);
		throw "Invalid";
		
	}
    oldsize = (thisbuf->oSize);
	memset(compBuffer,0,64691);
    newsize=GBA.LZ77Comp(oldsize, (unsigned char*)thisbuf->TileBuf2D, 0x10000, compBuffer);
	offset = GBA.FindFreeSpace(newsize+0x40,0xFF);
	while (offset % 4) offset++;//Align
    //WriteDataToROM(compBuffer, newsize, offset);
	MemFile::currentFile->seek(offset+4);
	MemFile::currentFile->fwrite(compBuffer,1,newsize);
   
   
   /*MemFile::currentFile->seek(offset);
	
    filedata [0] = 0x10;
    filedata [1] = (newsize & 0xff);
    filedata [2] = ((newsize & 0xff00) >> 8);
    filedata [3] = ((newsize & 0xff0000) >> 16);
	fwrite(filedata,1,4);*/
	//fputc(thisbuf->size);
	
    sprintf(stroff, "%X", offset);	 
    SetWindowText(Layer, stroff);
   

	SaveHeader(0);
	GBA.Reopen();
	DrawThisScene(SceneType);
	delete[] compBuffer;
	return 0;
}

int WriteDataToROM(unsigned char* datastream, unsigned long size,unsigned long offset){
    MemFile::currentFile->seek(offset);
	MemFile::currentFile->fwrite(datastream,1,size);
	
	
	
	return 0;
}
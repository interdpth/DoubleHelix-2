#include "cClipboard.h"
#include "MapManager.h"
#include "resource.h"
#include "GlobalVars.h"
#include "ResourceAndDefines.h"
#include "GBAGraphics.h"
cClipboard::cClipboard() {
}
cClipboard::~cClipboard() {

}
LRESULT CALLBACK CBImageProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	char buffer[256] = { 0 };
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

	//	ClipBoard.cbi[ClipBoard.curclip].Blit(hdc, 0, 0, 256, 256, 0, 0);
	//	ClipBoard.CheckforBlanks(hdc);
		// SceneryPic.Blit(hdc, 0,0,BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->X*8,BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->Y*8,	nHScroll[SceneHScroll]*8,nVScroll[12]*8);
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
	//	SetTiles(lParam);
		break;
	case WM_MOUSEMOVE:
	//	sprintf(buffer, "Tile info: %d", (GetX(lParam) / 16) + (GetY(lParam) / 16) * 16);
	/*	SetWindowText(GetDlgItem(ClipBoard.me, fraInfo), buffer);
		sprintf(buffer, "Foreground: %X", ClipBoard.thisclipboard[ClipBoard.curclip][(GetX(lParam) / 16) + ((GetY(lParam) / 16) * 16)][0]);
		SetWindowText(GetDlgItem(ClipBoard.me, lblForeground), buffer);
		sprintf(buffer, "Level: %X", ClipBoard.thisclipboard[ClipBoard.curclip][(GetX(lParam) / 16) + ((GetY(lParam) / 16) * 16)][1]);
		SetWindowText(GetDlgItem(ClipBoard.me, lblLevel), buffer);
		sprintf(buffer, "BackLayer: %X", ClipBoard.thisclipboard[ClipBoard.curclip][(GetX(lParam) / 16) + ((GetY(lParam) / 16) * 16)][2]);
		SetWindowText(GetDlgItem(ClipBoard.me, lblBackLayer), buffer);
		sprintf(buffer, "Clipdata: %X", ClipBoard.thisclipboard[ClipBoard.curclip][(GetX(lParam) / 16) + ((GetY(lParam) / 16) * 16)][3]);
		SetWindowText(GetDlgItem(ClipBoard.me, lblClipdata), buffer);*/
		break;



	}

	return DefWindowProc(hWnd, message, wParam, lParam);
	//return 0;
}


int cClipboard::Create(HINSTANCE globalinstance) {
	char blah[256] = { 0 };
	int i = 0;
	cboCurClip.Init(GetDlgItem(me, cboCBNo));
	for (i = 0; i<16; i++) {
		sprintf(blah, "%d", i);
		cboCurClip.Additem(blah);
		cbi[i].Create(256, 256);
	}
	cboCurClip.SetListIndex(0);

	CreatePic(globalinstance, CBImageProc);
	Erase();
	return 0;
}

int cClipboard::Erase() {
	int i = 0, j = 0;

	for (i = 0; i<16; i++)
		Erase(i, 0xFFFF);


	return 0;
}



int cClipboard::CheckforBlanks(HDC hdc) {
	int i = 0, x = 0, y = 0;
	for (i = 0; i<256; i++) {

		if ((signed)(thisclipboard[curclip][i][0] == 0xFFFF)
			&& (thisclipboard[curclip][i][1] == 0xFFFF)
			&& (thisclipboard[curclip][i][2] == 0xFFFF)
			&& (thisclipboard[curclip][i][3] == 0xFFFF)) {
			//if we are here then we draw the X
			//	for(x=0;x<16;x++){
			for (y = 0; y<16; y++) {
				SetPixel(hdc, ((i % 16) * 16) + (y), ((i / 16) * 16) + (y), RGBA(255, 0, 0,0));
				SetPixel(hdc, ((i % 16) * 16) + (16 - y), ((i / 16) * 16) + (y), RGBA(255, 0, 0,0));
			}
			//	}




		}
	}

	return 0;
}

int cClipboard::CreatePic(HINSTANCE mainInstance, WNDPROC imgproc ) {


	// Fill in the window class structure with parameters 
	// that describe the main window. 
	int myerror = 0;
	WNDCLASSEX penis6;
	memset(&penis6, 0, sizeof(penis6));
	penis6.cbSize = sizeof(penis6);		   // size of structure 
	penis6.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	penis6.lpfnWndProc = imgproc;// CBImageProc;	  // points to window procedure 
	penis6.cbClsExtra = NULL;				// no extra class memory 
	penis6.cbWndExtra = NULL; 			   // no extra window memory 
	penis6.hInstance = mainInstance;//hGlobal;		   // handle to instance 
	penis6.hIcon = NULL; 			 // predefined app. icon 
	penis6.hCursor = NULL;				// predefined arrow 
										//	penis6.hbrBackground = (HBRUSH)GetStockObject(15);				   // white background brush 
	penis6.lpszMenuName = NULL;    // name of menu resource 
	penis6.lpszClassName = "cBalls";  // name of window class 


									  // Register the window class. 

	RegisterClassEx(&penis6);
	myerror = GetLastError();
	cb = CreateWindowEx(NULL, "cBalls", NULL, WS_CHILD | WS_VISIBLE, 22, 57, 256, 256, me, 0, mainInstance, 0);
	return 0;
}



int cClipboard::DrawSlots() {
	//Determine layer to draw
	int base[3] = { 0 };
	int what[3] = { 0 };
	int i = 0;
	int j = 0;
	int thisX = 0, thisY = 0;
	cbi[curclip].Clear();
	cbi[curclip].SetPalette(GBAGraphics::VRAM->PcPalMem);
	for (j = 0; j<256; j++) {
		for (i = 0; i<3; i++) base[i] = thisclipboard[curclip][j][i];
		//0=2
		for (i = 2; i >= 0; i--) {
			if (base[i] == 0xFFFF) continue;
			thisX = ((j % 16));
			thisY = ((j / 16));
			//cbi[curclip].Draw(GlobalVars::gblVars->TileImage,((j%16)*16), ((j/16)) * 16, TSA.nTSA[(base[i])*4]);
			//	cbi[curclip].Draw(GlobalVars::gblVars->TileImage,((j%16)*16) + 8, ((j/16)) * 16, TSA.nTSA[((base[i])*4)+1]);
			//cbi[curclip].Draw(GlobalVars::gblVars->TileImage,((j%16)*16), ((j/16) * 16)+8, TSA.nTSA[((base[i])*4)+2]);
			//cbi[curclip].Draw(GlobalVars::gblVars->TileImage,((j%16)*16)+8, ((j/16) * 16)+8, TSA.nTSA[((base[i])*4)+3]);
			//TILE =(Map->TileBuf2D[thisX+(thisY * (Map->X))]);
			BitBlt(cbi[curclip].DC(), (thisX)* 16, (thisY)* 16, 16, 16, GlobalVars::gblVars->imgTileset->DC(), ((base[i]) % 16) * 16, ((base[i]) / 16) * 16, SRCCOPY);
			//	GlobalVars::gblVars->imgTileset->Blit(hdc, 0,0,coord.right - coord.left ,coord.bottom-coord.top,0,nVScroll[sTileset]*16);


		}
	}
	InvalidateRect(cb, 0, 1);
	return 0;
}


int cClipboard::CopyTiles(LPARAM lParam, int surface) {//Surface is where to place them 0 for room, 1 for ya know ;)
													   //Just do it in order
	//
	//MapManager* mm = BaseGame::theGame->mainRoom->mapMgr;
	//int baseX = (GetX(lParam) / 16) +MapHorizScroll->GetIndex();
	//int baseY = (GetY(lParam) / 16) + MapVertScroll->GetIndex();
	//int thisX = 0, thisY = 0;
	////Since whe have the base this should be rather easy.
	//int i = 0;//Tile counter

	//for (i = 0; i <256; i++) {//256 tiles per clipboard
	//	thisX = (i % 16);
	//	thisY = (i / 16);
	//	if (thisclipboard[curclip][i][0] != 0xFFFF) {
	//		mm->GetLayer(MapManager::ForeGround).TileBuf2D[(baseX + thisX) + (baseY + thisY)* mm->GetLayer(MapManager::ForeGround).X] = thisclipboard[curclip][i][0];
	//		mm->GetLayer(MapManager::ForeGround).Dirty = 1;
	//	}
	//	if (thisclipboard[curclip][i][1] != 0xFFFF) {
	//		mm->GetLayer(MapManager::LevelData).TileBuf2D[(baseX + thisX) + (baseY + thisY)* mm->GetLayer(MapManager::LevelData).X] = thisclipboard[curclip][i][1];
	//		mm->GetLayer(MapManager::LevelData).Dirty = 1;
	//	}
	//	if (thisclipboard[curclip][i][2] != 0xFFFF) {
	//		mm->GetLayer(MapManager::Backlayer).TileBuf2D[(baseX + thisX) + (baseY + thisY) * mm->GetLayer(MapManager::Backlayer).X] = thisclipboard[curclip][i][2];
	//		mm->GetLayer(MapManager::Backlayer).Dirty = 1;
	//	}

	//	if (thisclipboard[curclip][i][3] != 0xFFFF)
	//		mm->GetLayer(MapManager::Clipdata).TileBuf2D[(baseX + thisX) + (baseY + thisY) * mm->GetLayer(MapManager::Clipdata).X] = thisclipboard[curclip][i][3];


	//}

	return 0;
}


int cClipboard::Erase(int board, int slot) {
	int i = 0;

	//if (slot == 0xFFFF) {
	//	memset(&thisclipboard[board], 0xFFFF, 2048);
	//	cbi[board].Clear();
	//	cbi[board].SetPalette(GBAGraphics::VRAM->PcPalMem);
	//}
	//else {
	//	memset(&thisclipboard[board][slot], 0xFFFF, 4);
	//	cbi[board].Clear();
	//	cbi[board].SetPalette(GBAGraphics::VRAM->PcPalMem);
	//}
	InvalidateRect(cb, 0, 1);

	return 0;
}



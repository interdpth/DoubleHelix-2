#include "cTSAEditorClass.h"
#include "MainHeader.h"
#include "GBAGraphics.h"
#include "TilesetManager.h"
#include "BaseGame.h"
LRESULT CALLBACK BlockTilesetProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BlockPreviewProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
cTSAEditorClass::cTSAEditorClass() {

};
cTSAEditorClass::~cTSAEditorClass() {
	delete Tileset;
		
		 delete iPreview ;
		 delete thisTSA;
};

int cTSAEditorClass::Create() {
	int i = 0;
	HWND hrkwnd = NULL;
	HWND verwnd = NULL;
	HWND palwnd = NULL;
	char buf[256] = { 0 };
	CreateTilesetWind();
	CreatePreviewWind();

	//Load up controls
	curtsa = 0;
	for (i = 0; i<4; i++) {
		switch (i) {

		case 0:
			hrkwnd = GetDlgItem(tMain, chkHor);
			verwnd = GetDlgItem(tMain, vertchk);
			palwnd = GetDlgItem(tMain, cbopallettetsa);
			break;
		case 1:
			hrkwnd = GetDlgItem(tMain, chkHor2);
			verwnd = GetDlgItem(tMain, vertchk2);
			palwnd = GetDlgItem(tMain, cbopallettetsa2);
			break;
		case 2:
			hrkwnd = GetDlgItem(tMain, chkHor3);
			verwnd = GetDlgItem(tMain, vertchk3);
			palwnd = GetDlgItem(tMain, cbopallettetsa3);
			break;
		case 3:
			hrkwnd = GetDlgItem(tMain, chkHor4);
			verwnd = GetDlgItem(tMain, vertchk4);
			palwnd = GetDlgItem(tMain, cbopallettetsa4);
			break;
		}
		HorChk[i].SetCnt(hrkwnd);
		VerChk[i].SetCnt(verwnd);
		cboPals[i].Init(palwnd);


	}
	cboTSAPAL.Init(GetDlgItem(tMain, cboTP));
	cboTSAPAL.Clear();
	for (i = 0; i<4; i++) {
		for (int p = 0; p<0xF; p++) {
			sprintf(buf, "%X", p);
			cboPals[i].Additem(buf);
			if (!i) cboTSAPAL.Additem(buf);
		}
		cboPals[i].SetListIndex(0);
		if (!i)cboTSAPAL.Additem(buf);
	}

	Tileset = new Image();
	iPreview = new Image();
	thisTSA = new Image();
	Tileset->Zero();
	Tileset->Create(512, 1024);
	iPreview->Zero();
	iPreview->Create(32, 32);
	cboTSAPAL.SetListIndex(2);

	DrawThisTileset();

	curtsa = 0;
	LoadTSA();

	return 0;
}

int cTSAEditorClass::CreateTilesetWind() {
	WNDCLASSEX blar8;
	RECT t;
	HWND p = GetDlgItem(tMain, fraTTileset);
	memset(&blar8, 0, sizeof(blar8));
	blar8.cbSize = sizeof(blar8);		   // size of structure 
	blar8.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blar8.lpfnWndProc = &BlockTilesetProc;	  // points to window procedure 
	blar8.cbClsExtra = NULL;				// no extra class memory 
	blar8.cbWndExtra = NULL; 			   // no extra window memory 
	blar8.hInstance = hGlobal;		   // handle to instance 
	blar8.hIcon = NULL; 			 // predefined app. icon 
	blar8.hCursor = NULL;				// predefined arrow 
										//blar8.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH);				   // white background brush 
	blar8.lpszMenuName = NULL;    // name of menu resource 
	blar8.lpszClassName = "TSATilesetWind";  // name of window class 


											 // Register the window class. 
	RegisterClassEx(&blar8);
	GetWindowRect(p, &t);
	TilesetPic = CreateWindowEx(NULL, "TSATilesetWind", NULL, WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD, 0, 0, t.right - t.left, 670, p, 0, hGlobal, 0);

	ShowWindow(TilesetPic, SW_SHOW);
	return 0;
}

int cTSAEditorClass::CreatePreviewWind() {
	WNDCLASSEX ban9;

	//   HWND p=GetDlgItem(tMain,fraPreview);
	memset(&ban9, 0, sizeof(ban9));
	ban9.cbSize = sizeof(ban9);		   // size of structure 
	ban9.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	ban9.lpfnWndProc = &BlockPreviewProc;	  // points to window procedure 
	ban9.cbClsExtra = NULL;				// no extra class memory 
	ban9.cbWndExtra = NULL; 			   // no extra window memory 
	ban9.hInstance = hGlobal;		   // handle to instance 
	ban9.hIcon = NULL; 			 // predefined app. icon 
	ban9.hCursor = NULL;				// predefined arrow 
										//ban9.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH);				   // white background brush 
	ban9.lpszMenuName = NULL;    // name of menu resource 
	ban9.lpszClassName = "TSAPreviewWind";  // name of window class 


											// Register the window class. 
	RegisterClassEx(&ban9);

	Preview = CreateWindowEx(NULL, "TSAPreviewWind", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD, 175, 32, 32, 32, MyTSAEditor.tMain, 0, hGlobal, 0);


	ShowWindow(Preview, SW_SHOW);
	return 0;
}




int cTSAEditorClass::DrawThisTileset() {
	int i = 0;
	Tileset->SetPalette(GBAGraphics::VRAM->PcPalMem);
	Tileset->Clear();
	for (i = 0; i<1024; i++) {
		Tileset->Draw(*GlobalVars::gblVars->TileImage, (i % 16) * 8, (i / 16) * 8, (cboTSAPAL.GetListIndex() << 12) + i);
	}
	InvalidateRect(TilesetPic, 0, 1);
	return 0;
}




int cTSAEditorClass::LoadTSA() {
	int i = 0;
	if (!RD1Engine::theGame)
	{
		return -1;
	}
	for (i = 0; i<4; i++) {
		CurrentBlock[i].Pal = (RD1Engine::theGame->mgrTileset->TSA.nTSA[Tile * 4 + i] / 0x1000);
		CurrentBlock[i].HFlip =( RD1Engine::theGame->mgrTileset->TSA.nTSA[Tile * 4 + i] & 0x400);
		CurrentBlock[i].VFlip = (RD1Engine::theGame->mgrTileset->TSA.nTSA[Tile * 4 + i] & 0x800);
		CurrentBlock[i].Tile = (RD1Engine::theGame->mgrTileset->TSA.nTSA[Tile * 4 + i] & 0x3FF);
	}

	for (i = 0; i<4; i++) {


		HorChk[i].value(CurrentBlock[i].HFlip / 0x400);
		VerChk[i].value(CurrentBlock[i].VFlip / 0x800);
		cboPals[i].SetListIndex(CurrentBlock[i].Pal);



	}

	DrawPreview();
	return 0;
}

int cTSAEditorClass::SaveBlock() {
	//Will code after loading is done correctly.
	unsigned short block;
	unsigned long offset = RD1Engine::theGame->mgrTileset->RoomTilesets[RD1Engine::theGame->mainRoom->roomHeader->bTileset]->TSAMap - 0x8000000;
	RHeader* roomHeader = RD1Engine::theGame->mainRoom->roomHeader;
	for (int i = 0; i<4; i++) {
		block = CurrentBlock[i].Pal * 0x1000
			+ CurrentBlock[i].HFlip
			+ CurrentBlock[i].VFlip
			+ CurrentBlock[i].Tile;

		RD1Engine::theGame->mgrTileset->TSA.nTSA[Tile * 4 + i] = block;
	}
	//MemFile::currentFile->seek(offset);	
	//fwrite(&TSA.ID,sizeof(short),1);   
	//fwrite(&TSA.nTSA,sizeof(short),0x1080);
	unsigned long tileoffset = offset + 2 + Tile * 8;
	MemFile::currentFile->seek(tileoffset);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mgrTileset->TSA.nTSA[Tile * 4], 2, 4);
	fclose(GBA.ROM);
	GBA.ROM = fopen(GBA.FileLoc, "r+b");
	RD1Engine::theGame->mgrTileset->GetTileset(GlobalVars::gblVars->imgTileset, roomHeader->bTileset, roomHeader->lBg3);
	return 0;
}

int cTSAEditorClass::DrawPreview() {
	iPreview->SetPalette(GBAGraphics::VRAM->PcPalMem);
	iPreview->Clear();
	iPreview->Draw(*GlobalVars::gblVars->TileImage, 0, 0, CurrentBlock[0].Pal * 0x1000
		+ CurrentBlock[0].HFlip
		+ CurrentBlock[0].VFlip
		+ CurrentBlock[0].Tile);
	iPreview->Draw(*GlobalVars::gblVars->TileImage, 8, 0, CurrentBlock[1].Pal * 0x1000
		+ CurrentBlock[1].HFlip
		+ CurrentBlock[1].VFlip
		+ CurrentBlock[1].Tile);
	iPreview->Draw(*GlobalVars::gblVars->TileImage, 0, 8, CurrentBlock[2].Pal * 0x1000
		+ CurrentBlock[2].HFlip
		+ CurrentBlock[2].VFlip
		+ CurrentBlock[2].Tile);
	iPreview->Draw(*GlobalVars::gblVars->TileImage, 8, 8, CurrentBlock[3].Pal * 0x1000
		+ CurrentBlock[3].HFlip
		+ CurrentBlock[3].VFlip
		+ CurrentBlock[3].Tile);
	InvalidateRect(Preview, 0, 1);
	return 0;
}
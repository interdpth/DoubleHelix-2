#include "cSSE.h"
#include "cOAMManager.h"
LRESULT CALLBACK cSSETileProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK cSSEPalProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);

#include <GBAMethods.h>
extern GBAMethods GBA;
cSpriteSetEditor *cSpriteSetEditor::SpriteSet;
cSpriteSetEditor::cSpriteSetEditor()
{
	SpritePreview = new SpriteObject();
}


cSpriteSetEditor::~cSpriteSetEditor()
{
	delete SpritePreview;
}


int cSpriteSetEditor::CreatePalWindow(HINSTANCE myInstance, WNDPROC proc) {
	WNDCLASSEX garh;
	memset(&garh, 0, sizeof(garh));
	garh.cbSize = sizeof(garh);		   // size of structure 
	garh.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	garh.lpfnWndProc = proc; //&cSSEPalProc;	  // points to window procedure 
	garh.cbClsExtra = NULL;				// no extra class memory 
	garh.cbWndExtra = NULL; 			   // no extra window memory 
	garh.hInstance = myInstance;// hGlobal;		   // handle to instance 
	garh.hIcon = NULL; 			 // predefined app. icon 
	garh.hCursor = NULL;				// predefined arrow 
	garh.hbrBackground = (HBRUSH)GetStockObject(15);				   // white background brush 
	garh.lpszMenuName = NULL;    // name of menu resource 
	garh.lpszClassName = "SSEPal";  // name of window class 


									// Register the window class. 
	RegisterClassEx(&garh);


	//Create the Window



	PalView = CreateWindowEx(NULL, "SSEPal", NULL, WS_VISIBLE | WS_CHILD, 5, 11, 256, 128, GetDlgItem(me, fraPal), 0, myInstance, 0);


	return 0;
}

int cSpriteSetEditor::CreateTileWindow(HINSTANCE mainInstance, WNDPROC proc) {
	WNDCLASSEX blahf;
	memset(&blahf, 0, sizeof(blahf));
	blahf.cbSize = sizeof(blahf);		   // size of structure 
	blahf.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blahf.lpfnWndProc = proc;//&cSSESpriteProc;	  // points to window procedure 
	blahf.cbClsExtra = NULL;				// no extra class memory 
	blahf.cbWndExtra = NULL; 			   // no extra window memory 
	blahf.hInstance = mainInstance;		   // handle to instance 
	blahf.hIcon = NULL; 			 // predefined app. icon 
	blahf.hCursor = NULL;				// predefined arrow 
	blahf.hbrBackground = (HBRUSH)GetStockObject(15);				   // white background brush 
	blahf.lpszMenuName = NULL;    // name of menu resource 
	blahf.lpszClassName = "cSSEs";  // name of window class 


									// Register the window class. 
	RegisterClassEx(&blahf);


	//Create the Window



	SprTilesView = CreateWindowEx(NULL, "cSSEs", NULL, WS_VISIBLE | WS_CHILD, 5, 11, 256, 130, GetDlgItem(me, fraTiles), 0, mainInstance, 0);


	return 0;
}


int cSpriteSetEditor::Create(HINSTANCE mainInstance) {
	char stringBuffer[256] = { 0 };
	int i = 0;
	SpriteSets.Init(GetDlgItem(me, lstSprites));
	Population.Init(GetDlgItem(me, lstPopulation));
	cboDetail.Init(GetDlgItem(me, cboSlot));
	cboDetail.Clear();
	SpriteSets.Clear();
	Population.Clear();
	for (i = 0; i < 256; i++) {
		sprintf(stringBuffer, "%X", i);
		SpriteSets.Additem(stringBuffer);
		if (16 > i)Population.Additem(stringBuffer);
		if (8 > i)cboDetail.Additem(stringBuffer);
	}
	CreateTileWindow(mainInstance, &cSSETileProc);
	CreatePalWindow(mainInstance, &cSSEPalProc);
	SpriteSets.SetListIndex(0);
	return 0;
}





//Loads spriteset GFX and Pal into Ram
int cSpriteSetEditor::GetSetData(int TitleChoice, unsigned char SpriteSetSelection, cEntityManager* mgr)
{
	//SpriteSets.GetListIndex()
	char tehbuf[256] = { 0 };
	int i = 0;
	total = 0;
	GlobalVars::gblVars->SSE = true;
	GlobalVars::gblVars->ReadObjectDetailsFromROM = false;

	if (TitleChoice == 0)
	{
		SpriteSetData.spriteCount=	mgr->LoadSet(TitleChoice, &mgr->gfxinfo, mgr->palinfo, mgr->spriteset, SpriteSetSelection);
	}
	else if (TitleChoice == 1)
	{
		SpriteSetData.spriteCount=	mgr->MFLoadSet(TitleChoice, &mgr->gfxinfo, mgr->palinfo, mgr->spriteset, SpriteSetSelection);
	}
	mgr->LoadPal(mgr->palinfo, mgr->spriteset, SpriteSetData.pal);
	
	GlobalVars::gblVars->SSE = false;
	GlobalVars::gblVars->ReadObjectDetailsFromROM = true;
	Population.Clear();
	for (i = 0; i < SpriteSetData.spriteCount; i++) {
		sprintf(tehbuf, "%X", i);

		Population.Additem(tehbuf);

	}
	return 0;
}

int cSpriteSetEditor::GetSpritesPalAndTiles(bool romSwitch) {
	int i = 0;
	GlobalVars::gblVars->SSE = true;
	cEntityManager* mgr = RD1Engine::theGame->mainRoom->mgrEntities;
	memset(SpriteSetData.pal, 0, sizeof(SpriteSetData.pal));
	memset(GBAGraphics::VRAM->GBASprPal, 0, sizeof(GBAGraphics::VRAM->GBASprPal));
	mgr->LoadPal(mgr->palinfo, mgr->spriteset, SpriteSetData.pal);
	SpriteSetData.usedGFX = RD1Engine::theGame->mgrOAM->LoadSpriteToMem(romSwitch, _gbaMethods, &mgr->gfxinfo[0], mgr->spriteset, SpriteSetData.graphics, &SprGraphics);

	GlobalVars::gblVars->SSE = false;
	SpriteSet->vramImage.Create(1024, 512);
	SpriteSet->vramImage.SetPalette(SpriteSetData.pal);
	for (i = 512; i < 1024; i++) {
		SpriteSet->vramImage.Draw(SprGraphics, ((i - 512) % 32) * 8, ((i - 512) / 32) * 8, 0x8000 + i);
	}

	return 0;
}



int cSpriteSetEditor::SetInfo() {
	char buffer[2048] = { 0 };
	unsigned short math = 0;
	math = 0x4000 - SpriteSetData.usedGFX;
	sprintf(buffer, "Sprite Graphic Data:\nThis set has %X bytes free", math);
	SetWindowText(GetDlgItem(me, lblSpriteInfo), buffer);
	math = 8 - ((SpriteSetData.usedPAL / 0x10));
	sprintf(buffer, "Sprite Palette Data:\nThis set has %X palettes free", math);
	SetWindowText(GetDlgItem(me, lblPaletteInfo), buffer);

	return 0;

}


int cSpriteSetEditor::SetupPreview(SpriteObject* SprG, int TitleChoice) {

	if (SprG->id <= 0xf)
	{
		return -1;
	}
	//Sets up the preview control, will be called alot
	long GFXPnt = 0;
	long PalPnt = 0;

	long addybuf = 0;
	long PalPos = 0;
	long size = 0;

	int i = 0;
	int ii = 0;
	unsigned char* compBuffer = new unsigned char[32687];
	unsigned char* decompbuf = new unsigned char[64691];


	InvalidateRect(me, 0, 1);
	if (!GlobalVars::gblVars->frameTables->FramesExist(SprG->id)) return 0;
	//memset(&SprG->PreRAM[0x4000], 0, 0x4000);


	unsigned short transferpal[256] = { 0 };

	int  X = 0;
	long off = 0;
	int x = 0;
	memset(GBAGraphics::VRAM->SprPal, 0, sizeof(GBAGraphics::VRAM->SprPal));


	PalPnt = GameConfiguration::mainCFG->GetDataContainer("SpritePal")->Value + (SprG->id - 0x10) * 4;
	SprG->palsize = RD1Engine::theGame->GetPalSize(SprG->id);

	MemFile::currentFile->seek(PalPnt);
	MemFile::currentFile->fread(&addybuf, 4, 1);
	MemFile::currentFile->seek(addybuf - 0x8000000);
	MemFile::currentFile->fread(&transferpal, 1, (SprG->palsize) * 2);

	memcpy(&GBAGraphics::VRAM->GBASprPal[128], &transferpal, SprG->palsize * 2);

	GBA.DecodePal(GBAGraphics::VRAM->GBASprPal, SprG->PreviewPal, 16, 0);

	for (i = 0; i < SprG->palsize; i++)SprG->PreviewPal[128 + i] = cSpriteSetEditor::SpriteSet->SpriteSetData.pal[(8 + SprG->details) * 16 + i];

	RD1Engine::theGame->titleInstance->GetGFX(SprG->id, &cSpriteSetEditor::SpriteSet->SpriteSetData.graphics[0x4000]);
	GlobalVars::gblVars->SSE = true;
	SprG->PreRAM = cSpriteSetEditor::SpriteSet->SpriteSetData.graphics;

	SprG->PreviewPal = cSpriteSetEditor::SpriteSet->SpriteSetData.pal;
	vramImage.SetPalette(SprG->PreviewPal);
	SprG->sprTileBuffer->Load(SprG->PreRAM, 1024);

	RD1Engine::theGame->mgrOAM->DecodeOAM(false, SprG, GlobalVars::gblVars->frameTables->OAMFrameTable[SprG->id].front());
	RD1Engine::theGame->mgrOAM->DrawPSprite(SprG);
	SprG->PreRAM = cSpriteSetEditor::SpriteSet->SpriteSetData.graphics;
	delete[] decompbuf;
	SprG->PreviewPal = cSpriteSetEditor::SpriteSet->SpriteSetData.pal;
	vramImage.SetPalette(SprG->PreviewPal);
	GlobalVars::gblVars->SSE = false;
	InvalidateRect(me, 0, 1);
	delete[] compBuffer;

	return 0;
}
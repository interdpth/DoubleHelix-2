#include "cOAMEdit.h"
#include "resource.h"
#include "GBAGraphics.h"
#include "OAM.h"
#include "Frames.h"
#include "cOAMManager.h"


//File only contains intera
cOAMEdit* cOAMEdit::OamEditor;

//
//Constructor
cOAMEdit::cOAMEdit() 
{
	printf("Initing");
	//Create();
	Tiles = new Image();
	Tiles->Create(512, 512);
}

cOAMEdit::~cOAMEdit() 
{
	delete Tiles;
	printf("Dying");
}

int cOAMEdit::UpdateCurrentSprite()
{
	Frame* disFrame = currentFrames->GetStaticFrame();
	int partNum = cboPartNo.GetListIndex();
	DecodedOAM *theOAM = &currentFrames->GetStaticFrame()->theSprite->OAM[partNum].deOAM;
	currentFrames->UpdateSprite(currentFrames->GetStaticFrame()->index, partNum, theOAM->TileNumber, theOAM->xCoord, theOAM->yCoord,cboShapes.GetListIndex(), cboSizes.GetListIndex(), HChk.value(), VChk.value(), cboPal.GetListIndex());
	RD1Engine::theGame->mgrOAM->DrawPSprite(cOAMEdit::OamEditor->currentFrames->GetStaticFrame()->theSprite);

	InvalidateRect(hwndSpritePreview, 0, 1);
	return InvalidateRect(hwndAnimationWindow, 0, 1);
}


int cOAMEdit::maxframe()
{
	return currentFrames->theFrames.size();
}



int cOAMEdit::UpdateSize() {
	int i = 0;
	cboSizes.Disable();
	cboSizes.Clear();
	char* sizesr[3][4] = {
		{ "8, 8\0", "16,16\0","32,32\0", "64,64\0" },
		{ "16,8\0", "32,8\0" ,"32,16\0", "64,32\0" },
		{ "8,16\0", "8,32\0" ,"16,32\0", "32,64\0" },
	};
	for (i = 0; i < 4; i++)
	{
		char* thisString =(char*)sizesr[cboShapes.GetListIndex()][ i];
		cboSizes.Additem(thisString);
	}
	cboSizes.SetListIndex(0);
	cboSizes.Enable();

	return 0;
}




int cOAMEdit::GetSpriteData( int id, int titleType) {//Fillsout the SprGBuf // lstSprite.GetListIndex();
	char offset[256] = { 0 };
	
	unsigned long theOffset =  GlobalVars::gblVars->frameTables->OAMFrameTable[id].front();
	if (theOffset == 0) return -1;
	GetFrames(theOffset, id, titleType);
	currentFrames->SetStaticFrame(0);
	currentFrames->GetStaticFrame()->theSprite->id = id;

	
	return 0;
}

int cOAMEdit::GetFrames(unsigned long offset,  int spriteID, int titleType) {
	
	int i = 0;
	if (currentFrames != NULL)
	{
		delete currentFrames; 
	}
	
	currentFrames = new FrameManager(&GBA, offset, spriteID, titleType);

	return 0;


}

//cboFrames.GetListIndex()
int cOAMEdit::SetFrames(int index) {
	char buffer[256] = { 0 };
	currentFrames->SetStaticFrame(index);
	Frame* thisFrame = &(currentFrames->GetStaticFrame())[0];
	sprintf(buffer, "%X", thisFrame->frameTimer);
	SetWindowText(GetDlgItem(_oamWindow, txtTimer), buffer);
	sprintf(buffer, "%X", thisFrame->frameOffset);
	SetWindowText(GetDlgItem(_oamWindow, txtFPOff), buffer);

	return 0;
}


int cOAMEdit::UpdatePartUI(bool updateSizes ) {
	
	int hflip=0;
	int vflip=0;

	int thispart = cboPartNo.GetListIndex();
	const unsigned char objSizes[3][4][2]=
	{
	{ { 8, 8}, {16,16}, {32,32}, {64,64} },
	{ {16, 8}, {32, 8}, {32,16}, {64,32} },
	{ { 8,16}, { 8,32}, {16,32}, {32,64} },
	};
	char labelText[256] = { 0 };
	if (thispart == -1)
	{
		thispart = 0;
	}
	Frame* thisOAM = currentFrames->GetStaticFrame();
	//Fill it out
	cboPal.SetListIndex(thisOAM->theSprite->OAM[thispart].deOAM.Palette);
	VChk.value(thisOAM->theSprite->OAM[thispart].deOAM.VerticalFlip);
	HChk.value(thisOAM->theSprite->OAM[thispart].deOAM.HorizontalFlip);
	cboShapes.SetListIndex(thisOAM->theSprite->OAM[thispart].deOAM.ObjShape);

	if (updateSizes)
	{
		UpdateSize();
		cboSizes.SetListIndex(thisOAM->theSprite->OAM[thispart].deOAM.ObjSize);
	}
	
	int    	sx = (thisOAM->theSprite->OAM[thispart].enOAM.OAM1 & 511);
	sx=thisOAM->theSprite->OAM[thispart].deOAM.xCoord = sx;
	int 	sy = (thisOAM->theSprite->OAM[thispart].enOAM.OAM0 & 255);
	sy=thisOAM->theSprite->OAM[thispart].deOAM.yCoord = sy;

	cboXPos.SetListIndex(sx);
	cboYPos.SetListIndex(sy);
	//if (sy & 0x80)
	//sy = sy - 256; //check for the negative
	//if (sx &  0x100)
	//sx = sx - 511; //same here

	//sx = sx + 16;
	//sy = sy + 16;
	//Set up the basic tile

	thisOAM->theSprite->OAM[thispart].resolvedX = sx;

	thisOAM->theSprite->OAM[thispart].resolvedX = sy;


	
	sprintf(labelText, "Tile: %X", thisOAM->theSprite->OAM[thispart].deOAM.TileNumber);
	SetWindowText(GetDlgItem(_oamWindow, lblTileNumber), labelText);
	cboPal.SetListIndex( ((thisOAM->theSprite->OAM[thispart].enOAM.OAM2&0xF000))/0x1000);

	HChk.value(0);
	if(thisOAM->theSprite->OAM[thispart].enOAM.OAM1 & 0x1000) //XFLIP
	HChk.value(1);

	VChk.value(0);
	if(thisOAM->theSprite->OAM[thispart].enOAM.OAM1 & 0x2000 ) //YFLIP
	VChk.value(1);
	int width = thisOAM->theSprite->OAM[thispart].enOAM.OAM0 >> 14;
	int height = thisOAM->theSprite->OAM[thispart].enOAM.OAM1 >> 14;
	hflip = ( HChk.value() ? (objSizes[width][height][0])-1:0);
	vflip = ( VChk.value() ? (objSizes[width][height][1])-1:0);
	rPart.left   = ((sx/8)^(hflip/8))*8;
	rPart.top    = ((sy/8)^(vflip/8))*8;
	rPart.right  = ((rPart.left) +(objSizes[width][height][0]));
	rPart.bottom = ((rPart.top ) + (objSizes[width][height][1]));
	cboBgPriority.SetListIndex(thisOAM->theSprite->OAM[thispart].deOAM.priority);
	sprintf(labelText, "OAM0: %X", thisOAM->theSprite->OAM[thispart].enOAM.OAM1);
	SetWindowText(GetDlgItem(_oamWindow, lblOAM0), labelText);
	sprintf(labelText, "OAM1: %X", thisOAM->theSprite->OAM[thispart].enOAM.OAM1);
	SetWindowText(GetDlgItem(_oamWindow, lblOAM1), labelText);
	sprintf(labelText, "OAM2: %X", thisOAM->theSprite->OAM[thispart].enOAM.OAM1);
	SetWindowText(GetDlgItem(_oamWindow, lblOAM2), labelText);


	//SetWindowText(GetDlgItem(me, txtGFX2), )
	return 0;
}
// &cOAMEdit::OamEditor->currentFrames->GetStaticFrame()
int cOAMEdit::LoadTiles(Image* tileImage, Frame* targetFrame)
{
	SprGBuf*currentSprite = targetFrame->theSprite;
	currentSprite->Tiles->Load(currentSprite->PreRAM, 1023);
	if (GlobalVars::gblVars->TileImage != NULL) {
		tileImage->SetPalette(currentSprite->PreviewPal);
		tileImage->Clear();
		for (int i = 0; i < 1024; i++) {
			tileImage->Draw(*currentSprite->Tiles, ((i) % 32) * 8, ((i) / 32) * 8, 0x8000 + i);
		}
	}
	return 0;
}

 int cOAMEdit::SetupPreview(  int TitleChoice, Frame* targetFrame) {
	/*if (targetFrame->frameInited)
	{
		return 0;
	}*/
	long GFXPnt = 0;
	long PalPnt = 0;
	unsigned char*  compBuffer = new unsigned char[64691];
	long addybuf = 0;
	long size = 0;
	SprGBuf*currentSprite = targetFrame->theSprite;
	int i = 0;
	int ii = 0;
	char blah[256] = { 0 };
	unsigned char *decompbuf = new unsigned char[32687];
	memset(decompbuf, 0, 32687);
	memset(compBuffer, 0, 64691);
//	currentSprite->PreviewSprite.ClearLayers();
	
	if ( GlobalVars::gblVars->frameTables->OAMFrameTable[currentSprite->id].front() == 0) return 0;

		CompHeader thiscompheader;
		GFXPnt = GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (currentSprite->id - 0x10) * 4;
		PalPnt = GameConfiguration::mainCFG->GetDataContainer("SpritePal")->Value + (currentSprite->id - 0x10) * 4;
		currentSprite->palsize = RD1Engine::theGame->GetPalSize(currentSprite->id);

	if (!targetFrame->theSprite->selfInitPal)
	{
		targetFrame->theSprite->selfInitPal = true;
	
		unsigned short transferpal[256] = { 0 };

		int  X = 0;
		long off = 0;
		int x = 0;
		memset(GBAGraphics::VRAM->SprPal, 0, sizeof(GBAGraphics::VRAM->SprPal));
		

		MemFile::currentFile->seek(PalPnt);
		MemFile::currentFile->fread(&addybuf, 4, 1);
		MemFile::currentFile->seek(addybuf - 0x8000000);
		//MemFile::currentFile->fread(&transferpal, 1, (paltransfer[x].Size)*2);
		memset(GBAGraphics::VRAM->GBASprPal, 0, 0x200);
		MemFile::currentFile->fread(&transferpal, 1, currentSprite->palsize * 2);
		//	if(paltransfer[x][1] == 0) continue;
		memcpy(&GBAGraphics::VRAM->GBASprPal[128], &transferpal, currentSprite->palsize * 2);

		GBA.DecodePal(GBAGraphics::VRAM->GBASprPal, currentSprite->PreviewPal, 16, 0);

	}

	if (!targetFrame->theSprite->selfInitGFX) {
	

		switch (TitleChoice) {
		case 0:
			MemFile::currentFile->seek(GFXPnt);
			MemFile::currentFile->fread(&addybuf, 4, 1);
			MemFile::currentFile->seek(addybuf - 0x8000000);
			MemFile::currentFile->fread(compBuffer, 1, sizeof(compBuffer));
			currentSprite->graphicsize = GBA.LZ77UnComp(compBuffer, decompbuf);
			memcpy(&currentSprite->PreRAM[0x4000], &decompbuf, currentSprite->graphicsize);
			break;
		case 1:
			currentSprite->graphicsize = RD1Engine::theGame->mgrOAM->MFSprSize[(currentSprite->id - 0x10) << 1];
			MemFile::currentFile->seek(GFXPnt);
			MemFile::currentFile->fread(&addybuf, 4, 1);
			MemFile::currentFile->seek(addybuf - 0x8000000);
			MemFile::currentFile->fread(&currentSprite->PreRAM[0x4000], 1, currentSprite->graphicsize);
			break;
		}


		
	}
	bool OAMED = true;
	if (targetFrame->frameOffset < 0x8000000) return -1;
	//if (targetFrame->frameOffset != 0x8BADBEEF)
	//{
	//	RD1Engine::theGame->mgrOAM->DecodeOAM(ROM, OAMED, currentSprite, targetFrame->frameOffset - 0x8000000);
	//}
	//RD1Engine::theGame->mgrOAM->DrawPSprite(currentSprite);
	//
	OAMED = false;
	delete[] decompbuf;
	delete[] compBuffer;
	return 0;
}




int cOAMEdit::DrawSelObj(HDC hdc) {

	//    RECT myrect;
	HBRUSH curbrush = CreateSolidBrush(RGB(255, 0, 0));

	//FrameRect(hdc, &rPart, curbrush);
	DeleteObject(curbrush);
	return 0;
}


///
///Inits form dialog
int cOAMEdit::Create() {
	int i = 0;
	char sillystring[16] = { 0 };
	cboFrameTable.Init(GetDlgItem(_oamWindow, cboFrameTableCombo));
	cboXPos.Init(GetDlgItem(_oamWindow, cboX));
	cboYPos.Init(GetDlgItem(_oamWindow, cboY));
	cboFrames.Init(GetDlgItem(_oamWindow, cboFrameIndex));
	cboPartNo.Init(GetDlgItem(_oamWindow, cboParts));
	cboShapes.Init(GetDlgItem(_oamWindow, cboShape));
	cboSizes.Init(GetDlgItem(_oamWindow, cboSize));//Changes based on cboShape
	lstSprite.Init(GetDlgItem(_oamWindow, lstSprites));
	cboBgPriority.Init(GetDlgItem(_oamWindow, cboPriority));
	VChk.SetCnt(GetDlgItem(_oamWindow, chkVert));
	HChk.SetCnt(GetDlgItem(_oamWindow, chkHorz));
	cboShapes.Additem("Square");
	cboShapes.Additem("Horizontal");
	cboShapes.Additem("Vertical");
	cboShapes.SetListIndex(0);
	cboPal.Init(GetDlgItem(_oamWindow, cboBPal));
	UpdateSize();
	
	cboFrames.Clear();
	for (i = 0; i<0xFF; i++) 
	{
		sprintf(sillystring, "%X", i);
		if (i<16)cboPal.Additem(sillystring);
		lstSprite.Additem(sillystring);
		sprintf(sillystring, "%d", i);
		if (i < 4)
		{
			cboBgPriority.Additem(sillystring);
		}
		cboYPos.Additem(sillystring);
	}

	for (i = 0; i<511; i++) {
		sprintf(sillystring, "%d", i);
		cboXPos.Additem(sillystring);
	}
	cboXPos.SetListIndex(0);
	cboYPos.SetListIndex(0);
	CreatePalHWND();
	CreateTileHWND();
	CreateSpriteHWND();
	CreateSpriteAnimationHWND();
	CreatePartHWND();
	return 0;
}


int cOAMEdit::ExportPal()
{
	MessageBox(_oamWindow, "Not Implemented", "ExportPal is not implemented", MB_OK); return -1;
}
int cOAMEdit::UpdateTiles() {
	MessageBox(_oamWindow, "Not Implemented", "UpdateTiles is not implemented", MB_OK); return -1;
}
int cOAMEdit::UpdatePal() {
	MessageBox(_oamWindow, "Not Implemented", "UpdatePal is not implemented", MB_OK); return -1;
}
int cOAMEdit::ExportGraphics() {
	MessageBox(_oamWindow, "Not Implemented", "ExportGraphics is not implemented", MB_OK); return -1;
}
int cOAMEdit::ImportGraphics() {
	MessageBox(_oamWindow, "Not Implemented", "ImportGraphics is not implemented", MB_OK); return -1;
}

void cOAMEdit::SaveFrames() {
	MessageBox(_oamWindow, "Not Implemented", "SaveFrames is not implemented", MB_OK); 
}
void cOAMEdit::SaveFrameData() {
	MessageBox(_oamWindow, "Not Implemented", "Save Frame Data is not implemented", MB_OK); 
}
unsigned long cOAMEdit::GetNewOffset(int size) {
	MessageBox(_oamWindow, "Not Implemented", "Get New Offset is not implemented", MB_OK); return -1;
}
unsigned long cOAMEdit::Save(void* variableAddress) {
	return Save(SaveOptions::OFFSET, (char*) variableAddress);
}


unsigned long cOAMEdit::Save(SaveOptions savetype, char* variableThing) {
	
	Frame* tmpFrame = NULL;
	unsigned long* addr = (unsigned long*)variableThing;
	for (int thisCounter = 0; thisCounter <currentFrames->theFrames.size(); thisCounter++)
	{

		tmpFrame = currentFrames->theFrames[thisCounter];
		if (tmpFrame->frameOffset == 0x8BADBEEF)
		{
			int size = 2 + tmpFrame->theSprite->maxparts * 3 + 16;
			tmpFrame->frameOffset = GBA.FindFreeSpace(size, 0xFF);
		}
		RD1Engine::theGame->mgrOAM->SaveSprite(savetype, tmpFrame->theSprite, tmpFrame->frameOffset);
		char wndstr[256] = { 0 };

		
		
		
	}

	if (savetype == SaveOptions::OFFSET)
	{
		if (*addr == 0x8BADBEEF)
		{
			*addr = GBA.FindFreeSpace(currentFrames->theFrames.size() * 2 + 8, 0xFF);
		}
		currentFrames->Save(savetype, (void*)GBA.ROM, *addr);
	}

	return *addr;
}

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
	tileImage = new Image();
	tileImage->Destroy();
	tileImage->Create(512, 256);

	currentEditing = EditType::SPRITES;

	hwndAnimationWindow = 0;
	hwndSpritePreview = 0;
	hwndTiles = 0;
	hwndPal = 0;
	_oamWindow = 0;


	memset(&rPart, 0, sizeof(RECT));

}

cOAMEdit::~cOAMEdit()
{
	if(tileImage) delete tileImage;
	printf("Dying");
}

int cOAMEdit::UpdateCurrentSprite()
{
	Frame* disFrame = currentFrames->GetStaticFrame();
	int partNum = cboPartNo.GetListIndex();
	DecodedOAM *theOAM = &currentFrames->GetStaticFrame()->theSprite->OAM[partNum].deOAM;
	currentFrames->UpdateSprite(currentFrames->GetStaticFrame()->index, partNum, theOAM->TileNumber, theOAM->xCoord, theOAM->yCoord, cboShapes.GetListIndex(), cboSizes.GetListIndex(), HChk.value(), VChk.value(), cboPal.GetListIndex());
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
	for (i = 0; i <3; i++)
	{
		char* thisString = (char*)sizesr[cboShapes.GetListIndex()][i];
		cboSizes.Additem(thisString);
	}
	cboSizes.SetListIndex(0);
	cboSizes.Enable();

	return 0;
}




int cOAMEdit::GetSpriteData(int id, int titleType) {//Fillsout the SprGBuf // lstSprite.GetListIndex();
	char offset[256] = { 0 };

	std::vector<unsigned long>* pointerLst = &GlobalVars::gblVars->frameTables->OAMFrameTable[id];
	if (pointerLst == NULL || pointerLst->size() == 0) return 0;
	unsigned long theOffset = pointerLst->front();


	if (theOffset == 0) return -1;
	GetFrames(theOffset, id, titleType);
	currentFrames->SetStaticFrame(0);
	currentFrames->GetStaticFrame()->theSprite->id = id;
	return 0;
}

int cOAMEdit::GetFrames(unsigned long offset, int spriteID, int titleType) {

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
	txtTimerv.SetText(buffer);
	sprintf(buffer, "%X", thisFrame->frameOffset);
	txtFrameOffset.SetText(buffer);

	return 0;
}


int cOAMEdit::UpdatePartUI(bool updateSizes) {

	int hflip = 0;
	int vflip = 0;
	
	int thispart = cboPartNo.GetListIndex();
	const unsigned char objSizes[3][4][2] =
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
	thisOAM->theSprite->IsEdited = true;
	//Fill it out
	OverAllOAM* oam = &thisOAM->theSprite->OAM[thispart];
	cboPal.SetListIndex(oam->deOAM.Palette);
	VChk.value(oam->deOAM.VerticalFlip);
	HChk.value(oam->deOAM.HorizontalFlip);
	cboShapes.SetListIndex(oam->deOAM.ObjShape);

	if (updateSizes)
	{
		UpdateSize();
		cboSizes.SetListIndex(oam->deOAM.ObjSize);
	}

	int    	sx = (oam->enOAM.OAM1 & 511);
	sx = oam->deOAM.xCoord = sx;
	int 	sy = (oam->enOAM.OAM0 & 255);
	sy = oam->deOAM.yCoord = sy;

	cboXPos.SetListIndex(sx);
	cboYPos.SetListIndex(sy);
	//if (sy & 0x80)
	//sy = sy - 256; //check for the negative
	//if (sx &  0x100)
	//sx = sx - 511; //same here

	//sx = sx + 16;
	//sy = sy + 16;
	//Set up the basic tile

	oam->resolvedX = sx;

	oam->resolvedX = sy;

	sprintf(labelText, "Tile: %X", oam->deOAM.TileNumber);
	SetWindowText(GetDlgItem(_oamWindow, lblTileNumber), labelText);
	cboPal.SetListIndex(((oam->enOAM.OAM2 & 0xF000)) / 0x1000);

	HChk.value(0);
	if (oam->enOAM.OAM1 & 0x1000) //XFLIP
		HChk.value(1);

	VChk.value(0);
	if (oam->enOAM.OAM1 & 0x2000) //YFLIP
		VChk.value(1);
	int width = oam->enOAM.OAM0 >> 14;
	int height = oam->enOAM.OAM1 >> 14;
	hflip = (HChk.value() ? (objSizes[width][height][0]) - 1 : 0);
	vflip = (VChk.value() ? (objSizes[width][height][1]) - 1 : 0);
	rPart.left = ((sx / 8) ^ (hflip / 8)) * 8;
	rPart.top = ((sy / 8) ^ (vflip / 8)) * 8;
	rPart.right = ((rPart.left) + (objSizes[width][height][0]));
	rPart.bottom = ((rPart.top) + (objSizes[width][height][1]));
	cboBgPriority.SetListIndex(thisOAM->theSprite->OAM[thispart].deOAM.priority);
	sprintf(labelText, "OAM0: %X", thisOAM->theSprite->OAM[thispart].enOAM.OAM1);
	SetWindowText(GetDlgItem(_oamWindow, lblOAM0), labelText);
	sprintf(labelText, "OAM1: %X", thisOAM->theSprite->OAM[thispart].enOAM.OAM1);
	SetWindowText(GetDlgItem(_oamWindow, lblOAM1), labelText);
	sprintf(labelText, "OAM2: %X", thisOAM->theSprite->OAM[thispart].enOAM.OAM1);
	SetWindowText(GetDlgItem(_oamWindow, lblOAM2), labelText);

	return 0;
}
// &cOAMEdit::OamEditor->currentFrames->GetStaticFrame()
int cOAMEdit::LoadTiles(Image* tileImage2, Frame* targetFrame)
{
	SpriteObject*currentSprite = targetFrame->theSprite;

	currentSprite->sprTileBuffer->Load(currentSprite->PreRAM, 1023);
	if (GlobalVars::gblVars->TileImage != NULL) {
		tileImage2->SetPalette(currentSprite->PreviewPal);
		tileImage2->Clear();
		for (int i = 0; i < 1024; i++) {
			tileImage2->Draw(*currentSprite->sprTileBuffer, ((i) % 32) * 8, ((i) / 32) * 8, 0x8000 + i);
		}
	}
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
int cOAMEdit::InitDlg() {
	int i = 0;
	char sillystring[16] = { 0 };
	cboFrameTable.Init(GetDlgItem(_oamWindow, cboFrameTableCombo));
	cboXPos.Init(GetDlgItem(_oamWindow, cboX));
	cboYPos.Init(GetDlgItem(_oamWindow, cboY));
	cboFrames.Init(GetDlgItem(_oamWindow, cboFrameIndex));
	cboPartNo.Init(GetDlgItem(_oamWindow, cboParts));
	cboShapes.Init(GetDlgItem(_oamWindow, cboShape));
	cboSizes.Init(GetDlgItem(_oamWindow, cboSize));//Changes based on cboShape
	lstSprite.Init(GetDlgItem(_oamWindow, cboSpriteChoice));
	cboBgPriority.Init(GetDlgItem(_oamWindow, cboPriority));
	VChk.SetCnt(GetDlgItem(_oamWindow, chkVert));
	HChk.SetCnt(GetDlgItem(_oamWindow, chkHorz));
	cboShapes.Additem("Square");
	cboShapes.Additem("Horizontal");
	cboShapes.Additem("Vertical");
	cboShapes.SetListIndex(0);
	cboPal.Init(GetDlgItem(_oamWindow, cboBPal));
	UpdateSize();
	txtFrameOffset.Init(GetDlgItem(_oamWindow, txtFPOff));
	txtTimerv.Init(GetDlgItem(_oamWindow, txtFrameTimer));

	txtGlobalGfx.Init(GetDlgItem(_oamWindow, txtCommonGfx));
	txtGlobalPalc.Init(GetDlgItem(_oamWindow, txtCommonPal));
	txtCurrentGfx.Init(GetDlgItem(_oamWindow, txtCCurGfx));
	txtCurrentPal.Init(GetDlgItem(_oamWindow, txtCurPal));




	cboFrames.Clear();
	lstSprite.Additem("CUSTOM");
	for (i = 0; i < 0xFF; i++)
	{
		sprintf(sillystring, "%X", i);
		if (i < 16)cboPal.Additem(sillystring);
		if (!(RD1Engine::theGame->currentRomType == 0 && i < 16))
		{

			lstSprite.Additem(sillystring);
		}
		sprintf(sillystring, "%d", i);
		if (i < 4)
		{
			cboBgPriority.Additem(sillystring);
		}
		cboYPos.Additem(sillystring);
	}

	for (i = 0; i < 511; i++) {
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
	lstSprite.SetListIndex(0x11);
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
	return Save(SaveOptions::OFFSET, (char*)variableAddress);
}


unsigned long cOAMEdit::Save(SaveOptions savetype, char* variableThing) {

	Frame* tmpFrame = NULL;
	unsigned long* addr = (unsigned long*)variableThing;
	for (int thisCounter = 0; thisCounter < currentFrames->theFrames.size(); thisCounter++)
	{

		tmpFrame = currentFrames->theFrames[thisCounter];
		if (tmpFrame->frameOffset == 0x8BADBEEF)
		{
			int size = 2 + tmpFrame->theSprite->maxparts * 3 + 16;
			tmpFrame->frameOffset = GBA.FindFreeSpace(size, 0xFF);
		}
		RD1Engine::theGame->mgrOAM->SaveSprite(savetype, tmpFrame->theSprite, tmpFrame->frameOffset);		
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

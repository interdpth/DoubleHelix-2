#include "resource.h"
#include "MainHeader.h"
#include "GlobalVars.h"
#include "cOAMEdit.h"
#include "cSSE.h"
#include "cOAMManager.h"
#include "SpriteObjectManager.h"
#include "FrameManager.h"
#include "Frames.h"

#define theTimer 22424242
sCombo* cboPal;
sCombo* cboFrames;
sCombo* cboPartNo;
sCombo* cboShapes;
sCombo* cboSizes;//Changes based on cboShape
sCombo* cboXPos;
sCombo* cboYPos;
sList*  lstSprite;
sChecks* VChk;
sChecks* HChk;
HWND*   hwndSpritePreview;
HWND*   hwndAnimationSpritePreview;
HWND*   hwndTiles;
HWND*   hwndPal;
HWND*   oamWindow;
SpriteObjectManager* sprMgr;
GlobalVars* globalVars;
cOAMManager* oamManager;
int animCounter = 0;
unsigned short currentTile = -1;
BackBuffer animBuffer;
LRESULT CALLBACK SpecialKeyDownX(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SpecialKeyDownY(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
HWND hwndXPosText;
WNDPROC g_pOldhwndXPosText;

sCombo* cboFrameTable;

HWND hwndYPosText;
WNDPROC g_pOldhwndYPosText;

unsigned long GetCurrentFrameTable()
{
	unsigned long offset = 0;
	char wndstr[256] = { 0 };
	GetWindowText(cboFrameTable->GetHwnd(), wndstr, 8);
	sscanf(wndstr, "%X", &offset);

	return offset;
}

unsigned long SetCurrentFrameTable(unsigned long theOffset)
{
	char offset[256] = { 0 };
	sprintf(offset, "%X", theOffset);
	SetWindowText(cboFrameTable->GetHwnd(), offset);
	return theOffset;
}



LRESULT CALLBACK SpecialKeyDownX(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_KEYDOWN:							// Here's what you're interested in! -- check to see if someone preGlobalVars::gblVars->SSEd a key.
		if (wParam & VK_RETURN)
		{
			int a = 1;
		}
		break;
	}

	return CallWindowProc(g_pOldhwndXPosText, hWnd, message, wParam, lParam);
}
LRESULT CALLBACK SpecialKeyDownY(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:							// Here's what you're interested in! -- check to see if someone preGlobalVars::gblVars->SSEd a key.
		if (wParam & VK_RETURN)
		{
			int a = 1;
		}
		break;
	}
	return CallWindowProc(g_pOldhwndYPosText, hWnd, message, wParam, lParam);
}

void LoadPartsToUI()
{

}

void LoadPartsControl()
{
	Frame* tmpFrame = cOAMEdit::OamEditor->currentFrames->GetStaticFrame();
	char* strBuff = 0;
	strBuff = new char[256];

	cboPartNo->Clear();

	cboPartNo->Enable();
	for (int i = 0; i < tmpFrame->theSprite->maxparts; i++) {
		sprintf(strBuff, "%X", i);
		cboPartNo->Additem(strBuff);
	}

	delete[] strBuff;
	cboPartNo->SetListIndex(0);
}

void ReloadFramesControl()
{
	char wndstr[256] = { 0 };
	cboFrames->Clear();
	for (int i = 0; i < cOAMEdit::OamEditor->currentFrames->maxframe(); i++) {
		sprintf(wndstr, "%X", i);
		cboFrames->Additem(wndstr);
	}
	cboFrames->SetListIndex(0);
}

void UpdateStaticSprite()
{
	int partNum = cboPartNo->GetListIndex();
	Frame* thisFrame = cOAMEdit::OamEditor->currentFrames->GetStaticFrame();
	DecodedOAM *theOAM = &thisFrame->theSprite->OAM[partNum].deOAM;
	int size = cboSizes->GetListIndex();
	int shape = cboShapes->GetListIndex();
	cOAMEdit::OamEditor->currentFrames->UpdateSprite(
		
		cOAMEdit::OamEditor->currentFrames->GetStaticFrame()->index, 
		partNum, theOAM->TileNumber, theOAM->xCoord, theOAM->yCoord, 
		shape, 
		size, HChk->value(), VChk->value(), cboPal->GetListIndex());
	cOAMEdit::OamEditor->currentFrames->GetStaticFrame()->theSprite->PreviewSprite.GetFullImage()->Clear();
	oamManager->DrawPSprite(cOAMEdit::OamEditor->currentFrames->GetStaticFrame()->theSprite);
	InvalidateRect(*hwndSpritePreview, 0, 1);
}

#define lstSelchange 0x00010408
BOOL CALLBACK	OAMProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {
	cOAMEdit* oamEditor = cOAMEdit::OamEditor;
	int i = 0;
	char wndstr[256] = { 0 };
	unsigned int offset = 0;
	globalVars = GlobalVars::gblVars;
	oamManager = RD1Engine::theGame->mgrOAM;
	Frame* tmpFrame = NULL;
	int id = -1;

	if (oamEditor->currentFrames != NULL)
	{
		tmpFrame = oamEditor->currentFrames->GetStaticFrame();
	}

	signed char* charPnt;
	DecodedOAM* deOAM = NULL;
	int partIndex = 0;

	switch (message)
	{

	case WM_INITDIALOG:
		sprMgr = RD1Engine::theGame->mainRoom->mgrSpriteObjects;
		oamEditor->_oamWindow = hWnd;

		animBuffer.Create(2048, 2048);
		oamEditor->Create();
		cboPal = &oamEditor->cboPal;
		cboFrameTable = &oamEditor->cboFrameTable;
		cboFrames = &oamEditor->cboFrames;
		cboPartNo = &oamEditor->cboPartNo;
		cboXPos = &oamEditor->cboXPos;
		cboYPos = &oamEditor->cboYPos;
		cboShapes = &oamEditor->cboShapes;
		cboSizes = &oamEditor->cboSizes;//Changes based on cboShape
		lstSprite = &oamEditor->lstSprite;
		VChk = &oamEditor->VChk;
		HChk = &oamEditor->HChk;
		hwndSpritePreview = &oamEditor->hwndSpritePreview;
		hwndAnimationSpritePreview = &oamEditor->hwndAnimationWindow;
		hwndTiles = &oamEditor->hwndTiles;
		hwndPal = &oamEditor->hwndPal;
		oamWindow = &oamEditor->_oamWindow;
		hwndXPosText = GetWindow(cboXPos->GetHwnd(), GW_CHILD);
		g_pOldhwndXPosText = (WNDPROC)SetWindowLongPtr(hwndXPosText, GWLP_WNDPROC, (LONG_PTR)SpecialKeyDownX);
		hwndYPosText = GetWindow(cboYPos->GetHwnd(), GW_CHILD);
		g_pOldhwndYPosText = (WNDPROC)SetWindowLongPtr(hwndYPosText, GWLP_WNDPROC, (LONG_PTR)SpecialKeyDownY);
		SetTimer(hWnd, theTimer, 60, (TIMERPROC)NULL);

		break;
	case WM_TIMER:
		switch (wParam)
		{
		case theTimer:
			//init
			if (oamEditor->currentFrames != NULL) {
				Frame* animatedFrame = oamEditor->currentFrames->GetAnimatedFrame();
				if (animatedFrame != NULL)
				{
					if (oamEditor->currentFrames->Animate())
					{
						InvalidateRect(*hwndAnimationSpritePreview, 0, 1);
					}
				}
			}

			break;
		}
		break;

	case WM_COMMAND:

		switch (LOWORD(wParam)) {
		case cboX:
			if (HIWORD(wParam) == CBN_SELCHANGE || HIWORD(wParam) == VK_RETURN)
			{

				partIndex = cboPartNo->GetListIndex();
				deOAM = &tmpFrame->theSprite->OAM[partIndex].deOAM;
				deOAM->xCoord = (cboXPos->GetListIndex()) & (unsigned short)511;
				UpdateStaticSprite();
				oamEditor->UpdatePartUI();
			}
			else {

				break;
			}
			break;
		case cboY:
			if (HIWORD(wParam) == CBN_SELCHANGE || HIWORD(wParam) == VK_RETURN)
			{
				tmpFrame = oamEditor->currentFrames->GetStaticFrame();
				partIndex = cboPartNo->GetListIndex();
				deOAM = &tmpFrame->theSprite->OAM[partIndex].deOAM;
				deOAM->yCoord = (cboYPos->GetListIndex()) & (unsigned short)255;
				UpdateStaticSprite();
				oamEditor->UpdatePartUI();
			}
			else {
				break;
			}
			break;


		case lstSprites:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				if (currentRomType == 1)
				{
					if ((lstSprite->GetListIndex() < 0x10))
					{
						lstSprite->SetListIndex(0x10);
					}
				}

				id = lstSprite->GetListIndex();
				oamEditor->GetSpriteData(id, currentRomType);//lstSprite.GetListIndex());

				cboFrameTable->Clear();

				vector<unsigned long> thisTable = GlobalVars::gblVars->frameTables->OAMFrameTable[id];

				for (i = 0; i < thisTable.size(); i++)
				{
					sprintf(wndstr, "%X", thisTable[i]);
					cboFrameTable->Additem(wndstr);
				}

				cboFrameTable->SetListIndex(0);
				SendMessage(hWnd, WM_COMMAND, cmdGTO, 0);
			}
			break;
		case cboFrameTableCombo:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				SendMessage(hWnd, WM_COMMAND, cmdGTO, 0);
			}
			break;
		case cboFrameIndex:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				oamEditor->SetFrames(cboFrames->GetListIndex());
				globalVars->OAMED = true;
				oamEditor->currentFrames->SetStaticFrame(cboFrames->GetListIndex());
				//reload pointer
				tmpFrame = oamEditor->currentFrames->GetStaticFrame();
				if (GlobalVars::gblVars->frameTables->OAMFrameTable[tmpFrame->theSprite->id].front() != 0)
				{
					cOAMManager::SetupPreview(&GBA, currentRomType, tmpFrame);
				}
				
				oamEditor->LoadTiles(oamEditor->tileImage, tmpFrame);
				LoadPartsControl();
				cboPartNo->SetListIndex(0);
				oamEditor->UpdatePartUI();
				oamManager->DecodeOAM(globalVars->OAMED, tmpFrame->theSprite, tmpFrame->frameOffset - 0x8000000);
				globalVars->OAMED = false;
				oamManager->DrawPSprite(tmpFrame->theSprite);
				InvalidateRect(hWnd, 0, 1);
				InvalidateRect(*hwndSpritePreview, 0, 1);
			}
			break;
		case cmdSG:
			sprMgr->SaveGFX(currentRomType);
			break;
		case mnuSP:
			sprMgr->SavePal(currentRomType);
			break;
		case mnuIG:
			sprMgr->ImportGFX();
			break;
		case mnuEP:
			sprMgr->ExportPal();
			break;
		case mnuIP:
			sprMgr->ImportPal(currentRomType);

			break;
		case mnuEG:
			sprMgr->ExportGFX();
			break;
		case cmdGTO:
			offset = GetCurrentFrameTable();
			if (offset == 0x0cdcdcdc) break;
			id = oamEditor->currentFrames->GetStaticFrame()->theSprite->id;
			oamEditor->GetFrames(offset, id, currentRomType);
			oamEditor->currentFrames->SetStaticFrame(0);
			oamEditor->currentFrames->GetStaticFrame()->theSprite->id = id;

			ReloadFramesControl();

			oamEditor->SetFrames(cboFrames->GetListIndex());


			oamEditor->currentFrames->SetStaticFrame(cboFrames->GetListIndex());


			for (i = 0; i < oamEditor->currentFrames->maxframe(); i++) {
				tmpFrame = oamEditor->currentFrames->theFrames[i];


				globalVars->OAMED = true;
				if (GlobalVars::gblVars->frameTables->OAMFrameTable[tmpFrame->theSprite->id].front() != 0)
				{
					cOAMManager::SetupPreview(&GBA, currentRomType, tmpFrame);
				}
			
				oamEditor->LoadTiles(oamEditor->tileImage, tmpFrame);
				oamManager->DecodeOAM(globalVars->OAMED, tmpFrame->theSprite, tmpFrame->frameOffset - 0x8000000);
				globalVars->OAMED = false;
				oamManager->DrawPSprite(tmpFrame->theSprite);
				tmpFrame->frameInited = true;
				tmpFrame->animUpdated = true;
			}

			LoadPartsControl();
			cboPartNo->SetListIndex(0);


			globalVars->OAMED = false;
			oamEditor->UpdatePartUI();
			//			oamEditor->currentFrames->GetStaticFrame()->theSprite->PreviewSprite.RefreshImage();
			cboFrames->SetListIndex(0);
			SendMessage(hWnd, WM_COMMAND, lstSelchange, 0);
			InvalidateRect(*hwndSpritePreview, 0, 1);
			InvalidateRect(*hwndAnimationSpritePreview, 0, 1);
			InvalidateRect(*hwndPal, 0, 1);
			break;
		case cmdSave:
			offset = GetCurrentFrameTable();
			oamEditor->Save(SaveOptions::OFFSET, (char*)&offset);
			break;
		case cmdSaveToFree:
			offset = GetCurrentFrameTable();
			oamEditor->Save(SaveOptions::OFFSET, (char*)0x8BADBEEF);
			offset = GetCurrentFrameTable();
			SetCurrentFrameTable(offset);
			break;
		case btnCHeader:
			oamEditor->Save(SaveOptions::CHEADER, "");
			break;
		case cboParts:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				oamEditor->UpdatePartUI();
				currentTile = tmpFrame->theSprite->OAM[cboFrames->GetListIndex()].deOAM.TileNumber;
			}
			break;
		case cboBPal:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				UpdateStaticSprite();
			}
			break;


		case chkVert:
		case chkHorz:
			UpdateStaticSprite();
			oamEditor->UpdatePartUI(false);
			break;

		case cboSize:
		case cboShape:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				 
				UpdateStaticSprite();
				//oamEditor->UpdatePartUI(false);
			}
			break;
		case cmdDeleteFrame:
			cOAMEdit::OamEditor->currentFrames->DeleteFrame(cboFrames->GetListIndex());
			ReloadFramesControl();
			break;
		case btnNewFrame:
			cOAMEdit::OamEditor->currentFrames->AddFrame(NULL, lstSprite->GetListIndex());
			ReloadFramesControl();
			break;
		case btnCopyFrame:
			cOAMEdit::OamEditor->currentFrames->AddFrame(cOAMEdit::OamEditor->currentFrames->GetStaticFrame(), lstSprite->GetListIndex());
			ReloadFramesControl();
			break;

		case btnCopyPart:
			cOAMEdit::OamEditor->currentFrames->GetStaticFrame()->AddPart(&cOAMEdit::OamEditor->currentFrames->GetStaticFrame()->theSprite->OAM[cboPartNo->GetListIndex()].enOAM);
			LoadPartsControl();
			cboPartNo->SetListIndex(0);
			break;
		case cmdDeletePart:
			partIndex = cboPartNo->GetListIndex();

			oamEditor->currentFrames->GetStaticFrame()->DeletePart(partIndex);
			partIndex = partIndex == 0 ? 0 : partIndex - 1;
			cboPartNo->SetListIndex(partIndex);
			LoadPartsControl();

			break;
		case btnAddPartZ:
			cOAMEdit::OamEditor->currentFrames->GetStaticFrame()->AddPart(NULL);
			LoadPartsControl();
			cboPartNo->SetListIndex(0);

			break;
		case WM_DESTROY:
			ShowWindow(hWnd, SW_HIDE);
			break;
		}

		break;

	}

	return 0;
}

LRESULT CALLBACK OAMPalProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int buf = 0;
	int i = 0;
	int j = 0;

	Frame* theFrame=NULL;
	if (cOAMEdit::OamEditor->currentFrames != NULL)
	{
		theFrame = cOAMEdit::OamEditor->currentFrames->GetStaticFrame();
	}

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (cOAMEdit::OamEditor->currentFrames != NULL)
		{
			DrawPal(hdc, theFrame->theSprite->PreviewPal, 8);
			
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_RBUTTONDOWN:
		buf = (GetX(lParam) / 16) + ((GetY(lParam) / 16) * 16);
		sprintf(procstring, "Physical Pal Number:%d, Pal Number: %d\nCurrent Pal Slot: %d\nCurrent Pal Number:%d\nCurrent Palette Value is %X", (buf)+128, (buf), (buf % 16) + 1, cSSE::SpriteSet->SpriteSetData.pal[buf + 128], cOAMEdit::OamEditor->currentFrames->GetStaticFrame()->theSprite->PreviewPal[buf]);
		i = LoadInput(theFrame->theSprite->PreviewPal[buf]);
		if (i != -1) {

			theFrame->theSprite->PreviewPal[buf + 128] = RGBA((i / 0x10000), ((i & 0xFF00) / 0x100), (i & 0xFF),255);//(long)(i&0xFFFFFF);
			InvalidateRect(cSSE::SpriteSet->PalView, 0, 1);
			cOAMEdit::OamEditor->tileImage->SetPalette(theFrame->theSprite->PreviewPal);
			//			theFrame->theSprite->PreviewSprite.SetPalette(cSSE::SpriteSet->SpriteSetData.pal);

			for (i = 0; i < 1024; i++) {
				cOAMEdit::OamEditor->tileImage->Draw(*theFrame->theSprite->Tiles, ((i) % 32) * 8, ((i) / 32) * 8, 0x8000 + i);
			}

			oamManager->DrawPSprite(theFrame->theSprite);
			InvalidateRect(cOAMEdit::OamEditor->hwndSpritePreview, 0, 1);
			InvalidateRect(cOAMEdit::OamEditor->hwndTiles, 0, 1);
			InvalidateRect(cOAMEdit::OamEditor->hwndAnimationWindow, 0, 1);
			InvalidateRect(hWnd, 0, 1);
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
	//return 0;
}



void SetShapeSizeFromRect(RECT* theRect)
{
	const unsigned char objSizes[3][4][2] =
	{
		{ { 8, 8 },{ 16,16 },{ 32,32 },{ 64,64 } },
		{ { 16, 8 },{ 32, 8 },{ 32,16 },{ 64,32 } },
		{ { 8,16 },{ 8,32 },{ 16,32 },{ 32,64 } },
	};

	int height = theRect->bottom - theRect->top;
	int width = theRect->right - theRect->left;



	///This is really bad logic. 


	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			int thisWidth = objSizes[x][y][0];
			int thisHeight = objSizes[x][y][1];
			if (thisWidth == width && thisHeight == height)
			{
				cboShapes->SetListIndex(x);
				cboSizes->SetListIndex(y);
			}
		}
	}
}


void SetSizeShapeRect(RECT * dstRect)
{
	const unsigned char objSizes[3][4][2] =
	{
		{ { 8, 8 },{ 16,16 },{ 32,32 },{ 64,64 } },
		{ { 16, 8 },{ 32, 8 },{ 32,16 },{ 64,32 } },
		{ { 8,16 },{ 8,32 },{ 16,32 },{ 32,64 } },
	};
	dstRect->top = 0;
	dstRect->left = 0;
	dstRect->right = objSizes[cboShapes->GetListIndex()][cboSizes->GetListIndex()][0];
	dstRect->bottom = objSizes[cboShapes->GetListIndex()][cboSizes->GetListIndex()][1];
}







int movingX = 0;
int movingY = 0;

LRESULT CALLBACK OAMTileProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	int x = GetX(lParam);
	int y = GetY(lParam);

	HBRUSH          curbrush = CreateSolidBrush(RGB(255, 0, 0));
	RECT tmp;
	switch (message)
	{
	case WM_MOUSEMOVE:
		movingX = x;
		movingY = y;
		InvalidateRect(hWnd, 0, 1);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		cOAMEdit::OamEditor->tileImage->Blit(hdc, 0, 0, 256, 256, 0, 0);
		SetSizeShapeRect(&tmp);
		tmp.left = movingX;
		tmp.top = movingY;
		tmp.bottom += movingY;
		tmp.right += movingX;
		FrameRect(hdc, &tmp, curbrush);
		DeleteObject(curbrush);
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		currentTile = x / 8 + (y / 8) * 32;
		currentTile = currentTile;
		movingX = x;
		movingY = y;
		break;
	}
	DeleteObject(curbrush);
	return DefWindowProc(hWnd, message, wParam, lParam);

}

LRESULT CALLBACK OAMSpriteProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	Frame* curFrame = NULL;
	if (cOAMEdit::OamEditor->currentFrames != NULL)
	{
		curFrame = cOAMEdit::OamEditor->currentFrames->GetStaticFrame();
	}
	const unsigned char objSizes[3][4][2] =
	{
		{ { 8, 8 },{ 16,16 },{ 32,32 },{ 64,64 } },
		{ { 16, 8 },{ 32, 8 },{ 32,16 },{ 64,32 } },
		{ { 8,16 },{ 8,32 },{ 16,32 },{ 32,64 } },
	};
	int currentPart = 0;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (cOAMEdit::OamEditor->currentFrames != NULL) {

			curFrame->theSprite->PreviewSprite.Blit(hdc, 0, 0, 1024, 1024, 0, 0);
			cOAMEdit::OamEditor->DrawSelObj(hdc);
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		if (cOAMEdit::OamEditor->currentFrames != NULL) {
			int xVar = GetX(lParam);
			int yVar = GetY(lParam);

			if (currentTile != -1)
			{
				currentPart = cboPartNo->GetListIndex();
				curFrame->theSprite->OAM[currentPart].deOAM.TileNumber = currentTile;
			}

			int partWidth = objSizes[curFrame->theSprite->OAM[currentPart].deOAM.ObjShape][curFrame->theSprite->OAM[currentPart].deOAM.ObjSize][0];
			int partHeight = objSizes[curFrame->theSprite->OAM[currentPart].deOAM.ObjShape][curFrame->theSprite->OAM[currentPart].deOAM.ObjSize][1];
			int oldx = curFrame->theSprite->OAM[currentPart].deOAM.xCoord;
			int oldy = curFrame->theSprite->OAM[currentPart].deOAM.yCoord;

			xVar = xVar-partWidth;
			yVar = yVar-partHeight;
		

			curFrame->theSprite->OAM[currentPart].deOAM.xCoord = (xVar) & 511;
			curFrame->theSprite->OAM[currentPart].deOAM.yCoord = (yVar) & 0xff;

			UpdateStaticSprite();
			cOAMEdit::OamEditor->UpdatePartUI();
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}

///Windos proc for handling the animation window
LRESULT CALLBACK AnimationProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	Frame* curFrame = NULL;
	cOAMEdit* oamEditor = cOAMEdit::OamEditor;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (cOAMEdit::OamEditor->currentFrames != NULL)
		{

			curFrame = oamEditor->currentFrames->GetAnimatedFrame();
			if (!curFrame->frameInited) {
				globalVars->OAMED = true;
				if(GlobalVars::gblVars->frameTables->OAMFrameTable[curFrame->theSprite->id].front() != 0)
				{
					cOAMManager::SetupPreview(&GBA,currentRomType, curFrame);
				}
				oamEditor->LoadTiles(oamEditor->tileImage, curFrame);
				oamManager->DecodeOAM(globalVars->OAMED, curFrame->theSprite, curFrame->frameOffset - 0x8000000);
				globalVars->OAMED = false;
				oamManager->DrawPSprite(curFrame->theSprite);
				curFrame->frameInited = true;
				curFrame->animUpdated = true;
			}

			if (curFrame->animUpdated)
			{
				HBRUSH          br = CreateSolidBrush((COLORREF)RGB(255, 0, 0));
				RECT            rc;

				SetRect(&rc, 0, 0, curFrame->theSprite->PreviewSprite.Width, curFrame->theSprite->PreviewSprite.Height);
				FrameRect(hdc, &rc, br);

				DeleteObject(br);
				curFrame->animUpdated = false;
				curFrame->theSprite->PreviewSprite.Blit(hdc, 0, 0, 2048, 2048, 0, 0);
			}
			//BitBlt(hdc, 0, 0, 2048, 2048, animBuffer.DC(), 0, 0, SRCCOPY);


		}
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}


LRESULT CALLBACK OAMPartProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (cOAMEdit::OamEditor->currentFrames != NULL)
		{
			//cOAMEdit::OamEditor->partSprite->PreviewSprite.Blit(hdc, 0, 0, 1024, 1024, 0, 0);
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}


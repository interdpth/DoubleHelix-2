#include "MainHeader.h"
#include "GlobalVars.h"
#include "cEnitityManager.h"
#include "cSSE.h"
#include "SpriteObjectManager.h"
#include "resource.h"
void DrawSpritePAl(HDC hdc, long* palette, int size = 16)
{//Is called my WM_Paint for the palette window

	DrawSpritePAl(hdc, palette, 0, 0, 128, size);
}
int DrawSpritePAl(HDC hdc, long* palette, int X, int Y, int palcol, int size=16) {

	int i = 0;
	HBRUSH curbrush = NULL;
	RECT myrect = { 0,0,0,0 };
	//Blank the colors.
	for (i = 0; i < 256; i++) {

		curbrush = CreateSolidBrush(0);
		myrect.left = ((i) % 16) * size;
		myrect.top = ((i) / 16) * size;
		myrect.right = myrect.left + size;
		myrect.bottom = myrect.top + size;
		FillRect(hdc, &myrect, curbrush);
		DeleteObject(curbrush);

	}
	for (i = 128; i < 512; i++) {
		curbrush = CreateSolidBrush(palette[i]);
		myrect.left = ((i - 128) % 16) *size;
		myrect.top = ((i - 128) / 16) * size;
		myrect.right = myrect.left + size;
		myrect.bottom = myrect.top + size;
		FillRect(hdc, &myrect, curbrush);
		DeleteObject(curbrush);

	}
	return 0;

}


BOOL CALLBACK  SpriteSetEditorProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {

	cEntityManager* entMgr;
	if (RD1Engine::theGame && RD1Engine::theGame->mainRoom)
	{
		entMgr = RD1Engine::theGame->mainRoom->mgrEntities;
	}
	cSpriteSetEditor* theSpriteSet = cSpriteSetEditor::SpriteSet;
	PAINTSTRUCT ps;
	HDC hdc;
	unsigned char curdetail = 0;
	unsigned short i = 0;
	unsigned long off = 0;
	int  id = 0;
	unsigned long j = 0;
	RECT wnd;
	

	char buffer[3] = { 0 };
	switch (message)
	{
	case WM_INITDIALOG:
		cSpriteSetEditor::SpriteSet->me = hWnd;
		cSpriteSetEditor::SpriteSet->Create((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE));
		break;


	case WM_LBUTTONDOWN:


		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case lstSprites:
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				if (entMgr == NULL) break;
				cSpriteSetEditor::SpriteSet->GetSetData(currentRomType, cSpriteSetEditor::SpriteSet->SpriteSets.GetListIndex(), entMgr);//sprite manager should contain the index 
				cSpriteSetEditor::SpriteSet->GetSpritesPalAndTiles(currentRomType);

				//SendMessage(hWnd, WM_COMMAND, 0x000104a7, 0);
				InvalidateRect(cSpriteSetEditor::SpriteSet->PalView, 0, 1);
				InvalidateRect(cSpriteSetEditor::SpriteSet->SprTilesView, 0, 1);
			}
			break;
		case  cmdSetSprite:
			GetWindowText(GetDlgItem(hWnd, txtSV), buffer, 3);
			sscanf(buffer, "%X", &id);
			
		
			// SendMessage(hWnd,WM_COMMAND,0x000104a7,0);

			//we have our ID to use apply
			//Before we do any setting lets find out if there will be enough data to hold on this else we'll error 

			RD1Engine::theGame->mainRoom->mgrEntities->spriteset[theSpriteSet->Population.GetListIndex()].spriteID = (unsigned char)id;

		
			GlobalVars::gblVars->ReadObjectDetailsFromROM = false;
			GlobalVars::gblVars->SSE = true;
			cSpriteSetEditor::SpriteSet->GetSetData(currentRomType, 0, entMgr);
			cSpriteSetEditor::SpriteSet->GetSpritesPalAndTiles(currentRomType);

			InvalidateRect(cSpriteSetEditor::SpriteSet->PalView, 0, 1);
			InvalidateRect(cSpriteSetEditor::SpriteSet->SprTilesView, 0, 1);
			GlobalVars::gblVars->ReadObjectDetailsFromROM = false;
			GlobalVars::gblVars->SSE = true;
			break;
		case lstPopulation:
			if (HIWORD(wParam) == LBN_SELCHANGE && entMgr != NULL) {
				if (theSpriteSet->SpritePreview)
				{
					delete theSpriteSet->SpritePreview;
				}
				int pop = theSpriteSet->Population.GetListIndex();
				if (pop == -1)
				{
					pop=0;
				}
			//	theSpriteSet->GetSetData(currentRomType, 0, mgr);
				
				theSpriteSet->SpritePreview = new SpriteObject(cSpriteSetEditor::SpriteSet->SpriteSetData.graphics, cSpriteSetEditor::SpriteSet->SpriteSetData.pal);
				theSpriteSet->SpritePreview->id = entMgr->spriteset[pop].spriteID;
				theSpriteSet->SpritePreview->details = entMgr->spriteset[pop].sprdetail;
				theSpriteSet->GetSpritesPalAndTiles(currentRomType);
				theSpriteSet->cboDetail.SetListIndex(theSpriteSet->SpritePreview->details);
				theSpriteSet->SetupPreview(theSpriteSet->SpritePreview, currentRomType);

				sprintf(buffer, "%X", entMgr->spriteset[pop].spriteID);
				SetWindowText(GetDlgItem(hWnd, txtSV), buffer);

				InvalidateRect(theSpriteSet->PalView, 0, true);
				InvalidateRect(theSpriteSet->SprTilesView, 0, true);
			}
			break;
		case cboSlot:
			
			break;
		case cmdSaveSet:
			//Just bare min saving 
			RD1Engine::theGame->titleInstance->SeekSpriteTable(cSpriteSetEditor::SpriteSet->SpriteSets.GetListIndex());

			for (i = 0; i < RD1Engine::theGame->mgrOAM->maxsprite ; i++) {
				MemFile::currentFile->fwrite(&entMgr->spriteset[i].spriteID, 1, 1);
				MemFile::currentFile->fwrite(&entMgr->spriteset[i].sprdetail, 1, 1);
			}
			SendMessage(hWnd, WM_COMMAND, HIWORD(LBN_SELCHANGE) | LOWORD(lstSprites), 0);
			//SendMessage(hwndMain(), WM_COMMAND, 0x00010408, 0);
			break;
		case cmdSavePal:
			GlobalVars::gblVars->SSE = true;
			RD1Engine::theGame->mainRoom->mgrSpriteObjects->SavePal(entMgr->palinfo, entMgr->spriteset, cSpriteSetEditor::SpriteSet->SpriteSetData.pal);
			GBA.Reopen();
			GlobalVars::gblVars->SSE = false;
			//SendMessage(hwndMain(), WM_COMMAND, 0x00010408, 0);
			break;
		case WM_DESTROY:
			ShowWindow(hWnd, SW_HIDE);
			break;

		}
		break;
	case WM_VSCROLL:	// exact same idea, but V scroll instead of H scroll

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (cSpriteSetEditor::SpriteSet->SpritePreview)
		{
			GetWindowRect(GetDlgItem(hWnd, IdSprPrev), &wnd);
			cSpriteSetEditor::SpriteSet->SpritePreview->PreviewSprite.Blit(hdc, wnd.left, wnd.top, 82, 128, 0, 0);
		}

		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_SHOWWINDOW:
		 LoadCurSprite();	
		break;


	}

	//return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}




LRESULT CALLBACK cSSEPalProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	
	int i = 0;
	char tpal[256] = { 0 };

	switch (message)
	{
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		DrawSpritePAl(hdc, cSpriteSetEditor::SpriteSet->SpriteSetData.pal);



		EndPaint(hWnd, &ps);
		break;
	case WM_RBUTTONDOWN:
		//	sprintf(tpal,"%d",(GetX(lParam)/16)+(GetY(lParam)/16)*16);

			//MessageBox(hWnd,tpal,NULL,MB_OK);
		break;



	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}




LRESULT CALLBACK cSSETileProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		cSpriteSetEditor::SpriteSet->vramImage.Blit(hdc, 0, 0, 1024, 1024, 0, 0);



		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hdc);
		break;




		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}




int cSpriteSetEditor::SlightChange(int TitleChoice, unsigned char SpriteSetSelect, cEntityManager* mgr) {
	char tehbuf[256] = { 0 };
	int i = 0;
	total = 0;
	GlobalVars::gblVars->ReadObjectDetailsFromROM = true;
	if (currentRomType == 0) {
	
		mgr->LoadSet(true, &mgr->gfxinfo, mgr->palinfo, mgr->spriteset, (unsigned char)SpriteSets.GetListIndex());

	
	}
	else if (currentRomType == 1) {
	
		mgr->MFLoadSet(true,  &mgr->gfxinfo, mgr->palinfo, mgr->spriteset, (unsigned char)SpriteSets.GetListIndex());
		
	}
	GlobalVars::gblVars->ReadObjectDetailsFromROM = false;
	return 0;
}

int CheckTheSize(sprite_entry* SE) {
	long* GFX = 0;
	long* Pal = 0;
	unsigned long   i = 0;
	unsigned long   off = 0;
	unsigned long   j = 0;
	unsigned char id = 0;



	GFX = new long[RD1Engine::theGame->mgrOAM->maxsprite ];
	Pal = new long[RD1Engine::theGame->mgrOAM->maxsprite ];
	for (i = 0; i < RD1Engine::theGame->mgrOAM->maxsprite ; i++) GFX[i] = Pal[i] = 0;
	off = 0;
	if (currentRomType == 0) {


		RD1Engine::theGame->mainRoom->mgrSpriteObjects->GetZMSetSZ(GFX, Pal, SE);
		//Calculate Size
		for (i = 0; i < RD1Engine::theGame->mgrOAM->maxsprite ; i++)
			j += GFX[i];

		if (j <= 0x4000) {

			off++;
		}
		j = 0;
		for (i = 0; i < RD1Engine::theGame->mgrOAM->maxsprite ; i++) j += Pal[i];
		if (j < 256) {

			off++;

		}
	}
	else if (currentRomType == 1) {

		RD1Engine::theGame->mainRoom->mgrSpriteObjects->GetMFSetSZ(GFX, Pal, SE);
		for (i = 0; i < RD1Engine::theGame->mgrOAM->maxsprite ; i++)    j += GFX[i];

		if (j < 0x4000) {

			off++;
		}
		j = 0;
		for (i = 0; i < RD1Engine::theGame->mgrOAM->maxsprite ; i++) j += Pal[i];
		if (j < 256) {

			off++;

		}
	}
	delete[] GFX;
	delete[] Pal;
	/*
	 if(off==2){
		return 1;
	 } else {
		 MessageBox(0,"The current sprite/details do not work\nReverting to working","Error",MB_OK);

	 }*/
	return 0;

}

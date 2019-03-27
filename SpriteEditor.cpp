#include "MainHeader.h"
#include "GlobalVars.h"
#include "cEnitityManager.h"
#include "cSSE.h"
#include "SpriteObjectManager.h"
#include "resource.h"
void DrawPal(HDC hdc, long* palette, int size = 16)
{//Is called my WM_Paint for the palette window

	DrawPal(hdc, palette, 0, 0, 128, size);
}
int DrawPal(HDC hdc, long* palette, int X, int Y, int palcol, int size=16) {

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


void hey()
{

}
BOOL CALLBACK  cSSEProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {

	cEntityManager* mgr;
	if (RD1Engine::theGame && RD1Engine::theGame->mainRoom)
	{
		mgr = RD1Engine::theGame->mainRoom->mgrEntities;
	}
	cSSE* theSpriteSet = cSSE::SpriteSet;
	PAINTSTRUCT ps;
	HDC hdc;
	unsigned char curdetail = 0;
	unsigned char newdetail = 0;
	unsigned short i = 0;
	unsigned long off = 0;
	int  id = 0;
	unsigned long j = 0;
	RECT wnd;
	
	cEntityManager *entMgr;
	if (RD1Engine::theGame->mainRoom->mgrEntities)
	{
		entMgr = RD1Engine::theGame->mainRoom->mgrEntities;
	}
	char buffer[3] = { 0 };
	switch (message)
	{
	case WM_INITDIALOG:
		cSSE::SpriteSet->me = hWnd;
		cSSE::SpriteSet->Create((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE));
		break;


	case WM_LBUTTONDOWN:


		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case lstSprites:
			if (HIWORD(wParam) == LBN_SELCHANGE) {

				cSSE::SpriteSet->GetSet(currentRomType, cSSE::SpriteSet->SpriteSets.GetListIndex(), mgr);//sprite manager should contain the index 
				cSSE::SpriteSet->DecodeSet(currentRomType);
				cSSE::SpriteSet->SetInfo();
				cSSE::SpriteSet->Population.SetListIndex(0);
				SendMessage(hWnd, WM_COMMAND, 0x000104a7, 0);
				InvalidateRect(cSSE::SpriteSet->PalView, 0, 1);
				InvalidateRect(cSSE::SpriteSet->SprTilesView, 0, 1);
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
			cSSE::SpriteSet->GetSet(currentRomType, 0, mgr);
			cSSE::SpriteSet->DecodeSet(currentRomType);
			cSSE::SpriteSet->SetInfo();
			//cSSE::SpriteSet->Population.SetListIndex(0);
			SendMessage(hWnd, WM_COMMAND, 0x000104a7, 0);
			cSSE::SpriteSet->Population.SetListIndex(theSpriteSet->Population.GetListIndex());

			InvalidateRect(cSSE::SpriteSet->PalView, 0, 1);
			InvalidateRect(cSSE::SpriteSet->SprTilesView, 0, 1);
			GlobalVars::gblVars->ReadObjectDetailsFromROM = false;
			GlobalVars::gblVars->SSE = true;
			break;
		case lstPopulation:
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				if (theSpriteSet->SpritePreview)
				{
					delete theSpriteSet->SpritePreview;
				}
				int pop = theSpriteSet->Population.GetListIndex();
				if (pop == -1)
				{
					pop=0;
				}
			
				theSpriteSet->SpritePreview = new SprGBuf(cSSE::SpriteSet->SpriteSetData.graphics, cSSE::SpriteSet->SpriteSetData.pal);
				theSpriteSet->SpritePreview->id = entMgr->spriteset[pop].spriteID;
				theSpriteSet->SpritePreview->details = entMgr->spriteset[pop].sprdetail;
				theSpriteSet->cboDetail.SetListIndex(theSpriteSet->SpritePreview->details);
				theSpriteSet->SetupPreview(theSpriteSet->SpritePreview, currentRomType);

				sprintf(buffer, "%X", entMgr->spriteset[pop].spriteID);
				SetWindowText(GetDlgItem(hWnd, txtSV), buffer);

				InvalidateRect(theSpriteSet->PalView, 0, true);
				InvalidateRect(theSpriteSet->SprTilesView, 0, true);
			}
			break;
		case cboSlot:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				//Change this sprites slot :O could it cause problems? We'll see!
				curdetail = mgr->spriteset[theSpriteSet->Population.GetListIndex()].sprdetail;

				for (i = 0; i < theSpriteSet->total; i++) {
					if ((mgr->spriteset[i].sprdetail == theSpriteSet->cboDetail.GetListIndex())) {//Look for the new detail from the combo 
						id = 1;
						break;

					}
					else {

						id = 0;
					}

				}
				if (id)mgr->spriteset[i].sprdetail = curdetail;
				mgr->spriteset[cSSE::SpriteSet->Population.GetListIndex()].sprdetail = cSSE::SpriteSet->cboDetail.GetListIndex();

				GlobalVars::gblVars->ReadObjectDetailsFromROM = true;
				GlobalVars::gblVars->SSE = true;
				theSpriteSet->GetSet(currentRomType, 0, mgr);
				theSpriteSet->DecodeSet(currentRomType);
				theSpriteSet->SetInfo();
				//cSSE::SpriteSet->Population.SetListIndex(0);
				theSpriteSet->Population.SetListIndex(theSpriteSet->Population.GetListIndex());
				SendMessage(hWnd, WM_COMMAND, 0x000104a7, 0);
				InvalidateRect(theSpriteSet->PalView, 0, 1);
				InvalidateRect(theSpriteSet->SprTilesView, 0, 1);
				GlobalVars::gblVars->ReadObjectDetailsFromROM = false;
				GlobalVars::gblVars->SSE = true;

			}


			break;
		case cmdSaveSet:
			//Just bare min saving 
			if (currentRomType == 0) {

				MemFile::currentFile->seek((cSSE::SpriteSet->SpriteSets.GetListIndex() * 4) + 0x75F31C);
				MemFile::currentFile->fread(&off, sizeof(long), 1);
				MemFile::currentFile->seek(off - 0x8000000);



			}
			else if (currentRomType == 1) {
				MemFile::currentFile->seek(cSSE::SpriteSet->SpriteSets.GetListIndex() * 4 + 0x79ADD8);
				MemFile::currentFile->fread(&off, sizeof(long), 1);
				MemFile::currentFile->seek(off - 0x8000000);


			}

			for (i = 0; i < cSSE::SpriteSet->total; i++) {
				MemFile::currentFile->fwrite(&mgr->spriteset[i].spriteID, 1, 1);
				MemFile::currentFile->fwrite(&mgr->spriteset[i].sprdetail, 1, 1);
			}
			SendMessage(hWnd, WM_COMMAND, HIWORD(LBN_SELCHANGE) | LOWORD(lstSprites), 0);
			//SendMessage(hwndMain(), WM_COMMAND, 0x00010408, 0);
			break;
		case cmdSavePal:
			GlobalVars::gblVars->SSE = true;
			RD1Engine::theGame->mainRoom->mgrSpriteObjects->SavePal(mgr->palinfo, mgr->spriteset, cSSE::SpriteSet->SpriteSetData.pal);
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
		if (cSSE::SpriteSet->SpritePreview)
		{
			GetWindowRect(GetDlgItem(hWnd, IdSprPrev), &wnd);
			cSSE::SpriteSet->SpritePreview->PreviewSprite.Blit(hdc, wnd.left, wnd.top, 82, 128, 0, 0);
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
	int buf = 0;
	int i = 0;
	char tpal[256] = { 0 };

	switch (message)
	{
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		DrawPal(hdc, cSSE::SpriteSet->SpriteSetData.pal);



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

		cSSE::SpriteSet->Tiles.Blit(hdc, 0, 0, 512, 512, 0, 0);



		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hdc);
		break;




		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}




int cSSE::SlightChange(int TitleChoice, unsigned char SpriteSetSelect, cEntityManager* mgr) {
	char tehbuf[256] = { 0 };
	int i = 0;
	total = 0;
	if (currentRomType == 0) {
		GlobalVars::gblVars->ReadObjectDetailsFromROM = true;

		mgr->LoadSet(true, mgr->gfxinfo, mgr->palinfo, mgr->spriteset, (unsigned char)SpriteSets.GetListIndex());

		GlobalVars::gblVars->ReadObjectDetailsFromROM = false;
	}
	else if (currentRomType == 1) {
		GlobalVars::gblVars->ReadObjectDetailsFromROM = true;
		mgr->MFLoadSet(true,  mgr->gfxinfo, mgr->palinfo, mgr->spriteset, (unsigned char)SpriteSets.GetListIndex());
		GlobalVars::gblVars->ReadObjectDetailsFromROM = false;
	}

	return 0;
}

int CheckTheSize(sprite_entry* SE) {
	long* GFX = 0;
	long* Pal = 0;
	unsigned long   i = 0;
	unsigned long   off = 0;
	unsigned long   j = 0;
	unsigned char id = 0;



	GFX = new long[cSSE::SpriteSet->total];
	Pal = new long[cSSE::SpriteSet->total];
	for (i = 0; i < cSSE::SpriteSet->total; i++) GFX[i] = Pal[i] = 0;
	off = 0;
	if (currentRomType == 0) {


		RD1Engine::theGame->mainRoom->mgrSpriteObjects->GetZMSetSZ(GFX, Pal, SE);
		//Calculate Size
		for (i = 0; i < cSSE::SpriteSet->total; i++)
			j += GFX[i];

		if (j <= 0x4000) {

			off++;
		}
		j = 0;
		for (i = 0; i < cSSE::SpriteSet->total; i++) j += Pal[i];
		if (j < 256) {

			off++;

		}
	}
	else if (currentRomType == 1) {

		RD1Engine::theGame->mainRoom->mgrSpriteObjects->GetMFSetSZ(GFX, Pal, SE);
		for (i = 0; i < cSSE::SpriteSet->total; i++)    j += GFX[i];

		if (j < 0x4000) {

			off++;
		}
		j = 0;
		for (i = 0; i < cSSE::SpriteSet->total; i++) j += Pal[i];
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

#include "MainHeader.h"
#include "GlobalVars.h"
#include "TilesetManager.h"
#include "SpriteObjectManager.h"
#include "BaseGame.h"
sCombo cTransparency;
sCombo cMusic;
sCombo cmTileset;
sCombo cEffect;
sCombo cMiniX;
sCombo cMiniY;
sCombo comboSpriteSet;
sCombo cSYP;
sCombo cTrans;
sCombo cF;
sCombo cL;
sCombo cB;
sCombo cBI;
sCombo cEC;
sCombo cSG;
sCombo cControl1;
sCombo cControl2;
sCombo cControl3;
sCombo cControl4;
void LoadCombos(sCombo* Combo, char *FileName, int Max = 255);
BOOL CALLBACK  HeaderProced(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {

	unsigned char Width = 0;
	unsigned char Height = 0;
	int i = 0;
	switch (message)
	{
	case WM_INITDIALOG:
		hwndHeader = hWnd;
		SetupHeaderControls();
		break;


	case WM_LBUTTONDOWN:


		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case WM_DESTROY:
			hwndHeader = NULL;
			ShowWindow(hWnd, SW_HIDE);
			break;
		case cboSprites:
			LoadHeaderControls();
			break;
		case cmdSave:
			SaveHeader(1);
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			//RD1Engine::theGame->mainRoom->LoadUpSprites(0);
			break;
		case cmdChangeS:
			ChangeSprites();
			LoadHeaderControls();
			break;
		case cboSG:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				switch (comboSpriteSet.GetListIndex()) {
				case 0:
					RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex1 = (unsigned char)cSG.GetListIndex();
					break;
				case 1:
					RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex2 = (unsigned char)cSG.GetListIndex();
					break;
				case 2:
					RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex3 = (unsigned char)cSG.GetListIndex();
					break;
				}
			}
			break;
		case cboEC:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				{
					switch (comboSpriteSet.GetListIndex()) {

					case 0:
						RD1Engine::theGame->mainRoom->roomHeader.bEventSwitch = (unsigned char)cEC.GetListIndex();
						break;
					case 1:
						RD1Engine::theGame->mainRoom->roomHeader.bEventSwitch2 = (unsigned char)cEC.GetListIndex();
						break;



					}
				}
				break;
		case cboTileset:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				RD1Engine::theGame->mainRoom->roomHeader.bTileset = cmTileset.GetListIndex();
				RD1Engine::theGame->mgrTileset->GetTileset(GlobalVars::gblVars->imgTileset, RD1Engine::theGame->mainRoom->Area, cmTileset.GetListIndex(), RD1Engine::theGame->mainRoom->roomHeader.lBg3);
				RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->Dirty = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Dirty = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->Dirty = 1;
				RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			}
			break;
		case cboTrans:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				RD1Engine::theGame->mainRoom->roomHeader.TransForeground = cTransparency.GetListIndex();
				RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->Dirty = 1;
				RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->SDirty = 1;
				RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			}
			break;
		case cboSYP:
			if (HIWORD(wParam) == CBN_SELCHANGE) {

				RD1Engine::theGame->mainRoom->roomHeader.bSceneryYPos = cSYP.GetListIndex();
				RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
				InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
			}
			break;

		case cboMusic:
			if (HIWORD(wParam) == CBN_SELCHANGE) {

				RD1Engine::theGame->mainRoom->roomHeader.bMusic = cMusic.GetListIndex();


			}
			break;
		case cboEffect:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				RD1Engine::theGame->mainRoom->roomHeader.bEffect = cEffect.GetListIndex();
			}
			break;
			}
			break;

		case WM_SHOWWINDOW:
			SendMessage(hwndMM, WM_COMMAND, LOWORD(cboMAreaC), 0);
			break;
		}

	}

	//	return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}
int SetupHeaderControls() {
	char buffer[256] = { 0 };
	int i = 0;
	char Compress[3][5] = { "None","RLE","LZ77" };
	cTransparency.Init(GetDlgItem(hwndHeader, cboTrans));
	cMusic.Init(GetDlgItem(hwndHeader, cboMusic));
	cEffect.Init(GetDlgItem(hwndHeader, cboEffect));
	cMiniX.Init(GetDlgItem(hwndHeader, cboMiniX));
	cMiniY.Init(GetDlgItem(hwndHeader, cboMiniY));
	cmTileset.Init(GetDlgItem(hwndHeader, cboTileset));
	comboSpriteSet.Init(GetDlgItem(hwndHeader, cboSprites));
	cSYP.Init(GetDlgItem(hwndHeader, cboSYP));
	//cTrans.Init(GetDlgItem(hwndHeader,cboTrans));
	cEC.Init(GetDlgItem(hwndHeader, cboEC));
	cSG.Init(GetDlgItem(hwndHeader, cboSG));
	cF.Init(GetDlgItem(hwndHeader, cboCF));
	cL.Init(GetDlgItem(hwndHeader, cboCL));
	cB.Init(GetDlgItem(hwndHeader, cboCB));
	cBI.Init(GetDlgItem(hwndHeader, cboCBI));
	cControl1.Init(GetDlgItem(hwndHeader, cboControl1));
	cControl2.Init(GetDlgItem(hwndHeader, cboControl2));
	cControl3.Init(GetDlgItem(hwndHeader, cboControl3));
	cControl4.Init(GetDlgItem(hwndHeader, cboControl4));

	char quick[2048];

	for (i = 0; i < 256; i++) {
		sprintf(buffer, "%X", i);
		cTransparency.Additem(buffer);

		cEffect.Additem(buffer);
		cMiniX.Additem(buffer);
		cMiniY.Additem(buffer);
		cmTileset.Additem(buffer);
		cMusic.Additem(buffer);

		if (i < 3)comboSpriteSet.Additem(buffer);
		cSYP.Additem(buffer);
		cTrans.Additem(buffer);
		cEC.Additem(buffer);
		cSG.Additem(buffer);
		if (16 > i) {
			cControl1.Additem(buffer);
			cControl2.Additem(buffer);
			cControl3.Additem(buffer);
			cControl4.Additem(buffer);
		}
	}
	for (i = 0; i < 3; i++) {
		cF.Additem(Compress[i]);
		cL.Additem(Compress[i]);
		cB.Additem(Compress[i]);
		cBI.Additem(Compress[i]);
	}
	comboSpriteSet.SetListIndex(0);
	return 0;
}

int LoadHeaderControls() {
	char itembuf[256];
	//Load layer stuff first
	//Foreground
	unsigned long Offset = 0;
	if (currentRomType == 0)
	{
		Offset = ((RD1Engine::theGame->RoomOffsets[Combos[cArea].GetListIndex()] - 0x8000000) + Combos[cRoom].GetListIndex() * 0x3C);
	}
	else if (currentRomType == 1)
	{
		Offset = ((RD1Engine::theGame->RoomOffsets[Combos[cArea].GetListIndex()] - 0x8000000) + Combos[cRoom].GetListIndex() * 0x3C);
	}
	sprintf(itembuf, "For manual editing this header can be found at offset: %6X", Offset);
	SetWindowText(GetDlgItem(hwndHeader, lblHeaderOffset), itembuf);
	if (RD1Engine::theGame->mainRoom->roomHeader.bBg0 & 0x40) {
		cF.SetListIndex(2);
	}
	else if (RD1Engine::theGame->mainRoom->roomHeader.bBg0 & 0x10) {
		cF.SetListIndex(1);
	}
	else {
		cF.SetListIndex(0);
	}

	if (RD1Engine::theGame->mainRoom->roomHeader.bBg1 & 0x40) {
		cL.SetListIndex(2);
	}
	else if (RD1Engine::theGame->mainRoom->roomHeader.bBg1 & 0x10) {
		cL.SetListIndex(1);
	}
	else {
		cL.SetListIndex(0);
	}

	if (RD1Engine::theGame->mainRoom->roomHeader.bBg2 & 0x40) {
		cB.SetListIndex(2);
	}
	else if (RD1Engine::theGame->mainRoom->roomHeader.bBg2 & 0x10) {
		cB.SetListIndex(1);
	}
	else {
		cB.SetListIndex(0);
	}
	if (RD1Engine::theGame->mainRoom->roomHeader.lBg3 & 0x40) {
		cBI.SetListIndex(2);
	}
	else if (RD1Engine::theGame->mainRoom->roomHeader.lBg3 & 0x10) {
		cBI.SetListIndex(1);
	}
	else {
		cBI.SetListIndex(0);
	}



	//Load offsets
	sprintf(itembuf, "%X", RD1Engine::theGame->mainRoom->roomHeader.lForeground - 0x8000000);
	SetWindowText(GetDlgItem(hwndHeader, txtFO), itembuf);
	sprintf(itembuf, "%X", RD1Engine::theGame->mainRoom->roomHeader.lLevelData - 0x8000000);
	SetWindowText(GetDlgItem(hwndHeader, txtLO), itembuf);
	sprintf(itembuf, "%X", RD1Engine::theGame->mainRoom->roomHeader.lBackLayer - 0x8000000);
	SetWindowText(GetDlgItem(hwndHeader, txtBO), itembuf);
	sprintf(itembuf, "%X", RD1Engine::theGame->mainRoom->roomHeader.lBackgroundTSA - 0x8000000);
	SetWindowText(GetDlgItem(hwndHeader, txtBIO), itembuf);

	cSYP.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bSceneryYPos);
	cTransparency.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.TransForeground);
	cMusic.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bMusic);
	cEffect.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bEffect);
	cmTileset.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bTileset);
	cMiniX.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomX);
	cMiniY.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomY);

	switch (comboSpriteSet.GetListIndex()) {
		EnableWindow(cEC.GetHwnd(), 1);
	case 0:
		cEC.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bEventSwitch);
		sprintf(itembuf, "Spriteset %X has %X number of sprites", comboSpriteSet.GetListIndex(), RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[0].Max());
		SetWindowText(GetDlgItem(hwndHeader, lblNumber), itembuf);
		cSG.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex1);
		sprintf(itembuf, "%X", RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer);
		SetWindowText(GetDlgItem(hwndHeader, txtSOffset), itembuf);
		break;
	case 1:

		cEC.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bEventSwitch2);
		cSG.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex2);
		sprintf(itembuf, "%X", RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer2);
		SetWindowText(GetDlgItem(hwndHeader, txtSOffset), itembuf);
		sprintf(itembuf, "Spriteset %X has %X number of sprites", comboSpriteSet.GetListIndex(), RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Max());
		SetWindowText(GetDlgItem(hwndHeader, lblNumber), itembuf);
		break;
	case 2:
		EnableWindow(cEC.GetHwnd(), 0);

		cSG.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex3);
		sprintf(itembuf, "%X", RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer3);
		SetWindowText(GetDlgItem(hwndHeader, txtSOffset), itembuf);
		sprintf(itembuf, "Spriteset %X has %X number of sprites", comboSpriteSet.GetListIndex(), RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[2].Max());
		SetWindowText(GetDlgItem(hwndHeader, lblNumber), itembuf);
		break;
	}
	cControl1.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bBg0 & 0xF);
	cControl2.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bBg1 & 0xF);
	cControl3.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bBg2 & 0xF);
	cControl4.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.lBg3 & 0xF);
	return 0;
}

int ChangeSprites() {

	int i = 0;
	int set = comboSpriteSet.GetListIndex();
	char blah[1024] = "Please enter a new number\nfor the amount of sprites you want.\nMax is 255, least is 0 in hex ";
	int sprites = 0;

	unsigned long* offsets[3] = { &RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer,&RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer2,&RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer3 };
	nEnemyList* cursz[3] = { &RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[0], &RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1], &RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[2] };


	memcpy(procstring, blah, 1024);
	sprites = LoadInput(cursz[set]->Max());
	//We now have how many 
	if (sprites < 0) sprites = 0;
	if (sprites > 16)sprites =16;
	//Then see if sprites is bigger then current sets max

	if (sprites > (cursz[set]->Max()))//Bigger let's find some space
		*offsets[set] = GBA.FindFreeSpace(sprites * 3 + 32, 0xFF) + 0x8000000;//Find more than enough free bytes

	MemFile::currentFile->seek(*offsets[set] - 0x8000000);
	for (i = 0; i < cursz[set]->Max(); i++)
		MemFile::currentFile->fwrite(&cursz[set]->Enemies[i], 3, 1, GBA.ROM);


	//Put 3 bytes to say we're done
	MemFile::currentFile->fputc(0xFF);
	MemFile::currentFile->fputc(0xFF);
	MemFile::currentFile->fputc(0xFF);
	SaveHeader(1);
	//RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewL.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewF.value() == BST_CHECKED);
	//RD1Engine::theGame->mainRoom->LoadUpSprites(0);

	return 0;
}

int SaveHeader(unsigned char call) {
	unsigned long  ho = 0;//ho is Header offset :D
	char itembuf[256] = { 0 };
	//Load layer stuff first
	//Foreground


   //Load offsets
	if (call != 2) {
		RD1Engine::theGame->mainRoom->roomHeader.lForeground = 0;
		RD1Engine::theGame->mainRoom->roomHeader.lLevelData = 0;
		RD1Engine::theGame->mainRoom->roomHeader.lBackLayer = 0;
		RD1Engine::theGame->mainRoom->roomHeader.lBackgroundTSA = 0;
		GetWindowText(GetDlgItem(hwndHeader, txtFO), itembuf, 256);
		sscanf(itembuf, "%X", &RD1Engine::theGame->mainRoom->roomHeader.lForeground);
		RD1Engine::theGame->mainRoom->roomHeader.lForeground += 0x8000000;
		GetWindowText(GetDlgItem(hwndHeader, txtLO), itembuf, 256);
		sscanf(itembuf, "%X", &RD1Engine::theGame->mainRoom->roomHeader.lLevelData);
		RD1Engine::theGame->mainRoom->roomHeader.lLevelData += 0x8000000;
		GetWindowText(GetDlgItem(hwndHeader, txtBO), itembuf, 256);
		sscanf(itembuf, "%X", &RD1Engine::theGame->mainRoom->roomHeader.lBackLayer);
		RD1Engine::theGame->mainRoom->roomHeader.lBackLayer += 0x8000000;
		GetWindowText(GetDlgItem(hwndHeader, txtBIO), itembuf, 256);
		sscanf(itembuf, "%X", &RD1Engine::theGame->mainRoom->roomHeader.lBackgroundTSA);
		RD1Engine::theGame->mainRoom->roomHeader.lBackgroundTSA += 0x8000000;
		RD1Engine::theGame->mainRoom->roomHeader.bSceneryYPos = (unsigned char)cSYP.GetListIndex();
		RD1Engine::theGame->mainRoom->roomHeader.TransForeground = (unsigned char)cTransparency.GetListIndex();
		RD1Engine::theGame->mainRoom->roomHeader.bMusic = (unsigned char)cMusic.GetListIndex();
		RD1Engine::theGame->mainRoom->roomHeader.bEffect = (unsigned char)cEffect.GetListIndex();
		RD1Engine::theGame->mainRoom->roomHeader.bTileset = (unsigned char)cmTileset.GetListIndex();
		RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomX = (unsigned char)cMiniX.GetListIndex();
		RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomY = (unsigned char)cMiniY.GetListIndex();

		switch (comboSpriteSet.GetListIndex()) {
			EnableWindow(cEC.GetHwnd(), 1);
		case 0:
			cEC.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bEventSwitch);
			sprintf(itembuf, "Spriteset %X has %X number of sprites", comboSpriteSet.GetListIndex(), RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[0].Max());
			SetWindowText(GetDlgItem(hwndHeader, lblNumber), itembuf);
			cSG.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex1);
			sprintf(itembuf, "%X", RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer);
			SetWindowText(GetDlgItem(hwndHeader, txtSOffset), itembuf);
			break;
		case 1:

			cEC.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bEventSwitch2);
			cSG.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex2);
			sprintf(itembuf, "%X", RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer2);
			SetWindowText(GetDlgItem(hwndHeader, txtSOffset), itembuf);
			sprintf(itembuf, "Spriteset %X has %X number of sprites", comboSpriteSet.GetListIndex(), RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[1].Max());
			SetWindowText(GetDlgItem(hwndHeader, lblNumber), itembuf);
			break;
		case 2:
			EnableWindow(cEC.GetHwnd(), 0);

			cSG.SetListIndex(RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex3);
			sprintf(itembuf, "%X", RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer3);
			SetWindowText(GetDlgItem(hwndHeader, txtSOffset), itembuf);
			sprintf(itembuf, "Spriteset %X has %X number of sprites", comboSpriteSet.GetListIndex(), RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[2].Max());
			SetWindowText(GetDlgItem(hwndHeader, lblNumber), itembuf);
			break;
		}
		switch (cF.GetListIndex()) {
		case 0:
			RD1Engine::theGame->mainRoom->roomHeader.bBg0 = (unsigned char)cControl1.GetListIndex();

			break;
		case 1:
			RD1Engine::theGame->mainRoom->roomHeader.bBg0 = 0x10 + ((unsigned char)cControl1.GetListIndex());
			break;
		case 2:
			RD1Engine::theGame->mainRoom->roomHeader.bBg0 = 0x40 + ((unsigned char)cControl1.GetListIndex());
			break;

		}
		switch (cL.GetListIndex()) {
		case 0:
			RD1Engine::theGame->mainRoom->roomHeader.bBg1 = ((unsigned char)cControl2.GetListIndex());

			break;
		case 1:
			RD1Engine::theGame->mainRoom->roomHeader.bBg1 = 0x10 + ((unsigned char)cControl2.GetListIndex());
			break;
		case 2:
			RD1Engine::theGame->mainRoom->roomHeader.bBg1 = 0x40 + ((unsigned char)cControl2.GetListIndex());
			break;

		}
		switch (cB.GetListIndex()) {
		case 0:
			RD1Engine::theGame->mainRoom->roomHeader.bBg2 = ((unsigned char)cControl3.GetListIndex());

			break;
		case 1:
			RD1Engine::theGame->mainRoom->roomHeader.bBg2 = 0x10 + ((unsigned char)cControl3.GetListIndex());
			break;
		case 2:
			RD1Engine::theGame->mainRoom->roomHeader.bBg2 = 0x40 + ((unsigned char)cControl3.GetListIndex());
			break;

		}if (!import) {
			switch (cBI.GetListIndex()) {

			case 0:
				RD1Engine::theGame->mainRoom->roomHeader.lBg3 = ((unsigned char)cControl4.GetListIndex());

				break;
			case 1:
				RD1Engine::theGame->mainRoom->roomHeader.lBg3 = 0x10 + ((unsigned char)cControl4.GetListIndex());
				break;
			case 2:
				RD1Engine::theGame->mainRoom->roomHeader.lBg3 = 0x40 + ((unsigned char)cControl4.GetListIndex());
				break;
			}
		}
		else {
			RD1Engine::theGame->mainRoom->roomHeader.lBg3 = 0xF0 + ((unsigned char)cControl4.GetListIndex());;
			import = 0;
		}

	}

	ho = ((RD1Engine::theGame->RoomOffsets[Combos[cArea].GetListIndex()] - 0x8000000) + Combos[cRoom].GetListIndex() * 0x3C);

	MemFile::currentFile->seek(ho);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bTileset, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bBg0, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bBg1, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bBg2, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.lBg3, sizeof(unsigned long), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.lForeground, sizeof(unsigned long), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.lLevelData, sizeof(unsigned long), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.lBackLayer, sizeof(unsigned long), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.lClipData, sizeof(unsigned long), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.lBackgroundTSA, sizeof(unsigned long), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bUnknown1, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.TransForeground, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.iSep1, sizeof(unsigned short), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer, sizeof(unsigned long), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex1, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bEventSwitch, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.iSep2, sizeof(unsigned short), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer2, sizeof(unsigned long), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex2, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bEventSwitch2, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.iSeperator, sizeof(unsigned short), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.lSpritePointer3, sizeof(unsigned long), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bSpriteIndex3, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomX, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bMiniMapRoomY, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bEffect, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bSceneryYPos, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bNothing, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.bMusic, sizeof(unsigned char), 1, GBA.ROM);
	MemFile::currentFile->fwrite(&RD1Engine::theGame->mainRoom->roomHeader.blank, sizeof(unsigned char), 1, GBA.ROM);


	if (call == 0)SendMessage(hwndMain(), WM_COMMAND, cboRoom, 0);
	return 0;
}

int CCForeground() {
	int blah = SceneType;
	long someaddress = 0;
	unsigned char *  compressdata = new unsigned char[32192];
	memset(compressdata, 0, 32192);
	switch (cF.GetListIndex()) {
		// ChangeCompression(BaseGame::theGame->mainRoom->roomHeader.bBg0, 0, 0, BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer));
		RD1Engine::theGame->mainRoom->roomHeader.bBg0 = 0;
		break;
	case 1:
		//	 ChangeCompression(BaseGame::theGame->mainRoom->roomHeader.bBg1, 1, 0, BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer));
		RD1Engine::theGame->mainRoom->roomHeader.bBg0 = 0x10;
		//	     SaveBackground();
		break;

	case 2:
		//		ChangeCompression(BaseGame::theGame->mainRoom->roomHeader.bBg1, 2, 0, BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer));
		RD1Engine::theGame->mainRoom->roomHeader.bBg0 = 0x40;
		break;
	}
	delete[]  compressdata;
	return 0;
}


int CCBackground() {




	switch (cBI.GetListIndex()) {
	case 0:
		ChangeCompression((unsigned char)RD1Engine::theGame->mainRoom->roomHeader.lBg3, 0, 3, RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer));
		RD1Engine::theGame->mainRoom->roomHeader.lBg3 = 0;
		break;
	case 1:
		ChangeCompression((unsigned char)RD1Engine::theGame->mainRoom->roomHeader.lBg3, 1, 3, RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer));
		RD1Engine::theGame->mainRoom->roomHeader.lBg3 = 0x10;
		//	     SaveBackground();
		break;

	case 2:
		ChangeCompression((unsigned char)RD1Engine::theGame->mainRoom->roomHeader.lBg3, 2, 3, RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer));
		RD1Engine::theGame->mainRoom->roomHeader.lBg3 = 0x40;
		break;
	}

	return 0;
}
void setsize(nMapBuffer* buffer) {


	unsigned long fgW = (buffer->X + 16) / 32, fgH = (buffer->Y + 16) / 32;

	if (fgW > 2)	fgW = 2;
	if (fgH > 2)	fgH = 2;

	if (fgW == 1 && fgH == 1)	buffer->size = 0;
	else if (fgW == 1 && fgH == 2)	buffer->size = 2;
	else buffer->size = 1;

}

void ChangeCompression(unsigned char oldlayervalue, unsigned char newval, unsigned char layer, nMapBuffer* buffer) {


	unsigned char oldscene;

	switch (newval) {
	case 0:
		oldlayervalue = 0;
		break;
	case 1:
		if (oldlayervalue == 0) {
			buffer->X = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata)->X;//Because clipdata will never be 0!
			buffer->Y = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata)->Y;
			memset(buffer->TileBuf2D, 0, 0xFE00);
		}


		break;

	case 2:
		oldscene = SceneType;
		if (layer == 1) {
			MessageBox(0, "Level Data can not be LZ compressd\nSwitching to RLE compression", "Error", MB_OK);
			SceneType = oldscene;
			ChangeCompression(oldlayervalue, 1, layer, buffer);

		}
		if (oldlayervalue == 0x10) {
			setsize(buffer);
		}
		else if (oldlayervalue == 0) {
			buffer->X = 32;
			buffer->Y = 32;
			memset(buffer->TileBuf2D, 0, 0xFE00);
		}
		switch (layer) {
		case 0:
			SceneType = 2;
			break;
		case 2:
			SceneType = 1;
			break;
		case 3:
			SceneType = 0;
			break;
		}


		// SaveCurScene();
		SceneType = oldscene;
		break;
	}



}


int BackUpAllHeaders() {
	char curpath[2048] = { 0 };
	int ai = 0;//Area Index;
	int ri = 0;//Room Index
	int lc = 0;
	FILE* fp = NULL;
	unsigned char  roomarray[0x3c];


	memcpy(curpath, GlobalVars::gblVars->AppPath, strlen(GlobalVars::gblVars->AppPath));
	if (currentRomType == 0) {
		for (ai = 0; ai < 7; ri++) {
			if (ai == 6) {
				lc = 0x63;
			}
			else {
				lc = (RD1Engine::theGame->RoomOffsets[ai + 1] - RD1Engine::theGame->RoomOffsets[ai]) / 0x3C;
			}
			MemFile::currentFile->seek(RD1Engine::theGame->RoomOffsets[ai] - 0x8000000);
			for (ri = 0; ri < lc; ri++) {
				memset(curpath, 0, 2048);
				MemFile::currentFile->fread(roomarray, 0x3c, 1, GBA.ROM);
				strcat(curpath, GlobalVars::gblVars->AppPath);
				strcat(curpath, "\\ZM\\HdrBackUp\\");
				//                    strcat(curpath,Combos[cboArea].sList(ai));
			}


		}
	}
	else 	if (currentRomType == 1) {
	}

	return 0;
}

int InsertAllHeaders() {


	return 0;
}
int BackUpAreaHeaders() {
	return 0;
}
int InsertAreaHeaders() {


	return 0;
}
int BackupCurHeader() {

	return 0;
}

int InsertCurHeader() {

	return 0;
}

int BackupAheaderInThisArea() {

	return 0;
}
int BackupAHeader() {

	return 0;
}
int InsertAHeaderInThisArea() {

	return 0;
}

int InsertAHeader() {

	return 0;
}
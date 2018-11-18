#include "MainHeader.h"
#include "GlobalVars.h"
#include "GBAGraphics.h"
#include "cOAMManager.h"
#include "nMapBuffer.h"
#include "DoorManager.h"
#include "TilesetManager.h"
#include "SpriteObjectManager.h"
#include "clsRoomScrolls.h"
#include "BaseGame.h"
#include "MemFile.h"
#include "Logger.h"
//Get the Major Arrays
void DumpLayers();
bool LiteralCompare(char* a, char* b);

/*int LoadRoom(){
int Room = */
bool IsValidCompression(unsigned char tstByte) {


	if (tstByte == 0 || tstByte == 0x10 || tstByte == 0x40 || tstByte == 0x31 || tstByte == 0x45) {

		return true;
	}
	printf("Layer had an invalid compression type");
	return false;
}

bool IsValidPointer(unsigned long pnt) {
	if (pnt == 0xffffffff || pnt == 0xffff0000 || pnt == 0x0003fffc) {
		return false;
	}
	unsigned long tstPointer = pnt;
	tstPointer = tstPointer & 0x8000000;
	return tstPointer == 0x8000000;
}

bool CheckHeader(RHeader* tstHeader)
{
	//Check layer compression
	if (IsValidCompression(tstHeader->bBg0) == false) {
		return false;
	}
	if (IsValidCompression(tstHeader->bBg1) == false) {
		return false;
	}
	if (IsValidCompression(tstHeader->bBg2) == false) {
		return false;
	}
	if (IsValidCompression(tstHeader->lBg3) == false) {
		return false;
	}

	//Check pointers 



	if (IsValidPointer(tstHeader->lForeground) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lLevelData) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lBackLayer) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lClipData) == false) {
		return false;
	}


	if (IsValidPointer(tstHeader->lBackgroundTSA) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lSpritePointer) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lSpritePointer2) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lSpritePointer3) == false) {
		return false;
	}



	return true;

}
int GetRoomCount(long RoomTableOffset) {
	vector<RHeader> roomHeaders;
	for (int i = 0; i < 1000; i++)
	{
		unsigned long TheOffset = ((RoomTableOffset - 0x8000000) + i * 0x3C);
		RHeader tmpHeader;
		MemFile::currentFile->seek(TheOffset);
		MemFile::currentFile->fread(&tmpHeader.bTileset, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bBg0, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bBg1, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bBg2, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.lBg3, sizeof(unsigned long), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.lForeground, sizeof(unsigned long), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.lLevelData, sizeof(unsigned long), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.lBackLayer, sizeof(unsigned long), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.lClipData, sizeof(unsigned long), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.lBackgroundTSA, sizeof(unsigned long), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bUnknown1, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.TransForeground, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.iSep1, sizeof(unsigned short), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.lSpritePointer, sizeof(unsigned long), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bSpriteIndex1, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bEventSwitch, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.iSep2, sizeof(unsigned short), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.lSpritePointer2, sizeof(unsigned long), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bSpriteIndex2, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bEventSwitch2, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.iSeperator, sizeof(unsigned short), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.lSpritePointer3, sizeof(unsigned long), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bSpriteIndex3, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bMiniMapRoomX, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bMiniMapRoomY, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bEffect, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bSceneryYPos, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bNothing, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.bMusic, sizeof(unsigned char), 1, GBA.ROM);
		MemFile::currentFile->fread(&tmpHeader.blank, sizeof(unsigned char), 1, GBA.ROM);

		if (CheckHeader(&tmpHeader) == true) {
			roomHeaders.push_back(tmpHeader);
		}
		else {
			break;
		}
	}
	return roomHeaders.size();
}


int DrawLevel() {
	RECT rcMap;
	int i = 0;
	//	GlobalVars::gblVars->imgMap->Clear();
	nMapBuffer* buffForeground = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround);
	nMapBuffer* buffLeveldata = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData);
	nMapBuffer* buffBacklayer = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer);
	nMapBuffer* buffClipdata = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata);
	
	//for(i=0;i<2;i++){
	//   GlobalVars::gblVars->checkBoxViewF.value(i);
	//	GlobalVars::gblVars->checkBoxViewL.value(i);
	//	GlobalVars::gblVars->checkBoxViewB.value(i);
	RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
	//}
	CurMapWidth = buffLeveldata->X;
	CurMapHeight = buffLeveldata->Y;
	GetWindowRect(UiState::stateManager->GetMapWindow(), &rcMap);
	nMaxHScroll[sHMap] = (buffLeveldata->X - (rcMap.right - rcMap.left) / 16);//BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X/16;	// maximum H scroll
	nMaxVScroll[sVMap] = (buffLeveldata->Y - (rcMap.bottom - rcMap.top) / 16);		// maximu
	ChangeScrollbars(hwndMain(), 1, sVMap);
	ChangeScrollbars(hwndMain(), 0, sHMap);
	/*   frmMain.hsbMap.max = (ThisRoom.X) - 25
	frmMain.vsbMap.max = (ThisRoom.Y) - 17
	#define sHMap 1
#define sVMap 2*/
	InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);


	return 0;
}




int             Save(MemFile * fp)
{
	MapManager *mm = RD1Engine::theGame->mainRoom->mapMgr;
	long            offset = 0;

	long            tlength;

	unsigned char*  compBuffer = new unsigned char[64691];
	memset(compBuffer, 0, 64691);

	if (fp)
	{


		MapManager::SaveLayer(&GBA, fp, RD1Engine::theGame->mainRoom->roomHeader.bBg0, &RD1Engine::theGame->mainRoom->roomHeader.lForeground, mm->GetLayer(MapManager::ForeGround));
		MapManager::SaveLayer(&GBA, fp, RD1Engine::theGame->mainRoom->roomHeader.bBg1, &RD1Engine::theGame->mainRoom->roomHeader.lLevelData, mm->GetLayer(MapManager::LevelData));
		MapManager::SaveLayer(&GBA, fp, RD1Engine::theGame->mainRoom->roomHeader.bBg2, &RD1Engine::theGame->mainRoom->roomHeader.lBackLayer, mm->GetLayer(MapManager::Backlayer));

		if (mm->GetLayer(MapManager::Clipdata)->SDirty == 1)
		{
			memset(compBuffer, 0, 64691);
			tlength = GBA.compress(compBuffer, (unsigned char *)mm->GetLayer(MapManager::Clipdata)->TileBuf2D, (mm->GetLayer(MapManager::Clipdata)->X * mm->GetLayer(MapManager::Clipdata)->Y) * 2);
			offset = GBA.FindFreeSpace(tlength + 4, 0xFF);
			if (offset != 0)
			{
				RD1Engine::theGame->mainRoom->roomHeader.lClipData = 0x8000000 + offset;
				MemFile::currentFile->seek(offset);
				MemFile::currentFile->fputc(mm->GetLayer(MapManager::Clipdata)->X);
				MemFile::currentFile->fputc(mm->GetLayer(MapManager::Clipdata)->Y);
				MemFile::currentFile->fwrite(compBuffer, sizeof(char), tlength, (FILE*)NULL);
			}
			else
			{
				sMessage("Could not save Clip Data.");
			}
		}
	}
	delete[] compBuffer;
	return 0;
}
int SaveLevel(FILE* fp, unsigned long HeaderOffset) {

	MemFile* theFile = MemFile::currentFile;
	RHeader* roomHeader;
	//	GBA.REDIT = fopen(FilePath,"r+b");
	if (theFile) {
		//   while(1){ 
		Save(theFile);
		//   if(BaseGame::theGame->mainRoom->roomHeader.lBackLayer!=BaseGame::theGame->mainRoom->roomHeader.lLevelData) break;
	  //	}
		RD1Engine::theGame->mainRoom->mgrSpriteObjects->SaveSprites(&RD1Engine::theGame->mainRoom->roomHeader);
		theFile->seek(HeaderOffset);
		roomHeader = &RD1Engine::theGame->mainRoom->roomHeader;


		theFile->fwrite(&roomHeader->bTileset, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bBg0, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bBg1, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bBg2, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->lBg3, sizeof(unsigned long), 1, fp);
		theFile->fwrite(&roomHeader->lForeground, sizeof(unsigned long), 1, fp);
		theFile->fwrite(&roomHeader->lLevelData, sizeof(unsigned long), 1, fp);
		theFile->fwrite(&roomHeader->lBackLayer, sizeof(unsigned long), 1, fp);
		theFile->fwrite(&roomHeader->lClipData, sizeof(unsigned long), 1, fp);
		theFile->fwrite(&roomHeader->lBackgroundTSA, sizeof(unsigned long), 1, fp);
		theFile->fwrite(&roomHeader->bUnknown1, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->TransForeground, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->iSep1, sizeof(unsigned short), 1, fp);
		theFile->fwrite(&roomHeader->lSpritePointer, sizeof(unsigned long), 1, fp);
		theFile->fwrite(&roomHeader->bSpriteIndex1, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bEventSwitch, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->iSep2, sizeof(unsigned short), 1, fp);
		theFile->fwrite(&roomHeader->lSpritePointer2, sizeof(unsigned long), 1, fp);
		theFile->fwrite(&roomHeader->bSpriteIndex2, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bEventSwitch2, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->iSeperator, sizeof(unsigned short), 1, fp);
		theFile->fwrite(&roomHeader->lSpritePointer3, sizeof(unsigned long), 1, fp);
		theFile->fwrite(&roomHeader->bSpriteIndex3, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bMiniMapRoomX, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bMiniMapRoomY, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bEffect, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bSceneryYPos, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bNothing, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->bMusic, sizeof(unsigned char), 1, fp);
		theFile->fwrite(&roomHeader->blank, sizeof(unsigned char), 1, fp);

		RD1Engine::theGame->mgrScrolls->SaveScroll(&GBA);
		RD1Engine::theGame->mgrDoors->SaveDoors(RD1Engine::theGame->mainRoom->Area);
		RD1Engine::theGame->mainRoom->LoadHeader(HeaderOffset);
		MemFile::currentFile->Save();

	}
	//fp = NULL;
	return 0;
}



int LoadROM() {
	RD1Engine::theGame->LoadAreaTable();
	RD1Engine::theGame->GetArrays();
	for (int i = 0; i < 4; i++)
	{
		SetUpCombos(i);
	}

	//GlobalVars::gblVars->AppPath
		

	return 0;
}









extern sCombo cMusic;
extern sCombo cmTileset;

void LoadScrollInfo(int s, Scroller *scroll);
void LoadScrollControls(Scroller *scroll);
extern clsUIScroll scrMapV;
extern clsUIScroll scrMapH;
extern clsUIScroll scrTSV;
extern GBAGraphics* TheVRAM;
extern nMapBuffer* GetActiveBuffer();



int             sMessage(char *messagestring);

void LoadCombos(sCombo* Combo, char *FileName, int Max = 255);

void OpenRom()
{
	char* fileLoc = new char[MAX_PATH];
	memset(fileLoc, 0, MAX_PATH);
	sprintf(fileLoc, "%s", "C:\\DHTest\\MF.gba");
	if (RD1Engine::theGame)
	{
		delete RD1Engine::theGame;
		RD1Engine::theGame = NULL;
	}
	if (GBA.ROM)
	{
		fclose(GBA.ROM);
	}
	/*
	* memset(&ofn, 0, sizeof(ofn));  ofn.lStructSize = sizeof(ofn);
	* ofn.hwndOwner = NULL;  ofn.lpstrFile = GBA.FileLoc;  ofn.nMaxFile =
	* sizeof(GBA.FileLoc);  ofn.hInstance = NULL;  ofn.lpstrFilter = "Open
	* a GBA ROM\0*.gba"; ofn.nFilterIndex = 1;  ofn.Flags=
	* OFN_FILEMUSTEXIST;   int bRes;   bRes = GetOpenFileName(&ofn);
	*/
	{
		GBA.ReturnFileName(hGlobal, hwndMain(), "Open a GBA ROM\0*.gba", fileLoc, MAX_PATH, 1);
	}
	if (fileLoc[0])
	{

		sprintf(GBA.FileLoc, "%s", fileLoc);
		char prefix[3];
		GBA.REDIT = fopen(GBA.FileLoc, "r+b");
		char            ROMNAME[0x5] = { '\0' };
		fseek(GBA.REDIT, 0xAC, SEEK_SET);
		fread(ROMNAME, 1, 0x4, GBA.REDIT);
		char            c = 0;
		SupportedTitles theTitle;
		if (GameConfiguration::LiteralCompare(ROMNAME, (char*)ZeroMission::CodeName))
		{
			theTitle = SupportedTitles::titleZM;
			sprintf(prefix, "%s", "ZM");
		}
		else if (GameConfiguration::LiteralCompare(ROMNAME, (char*)MetroidFusion::CodeName))
		{

			theTitle = SupportedTitles::titleMF;
			sprintf(prefix, "%s", "MF");
		}
		else
		{
			fclose(GBA.REDIT);

			Logger::log->LogIt(Logger::ERRORZ, "UNSUPPORTED ROM");
			throw "UNSUPPORTED ROM";
			currentRomType = -1;
		}
		if (GBA.REDIT)
			fclose(GBA.REDIT);

		if (GameConfiguration::mainCFG)
		{
			delete GameConfiguration::mainCFG;
		}
		currentRomType = (int)theTitle;
		GameConfiguration::mainCFG = new GameConfiguration(currentRomType);
		EnableMenuItem(GetMenu(hwndMain()), 0, MF_BYPOSITION | MF_GRAYED);
		GBA.ROM = fopen(GBA.FileLoc, "r+b");

		char buffer[512] = { 0 };
		unsigned long tmp = 0;


		char filepath2[1024] = { 0 };
		if ((strlen(filepath2) + 20) >= 1024) {
			MessageBox(0, "Please move your file someplace else as the path is too long.", "Error.", MB_OK);
			return ;
		}
		if (!(int)theTitle) sprintf(filepath2, "%s\\SpriteList.txt", GlobalVars::gblVars->AppPath);
		if ((int)theTitle) sprintf(filepath2, "%s\\MF_oam.txt", GlobalVars::gblVars->AppPath);
		GlobalVars::gblVars->frameTables = new OamFrameTable((int)theTitle, filepath2);


		MemFile::currentFile = new MemFile(GBA.FileLoc);
		RD1Engine::theGame = new RD1Engine(theTitle, GlobalVars::gblVars->frameTables, GlobalVars::gblVars->BGImage, GlobalVars::gblVars->TileImage, GlobalVars::gblVars->imgTileset);



		char filepath[2048];

		// char que
		sprintf(filepath, "%s\\%s\\Music.txt", GlobalVars::gblVars->AppPath, prefix);


		LoadCombos(&cMusic, filepath, 255);

		// char que
		sprintf(filepath, "%s\\%s\\TilesetNames.txt", GlobalVars::gblVars->AppPath, prefix);
		LoadCombos(&cmTileset, filepath, 255);
		LoadROM();
		RD1Engine::theGame->mgrDoors->GetDoorArray(GBA.ROM);
		RD1Engine::theGame->mgrScrolls->GetScrollArray(GBA.ROM);

		EnableConnections();
		Combos[cClip].Enable();
		Combos[cSpriteSet].Enable();
		Combos[cRoom].Enable();
		Combos[cArea].Enable();

		if (theTitle == SupportedTitles::titleZM)
		{
			if (DefArea > 7 || DefArea < 0)
			{
				Combos[cArea].SetListIndex(0);
			}
			else
			{
				Combos[cArea].SetListIndex(DefArea);
			}


		}
		else if (theTitle == SupportedTitles::titleMF)
		{
			if (DefArea > 10 || DefArea < 0)
			{
				Combos[cArea].SetListIndex(0);
			}
			else
			{
				Combos[cArea].SetListIndex(DefArea);
			}

		}
		Combos[cArea].SetListIndex(0);
		//GlobalVars::gblVars->StatEditor->Switch();
 		GlobalVars::gblVars->TextEditor->Create(currentRomType, hGlobal, TextPicProc);
		unsigned long BIC = 0;
		MemFile::currentFile->seek(0x7c0430);
		MemFile::currentFile->fread(&BIC, 1, 4, GBA.ROM);
		if (BIC == 0x78004817)
		{
			BGiInstall = 1;
		}
		SendMessage(GlobalVars::gblVars->frameControls, WM_COMMAND, 0x000103ee, 0);
		UiState::stateManager->UpdateMapObjectWindow();
		delete[] fileLoc;
	}
}
//Loads the prelim data
//void OpenRom_old()
//{
//	char* fileLoc = new char[MAX_PATH];
//	memset(fileLoc, 0, MAX_PATH);
//	sprintf(fileLoc, "%s","C:\\FusionLessonInEvolution\\MetroidHacking\\testRom.gba");
//	if (RD1Engine::theGame)
//	{
//		delete RD1Engine::theGame;
//		RD1Engine::theGame = NULL;
//	}
//	if (GBA.ROM)
//	{
//		fclose(GBA.ROM);
//	}
//	/*
//	* memset(&ofn, 0, sizeof(ofn));  ofn.lStructSize = sizeof(ofn);
//	* ofn.hwndOwner = NULL;  ofn.lpstrFile = GBA.FileLoc;  ofn.nMaxFile =
//	* sizeof(GBA.FileLoc);  ofn.hInstance = NULL;  ofn.lpstrFilter = "Open
//	* a GBA ROM\0*.gba"; ofn.nFilterIndex = 1;  ofn.Flags=
//	* OFN_FILEMUSTEXIST;   int bRes;   bRes = GetOpenFileName(&ofn);
//	*/
//	/*{
//		GBA.ReturnFileName(hGlobal, hwndMain(), "Open a GBA ROM\0*.gba", fileLoc, MAX_PATH, 1);
//	}*/
//	if (fileLoc[0])
//	{
//	
//		sprintf(GBA.FileLoc, "%s", fileLoc);
//		GBA.REDIT = fopen(GBA.FileLoc, "r+b");
//		char            ROMNAME[0x5] = { '\0' };
//		char            MZMFILE[0x5] = { '\0' };
//		char            MFFILE[0x5] = { '\0' };
//
//		strcpy(MZMFILE, "BMXE");
//		strcpy(MFFILE, "AMTE");
//		fseek(GBA.REDIT, 0xAC,SEEK_SET);
//		fread(ROMNAME, 1, 0x4, GBA.REDIT);
//		char            c = 0;
//
//
//		if ((ROMNAME[0] == MZMFILE[0]) && (ROMNAME[1] == MZMFILE[1]) && (ROMNAME[2] == MZMFILE[2]) && (ROMNAME[3] == MZMFILE[3]) &&
//			(ROMNAME[4] == MZMFILE[4]))
//		{ // everthing went ok!
//			currentRomType = 0;
//			GameConfiguration::mainCFG = new GameConfiguration(0);
//			GBA.ROM = fopen(GBA.FileLoc, "r+b");
//			MemFile::currentFile = new MemFile(GBA.FileLoc);
//			RD1Engine::theGame = new RD1Engine(SupportedTitles::titleZM);
//			currentRomType = 0;
//			char quick[2048];
//
//			// char que
//			sprintf(quick, "%s\\ZM\\Music.txt", GlobalVars::gblVars->AppPath);
//
//
//			LoadCombos(&cMusic, quick, 255);
//
//
//
//
//
//			// char que
//			sprintf(quick, "%s\\ZM\\TilesetNames.txt", GlobalVars::gblVars->AppPath);
//			LoadCombos(&cmTileset, quick, 255);
//
//			LoadROM();
//			RD1Engine::theGame->mgrDoors->GetDoorArray(GBA.ROM);
//			RD1Engine::theGame->mgrScrolls->GetScrollArray(GBA.ROM);
//			InitPosArray();
//			EnableConnections();
//			Combos[cClip].Enable();
//			Combos[cSpriteSet].Enable();
//			Combos[cRoom].Enable();
//			Combos[cArea].Enable();
//			if (DefArea > 7 || DefArea < 0)
//				Combos[cArea].SetListIndex(0);
//			else
//				Combos[cArea].SetListIndex(DefArea);
//			MiniMapClass::miniMapEditor->Create();
//			MiniMapClass::miniMapEditor->cboMArea.SetListIndex(0);
//			MiniMapClass::miniMapEditor->DecompressMap();
//			MiniMapClass::miniMapEditor->DrawMap();
//			MiniMapClass::miniMapEditor->MFMap.Disable();
//			GlobalVars::gblVars->StatEditor->Switch();
//			GlobalVars::gblVars->TextEditor->Create(currentRomType, hGlobal, TextPicProc);
//			unsigned long BIC = 0;
//			MemFile::currentFile->seek(0x7c0430);
//			MemFile::currentFile->fread(&BIC, 1, 4, GBA.ROM);
//			if (BIC == 0x78004817)
//			{
//				BGiInstall = 1;
//			}
//			SendMessage(hwndMain(), WM_COMMAND, 0x000103ee, 0);
//			UiState::stateManager->UpdateMapObjectWindow();
//			delete[] fileLoc;
//
//		}
//		else if ((ROMNAME[0] == MFFILE[0]) && (ROMNAME[1] == MFFILE[1]) && (ROMNAME[2] == MFFILE[2]) && (ROMNAME[3] == MFFILE[3]) &&
//			(ROMNAME[4] == MFFILE[4]))
//		{
//			currentRomType = 1;
//			sprintf(GBA.FileLoc, "%s", fileLoc);
//			MemFile::currentFile = new MemFile(GBA.FileLoc);
//			RD1Engine::theGame = new RD1Engine(SupportedTitles::titleMF);
//			GBA.ROM = fopen(GBA.FileLoc, "r+b");
//			currentRomType = 1;
//			GameConfiguration::mainCFG = new GameConfiguration(1);
//			EnableMenuItem(GetMenu(hwndMain()), 0, MF_BYPOSITION | MF_GRAYED);
//			char quick[2048];
//
//			sprintf(quick, "%s\\MF\\Music.txt", GlobalVars::gblVars->AppPath);
//
//			LoadCombos(&cMusic, quick, 255);
//
//
//
//			sprintf(quick, "%s\\MF\\TilesetNames.txt", GlobalVars::gblVars->AppPath);
//
//			LoadCombos(&cmTileset, quick, 255);
//			LoadROM();
//			RD1Engine::theGame->mgrDoors->GetDoorArray(GBA.ROM);
//			RD1Engine::theGame->mgrScrolls->GetScrollArray(GBA.ROM);
//
//			EnableConnections();
//			Combos[cClip].Enable();
//			Combos[cSpriteSet].Enable();
//			Combos[cRoom].Enable();
//			Combos[cArea].Enable();
//			if (DefArea > 10 || DefArea < 0)
//				Combos[cArea].SetListIndex(0);
//			else
//				Combos[cArea].SetListIndex(DefArea);
//			Combos[cArea].SetListIndex(0);
//			MiniMapClass::miniMapEditor->Create();
//			MiniMapClass::miniMapEditor->cboMArea.SetListIndex(0);
//			MiniMapClass::miniMapEditor->DecompressMap();
//			MiniMapClass::miniMapEditor->DrawMap();
//			MiniMapClass::miniMapEditor->MFMap.Enable();
//			GlobalVars::gblVars->TextEditor->Create(currentRomType, hGlobal, TextPicProc);
//			GlobalVars::gblVars->StatEditor->Switch();
//			unsigned long BIC=0;
//			MemFile::currentFile->seek(0x7c0430);
//			MemFile::currentFile->fread(&BIC, 1, 4, GBA.ROM);
//			if (BIC == 0x78004819)
//			{
//				BGiInstall = 1;
//			}
//			SendMessage(hwndMain(), WM_COMMAND, 0x000103ee, 0);
//			UiState::stateManager->UpdateMapObjectWindow();
//
//		}
//		else
//		{
//			sMessage("This is not the correct file.");
//			fclose(GBA.REDIT);
//		
//			currentRomType = -1;
//		}
//		if (GBA.REDIT)
//			fclose(GBA.REDIT);
//	}
//}
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
#include "..\MemFile\MemFile.h"
#include "Logger.h"
//Get the Major Arrays
void DumpLayers();
bool LiteralCompare(char* a, char* b);

/*int LoadRoom(){
int Room = */
void CalculateMapScrolls(int width, int height)
{
	RECT rcMap;
	GetWindowRect(UiState::stateManager->GetMapWindow(), &rcMap);
	int newHorizMax = width - ((rcMap.right - rcMap.left)/16);
	int newVertMax= height - ((rcMap.bottom - rcMap.top)/16);		// maximu

	int horizIndex = MapHorizScroll->GetIndex();
	int vertIndex = MapVertScroll->GetIndex();


	MapHorizScroll->SetMax(newHorizMax);//BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X/16;	// maximum H scroll
	MapVertScroll->SetMax(newVertMax);

	MapVertScroll->ChangeScrollbars();
	MapHorizScroll->ChangeScrollbars();
}
int DrawLevel() {

	int i = 0;
	RoomClass* room = RD1Engine::theGame->mainRoom;
	nMapBuffer* buffForeground = room->mapMgr->GetLayer(MapManager::ForeGround);
	nMapBuffer* buffLeveldata = room->mapMgr->GetLayer(MapManager::LevelData);
	nMapBuffer* buffBacklayer = room->mapMgr->GetLayer(MapManager::Backlayer);
	nMapBuffer* buffClipdata = room->mapMgr->GetLayer(MapManager::Clipdata);
	
	//for(i=0;i<2;i++){
	//   GlobalVars::gblVars->checkBoxViewF.value(i);
	//	GlobalVars::gblVars->checkBoxViewL.value(i);
	//	GlobalVars::gblVars->checkBoxViewB.value(i);
	//RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
	//}
	CurMapWidth = buffLeveldata->X;
	CurMapHeight = buffLeveldata->Y;
	CalculateMapScrolls(CurMapWidth, CurMapHeight);
	room->currentHorizScroll = 0;
	room->currentVertScroll = 0;


	return 0;
}





extern sCombo cMusic;
extern sCombo cmTileset;

extern clsUIScroll scrMapV;
extern clsUIScroll scrMapH;
extern clsUIScroll scrTSV;
extern GBAGraphics* TheVRAM;
extern nMapBuffer* GetActiveBuffer();

int             sMessage(char *messagestring);

void LoadCombos(sCombo* Combo, char *FileName, int Max = 255);

void OpenRom()
{
	char* fileLoc = new char[512];
	memset(fileLoc, 0, 512);
	sprintf(fileLoc, "%s", "C:\\DHTest\\MF.gba\0");
	if (RD1Engine::theGame)
	{
		delete RD1Engine::theGame;
		RD1Engine::theGame = NULL;
	}
	if (GBA.ROM)
	{
		fclose(GBA.ROM);
	}
	
	
		GBA.ReturnFileName(hGlobal, NULL, "Open a GBA ROM\0*.gba\0", fileLoc, 512, 1);
	
	if (fileLoc[0])
	{
		DataContainer* tmp2;
		sprintf(GBA.FileLoc, "%s", fileLoc);
		char prefix[4] = { 0 };
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
		else if (GameConfiguration::LiteralCompare(ROMNAME, (char*)WarioLand::CodeName))
		{
			theTitle = SupportedTitles::titleWL;
			sprintf(prefix, "%s", "WL");
		}
		else
		{
			fclose(GBA.REDIT);

			Logger::log->LogIt(Logger::ERRORZ, "UNSUPPORTED ROM");
		
			currentRomType = -1;
			return;
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
		

		GlobalVars::gblVars->frameTables = new OamFrameTable(theTitle, GlobalVars::gblVars->AppPath);

		tmp2 = GameConfiguration::mainCFG->GetDataContainer("ZoomStates")  ;
		memcpy(&GlobalVars::gblVars->zoomLevel, &tmp2->DataArray[0], sizeof(float));
		float level = (float)GlobalVars::gblVars->zoomLevel;
		MemFile::currentFile = new MemFile(GBA.FileLoc, 0x800000);
		if (!MemFile::currentFile->ValidFileSize())
		{
			delete MemFile::currentFile;
			return; 
		}
		RD1Engine::theGame = new RD1Engine(theTitle, GlobalVars::gblVars->frameTables, &GlobalVars::gblVars->BGImage, GlobalVars::gblVars->TileImage, GlobalVars::gblVars->imgTileset);

		char filepath[1024] = { 0 };

		// char que
		sprintf(filepath, "%s\\%s\\Music.txt", GlobalVars::gblVars->AppPath, prefix);


		//fixitmattLoadCombos(&cMusic, filepath, 255);

		// char que
	
	
		RD1Engine::theGame->LoadROM();





		SetUpCombos();
		EnableConnections();
		cboClipData.Enable();
		comboSpriteSet.Enable();
		comboRoom.Enable();
		comboArea.Enable();

		if (theTitle == SupportedTitles::titleZM)
		{
			if (DefArea > 7 || DefArea < 0)
			{
				comboArea.SetListIndex(0);
			}
			else
			{
				comboArea.SetListIndex(DefArea);
			}

		}
		else if (theTitle == SupportedTitles::titleMF)
		{
			if (DefArea > 10 || DefArea < 0)
			{
				comboArea.SetListIndex(0);
			}
			else
			{
				comboArea.SetListIndex(DefArea);
			}

		}
		comboArea.SetListIndex(0);
		//GlobalVars::gblVars->StatEditor->Switch();
 //		GlobalVars::gblVars->TextEditor->Create(currentRomType, hGlobal, TextPicProc);
		unsigned long BIC = 0;
		MemFile::currentFile->seek(0x7c0430);
		MemFile::currentFile->fread(&BIC, 1, 4);
		if (BIC == 0x78004817)
		{
			BGiInstall = 1;
		}
		SendMessage(GlobalVars::gblVars->frameControls, WM_COMMAND, 0x000103ee, 0);
		UiState::stateManager->UpdateMapObjectWindow();
		delete[] fileLoc;
	}
}

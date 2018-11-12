#include "MainHeader.h"
#include "sChecks.h"
#include "mmD.h"
#include "cClipboard.h"
#include "clsScroll.h"
#include "GlobalVars.h"
#include "GBAMethods.h"
#include "GBAGraphics.h"
#include "cSSE.h"
#include "cOAMManager.h"
#include "globals.h"
#include "DoorManager.h"
#include "TilesetManager.h"
#include "SpriteObjectManager.h"
#include "MapEditingState.h"
#include "cOAMManager.h"
#include "resource.h"
#include "clsRoomScrolls.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "MainWindow.h"
#include "BaseGame.h"
#include "RoomClass.h"
#include "LeakFinder.h"
#include <commctrl.h>
#include "fraMainProc.h"
#pragma comment(lib, "comctl32.lib")
HWND hTabControl; // tab control handle
HWND hCurrentTab; // tab dialog handle
extern sCombo cMusic;
extern sCombo cmTileset;
cClipboard  ClipBoard;

clsUIScroll scrMapV;
clsUIScroll scrMapH;
clsUIScroll scrTSV;
GBAGraphics* TheVRAM;
nMapBuffer* GetActiveBuffer();

int             i;
#define theTimer 2242443242
bool UpdateSprites()
{

	bool updateWindow = false;
	if (!RD1Engine::theGame)
	{
		return false;
	}
	vector<FrameManager*> *sprites = &RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites;
	for (int spriteCounter = 0; spriteCounter < sprites->size(); spriteCounter++)
	{
		Frame* animatedFrame = sprites->at(spriteCounter)->GetAnimatedFrame();
		if (animatedFrame != NULL)
		{
			if (sprites->at(spriteCounter)->Animate())
			{
				updateWindow = true;
			}
		}
	}
	return false;
}

bool RefreshTileset()
{

	if (RD1Engine::theGame && RD1Engine::theGame->mgrTileset && RD1Engine::theGame->mgrTileset->animTiles)
	{
		if (RD1Engine::theGame->mgrTileset->animTiles->Animate())
		{

			RD1Engine::theGame->mgrTileset->Render(GlobalVars::gblVars->imgTileset);
		}
	}
	return true;
}

void UpdateTileset()
{

	if (RD1Engine::theGame && RD1Engine::theGame->mgrTileset)
	{
		RD1Engine::theGame->mgrTileset->GetTileset(GlobalVars::gblVars->imgTileset, RD1Engine::theGame->mainRoom->Area, RD1Engine::theGame->mainRoom->roomHeader.bTileset, RD1Engine::theGame->mainRoom->roomHeader.lBg3);
		RefreshTileset();
	}
}
int             sMessage(char *messagestring)
{
	MessageBox(hwndMain(), messagestring, NULL, MB_OK);
	return 0;
}
bool inited = false;
HWND hwndMain()
{
	return UiState::stateManager->GetWindow();

}

void hwndMain(HWND window)
{
	UiState::stateManager->SetWindow(window);
}

void TabResize()
{
	RECT tabControl;
	RECT currentTabWindow;
	RECT tilesetWindow;
	RECT mapWindow;
	RECT mainRect;

	GetWindowRect(hTabControl, &tabControl);
	GetWindowRect(hCurrentTab, &currentTabWindow);
	GetWindowRect(UiState::stateManager->GetMapWindow(), &mapWindow);
	GetWindowRect(UiState::stateManager->GetTilesetWindow(), &tilesetWindow);
	GetWindowRect(UiState::stateManager->GetWindow(), &mainRect);



	MoveWindow(hTabControl, 0, 0, currentTabWindow.right - currentTabWindow.left + 16, currentTabWindow.bottom - currentTabWindow.top, 1);
	MoveWindow(hCurrentTab, 0, 32, currentTabWindow.right - currentTabWindow.left, currentTabWindow.bottom - currentTabWindow.top, 1);
	GetWindowRect(hTabControl, &tabControl);

	GetWindowRect(hCurrentTab, &currentTabWindow);
	GetWindowRect(hTabControl, &tabControl);
	GetWindowRect(UiState::stateManager->GetMapWindow(), &mapWindow);
	MoveWindow(UiState::stateManager->GetTilesetWindow(), 8, tabControl.bottom - tabControl.top + 8, tilesetWindow.right - tilesetWindow.left, tilesetWindow.bottom - tilesetWindow.top, 1);
	MoveWindow(UiState::stateManager->GetMapWindow(), tabControl.right + 16, 8, mapWindow.right - mapWindow.left, mapWindow.bottom - mapWindow.top, 1);

	GetWindowRect(UiState::stateManager->GetTilesetWindow(), &tilesetWindow);

	//MoveWindow(UiState::stateManager->GetWindow(), mainRect.left, mainRect.top, tilesetWindow.bottom, mapWindow.right+32, 1);
	UiState::stateManager->ResizeMap(hTabControl);
}

void LoadCombos(sCombo* Combo, char *FileName, int Max = 255)
{

	FILE *          text = fopen(FileName, "rt");
	char buffer[1024];
	Combo->Clear();

	if (text)
	{


		while (!feof(text))
		{
			fgets(buffer, 256, text);

			if ((buffer[strlen(buffer)] == 0xD) || (buffer[strlen(buffer)] == 0xA))
				buffer[strlen(buffer)] = 0;
			if ((buffer[strlen(buffer) - 1] == 0xD) || (buffer[strlen(buffer) - 1] == 0xA))
				buffer[strlen(buffer) - 1] = 0;

			Combo->Additem(buffer);
		}




		fclose(text);

	}
	else
	{
		int i = 0;
		for (i = 0; i < Max; i++) {

			sprintf(buffer, "%X", i);
			Combo->Additem(buffer);
		}
	}
	Combo->SetListIndex(0);

}


///Returns true if we processed anything controls.
bool ProcessControls(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	FILE           *fp = NULL;
	int area;
	int             sprch = 0;
	char            cBuf[1024] = { 0 };
	int             i = 0;
	int             loadit = 0;
	long            LevelCounter = 0;
	unsigned long             BIC = 0;
	HWND            debug = NULL;
	int someval = 0;
	RD1Engine* mainGame = NULL;
	if (RD1Engine::theGame)
	{
		mainGame = RD1Engine::theGame;
	}

	switch (LOWORD(wParam))
	{
	case chkViewF:
		GlobalVars::gblVars->ViewForeground = GlobalVars::gblVars->checkBoxViewF.value() == BST_CHECKED;
		mainGame->DrawStatus.BG0 = GlobalVars::gblVars->ViewForeground;
		mainGame->DrawStatus.dirty = true;
		InvalidateRect(hwnd, 0, false);
		return true;
		break;
	case chkViewL:
		GlobalVars::gblVars->ViewLevel = GlobalVars::gblVars->checkBoxViewL.value() == BST_CHECKED;
		mainGame->DrawStatus.BG1 = GlobalVars::gblVars->ViewLevel;
		mainGame->DrawStatus.dirty = true;
		InvalidateRect(hwnd, 0, false);
		return true;
		break;
	case chkViewB:
		GlobalVars::gblVars->ViewBacklayer = GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED;
		mainGame->DrawStatus.BG2 = GlobalVars::gblVars->ViewBacklayer;
		mainGame->DrawStatus.dirty = true;
		InvalidateRect(hwnd, 0, false);
		return true;
		break;
	case chkHS:
		//GlobalVars::gblVars->ViewBacklayer = GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED;
		return true;
		break;
	case ID_DAA:
		RD1Engine::theGame->DumpAreaAsImage("heyman.bmp",  GlobalVars::gblVars->imgTileset, &SpriteImage);
		return true;
		break;
	}
	return false;
}



int  HandleDetections(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	FILE           *fp = NULL;
	int area;
	int             sprch = 0;
	char            cBuf[1024] = { 0 };
	int             i = 0;
	int             loadit = 0;
	long            LevelCounter = 0;
	unsigned long             BIC = 0;
	HWND            debug = NULL;
	int someval = 0;
	RD1Engine* mainGame = NULL;
	if (RD1Engine::theGame)
	{
		mainGame = RD1Engine::theGame;
	}
	if (ProcessControls(hwnd, message, wParam, lParam))
	{
		return 0;
	}
	switch (LOWORD(wParam))
	{
	case mnuEB:
		if (currentRomType == -1)
			return 0;
		if (GBA.ReturnFileName(hGlobal, hwnd, "Please select a bitmap file\0*.bmp\0", cBuf, 512, 0)) {

			fp = fopen(cBuf, "w+");
			if (fp)
			{
				RD1Engine::theGame->ThisBackBuffer.SaveToFile(fp);
				fclose(fp);
			}
			else
			{
				MessageBox(0, "Bad File Name", "Error", MB_OK);
			}
		}
		break;
	case mnuInsert:
		ShowWindow(GlobalVars::gblVars->BGi->me, 1);
		break;


	case mnuIPSP:
		if (currentRomType == -1)
			return 0;
		GBA.ReturnFileName("Please Select an Apply File to apply\0*.ips", cBuf, 1024);
		ApplyIPS(cBuf, GBA.FileLoc);
		break;



	case mnuClipboard:
		if (currentRomType == -1)
			return 0;
		ShowWindow(ClipBoard.me, SW_SHOW);
		break;

	case mnuSOE:
		if (currentRomType == -1)
			return 0;

		if (cOAMEdit::OamEditor->_oamWindow)
		{
			DestroyWindow(cOAMEdit::OamEditor->_oamWindow);

		}
		if (cOAMEdit::OamEditor == NULL)
		{
			cOAMEdit::OamEditor = new cOAMEdit();
			cOAMEdit::OamEditor->Create();
		}
		CreateDialog(hGlobal, MAKEINTRESOURCE(frmOAM), 0, OAMProc);
		ShowWindow(cOAMEdit::OamEditor->_oamWindow, SW_SHOW);
		break;

	case mnuSSE:
		if (currentRomType == -1)
			return 0;
		CreateDialog(hGlobal, MAKEINTRESOURCE(fraSSE), 0, cSSEProc);
		ShowWindow(cSSE::SpriteSet->me, SW_SHOW);
		break;
	case mnuTSA:
		if (currentRomType == -1)
			return 0;
		ShowWindow(MyTSAEditor.tMain, SW_SHOW);
		break;
	case mnuHeader:
		if (currentRomType == -1)
			return 0;
		if (hwndHeader == NULL)
		{
			CreateDialog(hGlobal, MAKEINTRESOURCE(frmHeader), 0, HeaderProced);

		}
		if (hwndHeader) {
			LoadHeaderControls();
			ShowWindow(hwndHeader, SW_SHOW);
		}

		break;
	case mnuRS:
		if (currentRomType == -1)
			return 0;
		if (ExtendedOptWND == NULL)
		{
			CreateDialog(hGlobal, MAKEINTRESOURCE(frmRoomStuff), 0, ExtendedProc);
		}

		ShowWindow(ExtendedOptWND, SW_SHOW);
		LoadExtensions();
		LayerCombo.SetListIndex(1);
		SendMessage(ExtendedOptWND, WM_COMMAND, 0x000103eb, 0);

		break;
	case IDCANCEL:
		EndDialog(hwnd, 0);
		EndDialog(DoorWin, 0);
		EndDialog(hwndResize, 0);
		break;
	case mnuTE:
		if (currentRomType == -1)
			return 0;
		if (!GlobalVars::gblVars->TextEditor->tMain)
		{
			CreateDialog(hGlobal, MAKEINTRESOURCE(frmTE), 0, TextEditorProc);
		}
		ShowWindow(GlobalVars::gblVars->TextEditor->tMain, SW_SHOW);
		break;
	case mnuSE:
		if (currentRomType == -1)
			return 0;
		if (!GlobalVars::gblVars->StatEditor->me) {
			CreateDialog(hGlobal, MAKEINTRESOURCE(fraStat), 0, StatEditorProc);
		}

		ShowWindow(GlobalVars::gblVars->StatEditor->me, SW_SHOW);
		break;
	case mnuOpen:
		OpenRom();
		break;
	case mnuMini:
		if (currentRomType == -1)
			return 0;
		if (!hwndMM) {
			CreateDialog(hGlobal, MAKEINTRESOURCE(frmMiniMap), 0, MiniProc);

		}
		ShowWindow(hwndMM, SW_SHOW);
		break;

	case ID_GENERALTOOLS_SWITCHGUI:
		MessageBox(0, "SWITCHING GUIS ALL DATA WILL BE LOST, CLOSING APPLICATION", "WARNING", MB_OK);
		i = GameConfiguration::mainCFG->GetDataContainer("UIState")->Value;

		if (i)
		{
			i = 0;
		}
		else if (!i)
		{
			i = 1;
		}

		GameConfiguration::mainCFG->GetDataContainer("UIState")->Value = i;
		EndDialog(hwnd, 0);

		break;
	case mnuRR:
		ShowWindow(hwndResize, SW_SHOW);
		break;


	}


	return 0;
}
HWND tabs[6];
; BOOL CALLBACK   DialogProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	int blah;
	char            FilePath[1024] =
	{
	   0
	};
	unsigned short             wndrs[3] =
	{
	   hsbC1, hsbC2, hsbC3
	};
	switch (message)
	{

	case WM_INITDIALOG:
		for (int h = 0; h < 5; h++)
		{
			tabs[h] = NULL;
		}

		if (UiState::stateManager->GetWindowState() == WindowState::EXPERT)
		{
			INITCOMMONCONTROLSEX commCon;
			commCon.dwSize = sizeof(INITCOMMONCONTROLSEX);
			commCon.dwICC = ICC_TAB_CLASSES;
			InitCommonControlsEx(&commCon); // have to run this to use tab control
			hTabControl = GetDlgItem(hwnd, tabMain);

			TCITEM tcItem;
			tcItem.mask = TCIF_TEXT; // I'm only having text on the tab

									 //Tab 1
			tcItem.pszText = "Main";
			TabCtrl_InsertItem(hTabControl, 0, &tcItem);

			//Tab 2
			tcItem.pszText = "Map Data";
			TabCtrl_InsertItem(hTabControl, 1, &tcItem);

			//Tab 3
			tcItem.pszText = "Door Data";
			TabCtrl_InsertItem(hTabControl, 2, &tcItem);

			//Tab 4
			tcItem.pszText = "Scroll Data";
			TabCtrl_InsertItem(hTabControl, 3, &tcItem);


			//Tab 5
			tcItem.pszText = "Sprite Data";
			TabCtrl_InsertItem(hTabControl, 4, &tcItem);
			TabCtrl_SetCurSel(hTabControl, 0);



			tabs[0] = CreateDialog(hGlobal, MAKEINTRESOURCE(fraMain), hTabControl, fraMainProc); // Setting dialog to tab one by default

			tabs[1] = CreateDialog(hGlobal, MAKEINTRESOURCE(frmMapData), hTabControl, MapDataProc);

			tabs[2] = CreateDialog(hGlobal, MAKEINTRESOURCE(frmDoor), hTabControl, DwProc);

			tabs[3] = CreateDialog(hGlobal, MAKEINTRESOURCE(frmScroll), hTabControl, ScrollWndProc);

			tabs[4] = CreateDialog(hGlobal, MAKEINTRESOURCE(fraSS), hTabControl, SSProc);

			hCurrentTab = tabs[0];

			//SendMessage(hTabControl, )


			ShowWindow(hCurrentTab, 1);

		}
		ClearGlobals();


		InitPosArray();
		hwndMain(hwnd);
		UiState::stateManager->InitTileset();
		UiState::stateManager->InitMap();
		GetCurrentDirectory(1023, GlobalVars::gblVars->AppPath);
		strcpy(FilePath, GlobalVars::gblVars->AppPath);
		strcat(FilePath, "\\DH.cfg");
		LoadConfigFile(FilePath);
		DM[0] = DM[1] = 0;

		CurObjT = 0;
		CurObjNo = 0;



		clrIndex = 0;
		GlobalVars::gblVars->imgTileset = new Image();
		GlobalVars::gblVars->imgTileset->Create(512, 1024);


		chkDoTrans.value(1);
		GlobalVars::gblVars->checkBoxshowmap.value(1);
		GlobalVars::gblVars->checkBoxshowtileset.value(1);





		//CreateDialog(hGlobal, MAKEINTRESOURCE(frmSceneryEditor), 0, SceneProc);

		//

		//CreateDialog(hGlobal, MAKEINTRESOURCE(fraTBE), 0, TSAProc);

		//CreateDialog(hGlobal, MAKEINTRESOURCE(frmClipboard), 0, ClipBoardProc);
		//CreateDialog(hGlobal, MAKEINTRESOURCE(fraBGI), 0, BGiProc);
		//CreateDialog(hGlobal, MAKEINTRESOURCE(frmLPE), 0, LPProc);


		/*ShowWindow(ScrollWIn, SW_HIDE);
		ShowWindow(DoorWin, SW_HIDE);*/


		// SetParent(Main, DoorWin );
		nMaxHScroll[sTileset] = (256 - nDisplayWidth[sTileset] + 7) / 8; // maximum 
																		 // H
																		 // scroll

		nMaxVScroll[sTileset] = (256 - nDisplayHeight[sTileset] + 7) / 8;

		ChangeScrollbars(hwnd, vsbScroll, sTileset);
		ChangeScrollbars(hwnd, vsbMap, sVMap);
		blah = GetLastError();
		ChangeScrollbars(hwnd, hsbMap, sHMap);

		for (i = 0; i < 3; i++)
		{
			GlobalVars::gblVars->scrColors[i].create(GetDlgItem(hwndLPE, wndrs[i]), 0, 255);
			GlobalVars::gblVars->scrColors[i].ChangeScrollbars();
			// SetScrollPos( UiState::stateManager->GetTilesetWindow(), SB_HORZ, 0, 1 );

		}

		// etArrays();
		MPToUse = 0;
		mpTileset.Width = 1;
		mpTileset.Height = 1;
		mpTileset.sX = mpTileset.eX = mpTileset.cX = 0;
		mpTileset.sY = mpTileset.eY = mpTileset.cY = 0;
		mpMap.Width = 1;
		mpMap.Height = 1;
		mpMap.sX = mpMap.eX = mpMap.cX = 0;
		mpMap.sY = mpMap.eY = mpMap.cY = 0;



		UiState::stateManager->ShowObj();

		TabResize();

		SetTimer(hwnd, theTimer, 15, (TIMERPROC)NULL);
		SetTimer(hwnd, theTimer + 1, 90, (TIMERPROC)NULL);
		//LoadTrans("[MainMenu]", 0, hwnd);
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE: // ******* message sent because someone changed the tab selection (clicked on another tab)
		{
			ShowWindow(hCurrentTab, 0); // we don't want the current tab, kill it
			switch (TabCtrl_GetCurSel(hTabControl))
			{
			case 0:
				if (!tabs[0])
				{
				}
				hCurrentTab = tabs[0];
				break;
			case 1:
				if (!tabs[1])
				{
				}
				hCurrentTab = tabs[1];
				break;
			case 2:
				if (!tabs[2])
				{
				}
				hCurrentTab = tabs[2];
				break;
			case 3:
				if (!tabs[3])
				{
				}
				hCurrentTab = tabs[3];
				break;
			case 4:
				if (!tabs[4])
				{
				}
				hCurrentTab = tabs[4];
				break;
			}
			ShowWindow(hCurrentTab, 1);
			//Resize 
			TabResize();
			return TRUE;
		}//End of case
		}//End of Switch
		return TRUE;
		break;
	case WM_TIMER:
		//Update sprite animations and tileset
		//	TilesetManager::
		if (!RD1Engine::theGame || !RD1Engine::theGame->mainRoom)
		{
			break;
		}
		if (wParam == theTimer)
		{
			if (RefreshTileset()) {
				if (RD1Engine::theGame&&RD1Engine::theGame->mainRoom&&RD1Engine::theGame->mainRoom->mapMgr)
				{
					RD1Engine::theGame->DrawStatus.dirty = true;
					RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
					InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
					InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
				}
			}
		}

		if (wParam == theTimer + 1)
		{
			if (UpdateSprites()) {
				if (RD1Engine::theGame&&RD1Engine::theGame->mainRoom&&RD1Engine::theGame->mainRoom->mapMgr)
				{
					RD1Engine::theGame->DrawStatus.dirty = true;
					RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
					InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
					InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
				}
			}
		}
		break;
	case  WM_WINDOWPOSCHANGED:
		/*	ShowWindow(DoorWin, SW_SHOW);
			MoveDoorObj();
			ShowWindow(ScrollWIn, SW_SHOW);
			MoveScrollObj();*/

			/*ShowWindow(hwndSS, SW_SHOW);
			MoveSpriteObj();*/


		break;
	case BN_CLICKED:

		break;
	case WM_COMMAND:

		HandleDetections(hwnd, message, wParam, lParam);
		break;

	case WM_VSCROLL: // exact same idea, but V scroll instead of H scroll

	   // if((HWND)lParam == GetDlgItem(hwnd,vsbMap))
	   // UpdateScroll(hwnd,wParam,1,vsbMap,sVMap);

		break;

	case	WM_COPYDATA:
	case WM_COPY:
		Combos[cArea].SetListIndex(0);
		break;
	case WM_PASTE:
		Combos[cArea].SetListIndex(0);
		break;
	case WM_HSCROLL:

		// if((HWND)lParam == GetDlgItem(hwnd,hsbMap))
		// UpdateScroll(hwnd,wParam,NULL,hsbMap,sHMap);

		break;
	case WM_LBUTTONDOWN:
		// if((HWND)wParam == GetDlgItem(hwnd,picButton)) sMessage("Mouse was
		// clicked down");

		break;

	case WM_DESTROY:
		bRunApp = 0;
		if (GBA.ROM)
			fclose(GBA.ROM);
		// / myBackBuffer.Destroy();
		break;
	case WM_SIZE:
		//min heigh needs to be at least tileset

		if (lParam != 0)
		{
			if (UiState::stateManager->GetWindowState() == WindowState::SINGLE)
			{

				/*RECT tileset;
				GetWindowRect(UiState::stateManager->GetTilesetWindow(), &tileset);
				RECT map;
				GetWindowRect(UiState::stateManager->GetMapWindow(), &map);
				RECT thisWnd;
				GetWindowRect(hwnd, &thisWnd);

				if (thisWnd.bottom > tileset.bottom)
				{
					thisWnd.bottom = tileset.bottom + 16 - tileset.top;
				}



				if (thisWnd.right < map.right)
				{
					thisWnd.right = map.right + 16 - map.top;
				}

				MoveWindow(hwnd, thisWnd.left, thisWnd.top, thisWnd.right, thisWnd.bottom, true);*/
			}
		}

		break;
	}

	return 0;
}



GlobalVars* GlobalVars::gblVars;



//FILE* GBAMethods::ROM;


BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_C_EVENT)
	{
		return TRUE;
	}

	return FALSE;
}


HANDLE handle_out;
int WINAPI      WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hwndHeader = NULL;
	ExtendedOptWND = NULL;
	Logger::log = new Logger();
	LeakFinder::finder = new LeakFinder();

	//init stufff
	GlobalVars::gblVars = new GlobalVars();


	GBAGraphics::VRAM = new GBAGraphics();
	RD1Engine::theGame = new RD1Engine(SupportedTitles::titleUnsupport, NULL, NULL, NULL, NULL);
	cSSE::SpriteSet = new cSSE();



	cOAMEdit::OamEditor = new cOAMEdit();
	printf("Ayy");
	currentRomType = -1;
	hGlobal = hInstance;
	bRunApp = 1;
	DefRoom = 0;
	DefArea = 0;
	DefAddress = 0;
	Tile = 0;
	keyno = 0;

	HICON           hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON7));
	MP[0] = MP[1] = 0;

	GameConfiguration* tmpConfig = new GameConfiguration(-1);
	UiState*mgr;
	if (tmpConfig)
	{
		DataContainer* uiState = tmpConfig->GetDataContainer("UIState");

		mgr = UiState::stateManager = new UiState((WindowState)uiState->Value);
		delete tmpConfig;
		tmpConfig = 0;
	}
	else
	{
		mgr = UiState::stateManager = new UiState();
	}

	mgr->UpdateWindow();

	delete mgr;
	delete GameConfiguration::mainCFG;
	delete GlobalVars::gblVars;
	delete GBAGraphics::VRAM;
	delete MiniMapClass::miniMapEditor;
	delete cSSE::SpriteSet;


	delete cOAMEdit::OamEditor;


	return 0;

}

int             SetUpCombos(int combon)
{
	char            quick[1023] =
	{
	   0
	};
	char            cs[128] =
	{
	   0
	};

	Combos[cSpriteSet].Clear();
	Combos[cSpriteSet].Additem("0");
	Combos[cSpriteSet].Additem("1");
	Combos[cSpriteSet].Additem("2");
	Combos[cSpriteSet].SetListIndex(0);
	if (combon == cArea)
	{
		Combos[combon].Clear();
		if (currentRomType == 0)
		{
			Combos[cArea].Clear();
			Combos[cA1].Clear();
			Combos[cA2].Clear();
			Combos[cdA1].Clear();
			Combos[cdA2].Clear();

			Combos[cArea].Additem("Brinstar");
			Combos[cA1].Additem("Brinstar");
			Combos[cA2].Additem("Brinstar");
			Combos[cdA1].Additem("Brinstar");
			Combos[cdA2].Additem("Brinstar");
			Combos[cArea].Additem("Kraid");
			Combos[cA1].Additem("Kraid");
			Combos[cA2].Additem("Kraid");
			Combos[cdA1].Additem("Kraid");
			Combos[cdA2].Additem("Kraid");
			Combos[cArea].Additem("Norfair");
			Combos[cA1].Additem("Norfair");
			Combos[cA2].Additem("Norfair");
			Combos[cdA1].Additem("Norfair");
			Combos[cdA2].Additem("Norfair");
			Combos[cArea].Additem("Ridley");
			Combos[cA1].Additem("Ridley");
			Combos[cA2].Additem("Ridley");
			Combos[cdA1].Additem("Ridley");
			Combos[cdA2].Additem("Ridley");
			Combos[cArea].Additem("Tourian");
			Combos[cA1].Additem("Tourian");
			Combos[cA2].Additem("Tourian");
			Combos[cdA1].Additem("Tourian");
			Combos[cdA2].Additem("Tourian");
			Combos[cArea].Additem("Crateria");
			Combos[cA1].Additem("Crateria");
			Combos[cA2].Additem("Crateria");
			Combos[cdA1].Additem("Crateria");
			Combos[cdA2].Additem("Crateria");
			Combos[cArea].Additem("Chozodia");
			Combos[cA1].Additem("Chozodia");
			Combos[cA2].Additem("Chozodia");
			Combos[cdA1].Additem("Chozodia");
			Combos[cdA2].Additem("Chozodia");
		}
		else if (currentRomType == 1)
		{
			Combos[cArea].Clear();
			Combos[cA1].Clear();
			Combos[cA2].Clear();
			Combos[cdA1].Clear();
			Combos[cdA2].Clear();

			Combos[cArea].Additem("Main Deck");
			Combos[cA1].Additem("Main Deck");
			Combos[cA2].Additem("Main Deck");
			Combos[cdA1].Additem("Main Deck");
			Combos[cdA2].Additem("Main Deck");

			Combos[cArea].Additem("SRX");
			Combos[cA1].Additem("SRX");
			Combos[cA2].Additem("SRX");
			Combos[cdA1].Additem("SRX");
			Combos[cdA2].Additem("SRX");

			Combos[cArea].Additem("TRO");
			Combos[cA1].Additem("TRO");
			Combos[cA2].Additem("TRO");
			Combos[cdA1].Additem("TRO");
			Combos[cdA2].Additem("TRO");

			Combos[cArea].Additem("PYR");
			Combos[cA1].Additem("PYR");
			Combos[cA2].Additem("PYR");

			Combos[cArea].Additem("AQA");
			Combos[cA1].Additem("AQA");
			Combos[cA2].Additem("AQA");
			Combos[cdA1].Additem("AQA");
			Combos[cdA2].Additem("AQA");
			Combos[cArea].Additem("ARC");
			Combos[cA1].Additem("ARC");
			Combos[cA2].Additem("ARC");
			Combos[cdA1].Additem("ARC");
			Combos[cdA2].Additem("ARC");

			Combos[cArea].Additem("NOC");
			Combos[cA1].Additem("NOC");
			Combos[cA2].Additem("NOC");
			Combos[cdA1].Additem("NOC");
			Combos[cdA2].Additem("NOC");

			Combos[cArea].Additem("Debug1");
			Combos[cA1].Additem("Debug1");
			Combos[cA2].Additem("Debug1");
			Combos[cdA1].Additem("Debug1");
			Combos[cdA2].Additem("Debug1");

			Combos[cArea].Additem("Debug2");
			Combos[cA1].Additem("Debug2");
			Combos[cA2].Additem("Debug2");
			Combos[cdA1].Additem("Debug2");
			Combos[cdA2].Additem("Debug2");

			Combos[cArea].Additem("Debug3");
			Combos[cA1].Additem("Debug3");
			Combos[cA2].Additem("Debug3");
			Combos[cdA1].Additem("Debug3");
			Combos[cdA2].Additem("Debug3");
		}
	}
	else if (combon == cClip)
	{
		if (currentRomType == 0)
		{

			// char que
			sprintf(quick, "%s\\ZM\\clip.txt", GlobalVars::gblVars->AppPath);

		}
		else if (currentRomType == 1)
		{

			sprintf(quick, "%s\\MF\\clip.txt", GlobalVars::gblVars->AppPath);

		}

		LoadCombos(&Combos[cClip], quick, 255);

		/* FILE *          text = fopen(quick, "rt");

		 Combos[cClip].Clear();
		 if (text)
		 {
			while (!feof(text))
			{
			   fgets(cs, 128, text);

			   if ((cs[strlen(cs)] == 0xD) || (cs[strlen(cs)] == 0xA))
				  cs[strlen(cs)] = 0;
			   if ((cs[strlen(cs) - 1] == 0xD) || (cs[strlen(cs) - 1] == 0xA))
				  cs[strlen(cs) - 1] = 0;

			   Combos[cClip].Additem(cs);
			}
			fclose(text);
		 }
		 Combos[combon].SetListIndex(0);
		 Combos[cClip].SetListIndex(0);*/
	}
	return 0;
}

int             ClearGlobals()
{
	int             i = 0;

	currentRomType = -1; // Current ROM file

	InputHwnd = NULL;

	// wcTileset=NULL;

	//

	memset(&TilesetRect, 0, sizeof(TilesetRect));
	// TileCurs=(RectBuff)NULL;
	hwndResize = NULL;
	wndHE = NULL;
	//for (i = 0; i < 0xC0; i++)
	//   memset(&cOAMManager::mgrOAM->MFSprSize[i], 0, sizeof(unsigned long));
	movingDoors = 0;
	movingDoorIndex = -1;
	movingSprite = 0;
	movingSpriteIndex = -1;
	SceneWnd = NULL;
	SceneGraph = NULL;
	TSScene = NULL;

	//memset(&ZMSpritePos, 0, sizeof(ZMSpritePos));

	SceneryPic.Destroy();
	SceneryTiles.Destroy();

	memset(&DoorConnections, 0, sizeof(DoorConnections));
	for (i = 0; i < 15; i++)
		nHScroll[i] = nVScroll[i] = nMaxHScroll[i] = nMaxVScroll[i] = 0;
	for (i = 0; i < 11; i++)
		nDisplayWidth[i] = nDisplayHeight[i] = 0;
	memset(&GBAGraphics::VRAM->BGBuf, 0, sizeof(GBAGraphics::VRAM->BGBuf));
	//	memset(&BaseGame::theGame->mgrTileset->TSA, 0, sizeof(BaseGame::theGame->mgrTileset->TSA));

	if (GlobalVars::gblVars->imgTileset)
	{
		GlobalVars::gblVars->imgTileset->Destroy();
	}

	bRunApp = 0;
	CurDoor = 0;

	return 0;
}

int             LoadConfigFile(char *ConfigLoc)
{
	char            stringchk[256] =
	{
	   0
	};
	char            valstring[10] =
	{
	   0
	}; // Copies the value in the string

	char            area[] = "Default Area:";
	char            room[] = "Default Room:";
	char            fs[] = "Start Freespace:";
	int             i = 0;
	int             j = 0;
	int             k = 0;
	int returnVal = 0;
	DefRoom = DefArea = DefAddress = 0;
	FILE *          fp = fopen(ConfigLoc, "r");

	if (fp)
	{
		fseek(fp, 0, SEEK_SET);
		while (1)
		{
			memset(&stringchk, 0, 256);
			fgets(stringchk, 128, fp);
			if (stringchk[0] == '/' && stringchk[1] == '/')
				continue;
			i = strlen(stringchk);

			if (strcmp(stringchk, "END") == 0)
			{
				return 1;
			}
			else if (room[strlen("Default Room")] == stringchk[strlen("Default Room")])
			{
				memcpy(&valstring, &stringchk[strlen("Default Room:")], strlen(stringchk) - strlen("Default Room:"));
				sscanf(valstring, "%d", &DefRoom);

			}
			else if (fs[strlen("Start Freespace")] == stringchk[strlen("Start Freespace")])
			{
				memcpy(&valstring, &stringchk[strlen("Start Freespace:")], strlen(stringchk) - strlen("Start Freespace:"));
				sscanf(valstring, "%X", &DefAddress);

			}
			k = 0;
			for (i = 0; i < strlen("Default Area"); i++)
			{
				if (stringchk[i] != area[i])
				{
					k = 0;
					break;
				}
				k++;

			}

			if (k == strlen("Default Area"))
			{
				memcpy(&valstring, &stringchk[strlen("Default Area:")], strlen(stringchk) - strlen("Default Area:"));
				sscanf(valstring, "%d", &DefArea);
			}
			j += 10;
			if (j > 50)
			{
				returnVal = 1;
				break;
			}

		}

	}
	if (fp)
	{
		fclose(fp);
		fp = 0;
	}
	return returnVal;
}

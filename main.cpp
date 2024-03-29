
#include "MainHeader.h"
#include "MinimapClass.h"
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
#include "cOAMEdit.h"
#include <DynaRec.h>
void CalculateMapScrolls(int width, int height);
void DrawStatusFromUI();
BOOL CALLBACK	SamusProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
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
void CheckZoom(int zoomid);

//Wrapper for MessageBox
int             sMessage(char *messagestring)
{
	MessageBox(hwndMain(), messagestring, NULL, MB_OK);
	return 0;
}

void ErrorMessage(char *errString)
{//
	//MessageBox(hwndMain(), errString, "Error", MB_ICONEXCLAMATION|MB_OK);
	SetWindowText(UiState::stateManager->StatusBar, errString);
}
//Get the current window
HWND hwndMain()
{
	return UiState::stateManager->GetWindow();

}

//Set hwnd main
void hwndMain(HWND window)
{
	UiState::stateManager->SetWindow(window);
}
bool inited;


///Resizes everythning on main window
void TabResize()
{
	RECT tabControl;
	RECT currentTabWindow;
	RECT tilesetWindow;
	RECT mapWindow;
	RECT mainRect;


	//Inside of current tab
	if (!inited)
	{
		UiState::AutoRect(hTabControl, &tabControl);
		MoveWindow(hTabControl, 1, 8, tabControl.right + 1, tabControl.bottom + 20, 0);
		UiState::AutoRect(hCurrentTab, &currentTabWindow);

		inited = true;
	}

	UiState::AutoRect(hTabControl, &tabControl);
	MoveWindow(hCurrentTab, 0, 30, tabControl.right - 1, tabControl.bottom - 40, 0);

	UiState::stateManager->ResizeTileset(GetDlgItem(hwndMain(), tabMain));


	UiState::AutoRect(UiState::stateManager->GetTilesetWindow(), &tilesetWindow);
	UiState::AutoRect(hwndMain(), &tabControl);

	//Now resize map to take up 
	UiState::stateManager->ResizeMap(hTabControl);
	UiState::AutoRect(hwndMain(), &tabControl);
	int height = tabControl.bottom;
	if (height < tilesetWindow.top + tilesetWindow.bottom)
	{
		height = tilesetWindow.top + tilesetWindow.bottom + 32;
	}
	MoveWindow(hwndMain(), tabControl.left, tabControl.top, tabControl.right, height, true);
	SendMessage(UiState::stateManager->StatusBar, WM_SIZE, 0, 0);
	UiState::stateManager->ForceRedraw();
}

void CreateSpriteEditor()
{
	if (currentRomType == -1)
		return;


	if (cOAMEdit::OamEditor == NULL)
	{
		cOAMEdit::OamEditor = new cOAMEdit();	
	}

	if (cOAMEdit::OamEditor->_oamWindow)
	{
		DestroyWindow(cOAMEdit::OamEditor->_oamWindow);
	

	}

	CreateDialog(hGlobal, MAKEINTRESOURCE(frmOAM), 0, (DLGPROC)OAMProc);

	ShowWindow(cOAMEdit::OamEditor->_oamWindow, SW_SHOW);
	SendMessage(cOAMEdit::OamEditor->_oamWindow, WM_COMMAND, MAKELONG(cboSpriteChoice,CBN_SELCHANGE), 0);
}
///Loads a cobomo box
void LoadCombos(sCombo* Combo, const char *FileName, int Max = 255)
{

	FILE *          text = fopen(FileName, "rt");
	char buffer[1024] = { 0 };
	Combo->Clear();

	if (text)
	{


		while (!feof(text))
		{
			fgets(buffer, 256, text);
			
				Combo->Additem(buffer);
			
		}


		fclose(text);

	}
	else
	{
		int c = 0;
		for (c = 0; c < Max; c++) {

			sprintf(buffer, "%X", c);
			Combo->Additem(buffer);
		}
	}
	Combo->SetListIndex(0);

}


///Returns true if we processed anything controls.
bool ProcessControls(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	RD1Engine* mainGame = NULL;
	if (currentRomType == -1)
		return false;
	if (RD1Engine::theGame)
	{
		mainGame = RD1Engine::theGame;
	}

	switch (LOWORD(wParam))
	{

	case ID_MAP_VIEWFOREGROUND:
	{
		if (mainGame == NULL) break;
		bool curVal = GlobalVars::gblVars->checkBoxViewF.GetCheckState();
		GlobalVars::gblVars->checkBoxViewF.SetCheckState(!curVal);
		GlobalVars::gblVars->ViewForeground = !curVal;
		mainGame->DrawStatus.BG0 = !curVal;
		UiState::stateManager->ForceRedraw();
		return true;
	}
	case ID_MAP_VIEWLEVELLAYER:
	{
		if (mainGame == NULL) break;
		bool curVal = GlobalVars::gblVars->checkBoxViewL.GetCheckState();
		GlobalVars::gblVars->checkBoxViewL.SetCheckState(!curVal);
		GlobalVars::gblVars->ViewLevel = !curVal;
		mainGame->DrawStatus.BG1 = !curVal;
		UiState::stateManager->ForceRedraw();
		return true;
	}

	case ID_MAP_VIEWBACKLAYER:
	{
		if (mainGame == NULL) break;
		bool curVal = GlobalVars::gblVars->checkBoxViewB.GetCheckState();
		GlobalVars::gblVars->checkBoxViewB.SetCheckState(!curVal);
		GlobalVars::gblVars->ViewBacklayer = !curVal;
		mainGame->DrawStatus.BG2 = !curVal;
		UiState::stateManager->ForceRedraw();
		return true;
	}
		break;
	case ID_MAP_SHOWSCROLLS:
		GlobalVars::gblVars->mnuShowScroll.SetCheckState(!GlobalVars::gblVars->mnuShowScroll.GetCheckState());

		DrawStatusFromUI();
	
		UiState::stateManager->ForceRedraw();
		return true;
		break;
	case ID_MAP_SHOWSPRITES:
		GlobalVars::gblVars->mnuItemHideSprites.SetCheckState(!GlobalVars::gblVars->mnuItemHideSprites.GetCheckState());
	
		DrawStatusFromUI();
		mainGame->DrawStatus.Sprites = !GlobalVars::gblVars->mnuItemHideSprites.GetCheckState();
		UiState::stateManager->ForceRedraw();
		return true;
		break;
	case ID_DAA:
		if (mainGame == NULL) break;
		RD1Engine::theGame->DumpAreaAsImage("heyman.bmp", GlobalVars::gblVars->imgTileset, &SpriteImage, GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage);
		return true;
		break;
	}
	return false;
}



int  HandleDetections(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{

	LeakFinder::finder->PollHeap();
	FILE           *fp = NULL;
	int area;

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
	LeakFinder::finder->PollHeap();
	if (ProcessControls(hwnd, message, wParam, lParam))
	{
		LeakFinder::finder->PollHeap();
		return 0;
	}
	LeakFinder::finder->PollHeap();

	if (mnuOpen == LOWORD(wParam))
	{
		OpenRom();
		return true;
	}
	LeakFinder::finder->PollHeap();
	if(mainGame==NULL) return 0; 
	switch (LOWORD(wParam))
	{
	case ID_ROOMTOOLS_EDITLEVELPAL:
		// if(crf==-1) return  0;
		ShowWindow(hwndLPE, 1);

		return true;


	case ID_MAP_SHOWSPRITES:
		DrawStatusFromUI();
		UiState::stateManager->ForceRedraw();
		return true;
	case mnuEB:
		if (currentRomType == -1)
			return 0;
		if (GBA.ReturnFileName(hGlobal, hwnd, "Please select a bitmap file\0*.bmp\0", cBuf, 512, 0)) {

			fp = fopen(cBuf, "w+");
			if (fp)
			{
				nMapBuffer* tmap = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData);
				RD1Engine::theGame->ThisBackBuffer.SaveToFile(fp);
				fclose(fp);
			}
			else
			{
				MessageBox(0, "Bad File Name", "Error", MB_OK);
			}
		}
		return true;
	case mnuInsert:
		if (currentRomType == -1)
			return 0;
		ShowWindow(GlobalVars::gblVars->BGi->me, 1);
		return true;

	case ID_ZOOM_NORMAL:
		if (currentRomType == -1)
			return 0;
		GlobalVars::gblVars->zoomLevel = 1.0;
		GameConfiguration::mainCFG->GetDataContainer("ZoomStates")->Value = 0;
		CheckZoom(0);
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, true);
		return true;
	case ID_ZOOM_1:
		GlobalVars::gblVars->zoomLevel = 1.5;
		GameConfiguration::mainCFG->GetDataContainer("ZoomStates")->Value = 1;
		CheckZoom(1);
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, true);
		return true;
	case ID_ZOOM_2:
		GlobalVars::gblVars->zoomLevel = 2.0;
		GameConfiguration::mainCFG->GetDataContainer("ZoomStates")->Value = 2;
		CheckZoom(2);
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, true);
		return true;
	case mnuIPSP:
		if (currentRomType == -1)
			return 0;
		GBA.ReturnFileName("Please Select an Apply File to apply\0*.ips", cBuf, 1024);
		ApplyIPS(cBuf, GBA.FileLoc);
		return true;



	case mnuClipboard:
		if (currentRomType == -1)
			return 0;
		ShowWindow(ClipBoard.me, SW_SHOW);
		return true;

	case mnuSOE:
		LeakFinder::finder->PollHeap();
		CreateSpriteEditor();
		LeakFinder::finder->PollHeap();
		return true;

	case ID_GENERALTOOLS_SAMUSEDITOR:
		if (currentRomType == -1)
			return 0;
		if (SamusEditorClass::hwndSamusEditor == NULL)
		{
			CreateDialog(hGlobal, MAKEINTRESOURCE(frmSamusEditor), 0, (DLGPROC)SamusProc);
			ShowWindow(SamusEditorClass::hwndSamusEditor, SW_SHOW);
		}
		return true;
	case mnuSSE:
		if (currentRomType == -1)
			return 0;
		CreateDialog(hGlobal, MAKEINTRESOURCE(fraSSE), 0, (DLGPROC)SpriteSetEditorProc);
		ShowWindow(cSpriteSetEditor::SpriteSet->me, SW_SHOW);
		return true;
	case mnuTSA:
		if (currentRomType == -1)
			return 0;
		ShowWindow(MyTSAEditor.tMain, SW_SHOW);
		return true;
	case mnuHeader:
		if (currentRomType == -1)
			return true;
		if (hwndHeader == NULL)
		{
			CreateDialog(hGlobal, MAKEINTRESOURCE(frmHeader), 0,(DLGPROC) HeaderProced);

		}
		if (hwndHeader) {
			LoadHeaderControls();
			ShowWindow(hwndHeader, SW_SHOW);
		}

		return true;
	case mnuRS:
		if (currentRomType == -1)
			return true;
		if (ExtendedOptWND == NULL)
		{
			CreateDialog(hGlobal, MAKEINTRESOURCE(frmRoomStuff), 0, (DLGPROC)ExtendedProc);
		}

		ShowWindow(ExtendedOptWND, SW_SHOW);
		LoadExtensions();
		LayerCombo.SetListIndex(1);
		SendMessage(ExtendedOptWND, WM_COMMAND, 0x000103eb, 0);

		return true;
	case IDCANCEL:
		if (MessageBox(hwnd, "Wait! Are you sure?", "Leaving so soon?", MB_YESNOCANCEL) == IDYES)
		{
			EndDialog(hwnd, 0);
			EndDialog(DoorWin, 0);
			EndDialog(hwndResize, 0);
		}

		return true;
	case mnuTE:
		if (currentRomType == -1)
			return 0;
		if (!GlobalVars::gblVars->TextEditor->tMain)
		{
			CreateDialog(hGlobal, MAKEINTRESOURCE(frmTE), 0, (DLGPROC)TextEditorProc);
		}
		ShowWindow(GlobalVars::gblVars->TextEditor->tMain, SW_SHOW);
		return true;
	case mnuSE:
		if (currentRomType == -1)
			return 0;
		if (!GlobalVars::gblVars->StatEditor) {
			GlobalVars::gblVars->StatEditor = new cStatEd(currentRomType);
			CreateDialog(hGlobal, MAKEINTRESOURCE(fraStat), 0, (DLGPROC)StatEditorProc);
		}
		if (GlobalVars::gblVars->StatEditor->me) {
			
	
		ShowWindow(GlobalVars::gblVars->StatEditor->me, SW_SHOW);
		}
		return true;


	case mnuMini:
		if (currentRomType == -1)
			return 0;
		if (!hwndMM) {
			CreateDialog(hGlobal, MAKEINTRESOURCE(frmMiniMap), 0, (DLGPROC)MiniProc);

		}

		//Reset form 
		MiniMapClass::miniMapEditor->cboPalette.SetListIndex(0);
		MiniMapClass::miniMapEditor->cboMArea.SetListIndex(comboArea.GetListIndex());
		MiniMapClass::miniMapEditor->DecompressMap(MiniMapClass::miniMapEditor->cboMArea.GetListIndex());
		MiniMapClass::miniMapEditor->DrawTileset(MiniMapClass::miniMapEditor->Tileset, 0);
		MiniMapClass::miniMapEditor->DrawMap(MiniMapClass::miniMapEditor->Map);
		ShowWindow(hwndMM, SW_SHOW);
		return true;

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

		return true;
	case mnuRR:

		if (currentRomType == -1)
			return 0;
		ShowWindow(hwndResize, SW_SHOW);
		break;


	}


	return true;
}
HWND tabs[6];
sChecks door;
void InitMainControls(HWND hwnd)
{
	hwndMain(hwnd);
	char            FilePath[1024] =
	{
		0
	};
	unsigned short             wndrs[3] =
	{
		hsbC1, hsbC2, hsbC3
	};


	for (int h = 0; h < 5; h++)
	{
		tabs[h] = NULL;
	}
	inited = false;
	{
		HICON           hIcon = LoadIcon(hGlobal, MAKEINTRESOURCE(IDI_ICON7));
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}

	if (UiState::stateManager->GetWindowState() == WindowState::EXPERT)
	{
		INITCOMMONCONTROLSEX commCon;
		commCon.dwSize = sizeof(INITCOMMONCONTROLSEX);
		commCon.dwICC = ICC_TAB_CLASSES;
		InitCommonControlsEx(&commCon); // have to run this to use tab control
		hTabControl = GetDlgItem(hwnd, tabMain);
		//	SetWindowSubclass(hTabControl, ComboProc, subclasscounter++, 0);
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



		tabs[0] = CreateDialog(hGlobal, MAKEINTRESOURCE(fraMain), hTabControl, (DLGPROC)fraMainProc); // Setting dialog to tab one by default

		tabs[1] = CreateDialog(hGlobal, MAKEINTRESOURCE(frmMapData), hTabControl, (DLGPROC)MapDataProc);

		tabs[2] = CreateDialog(hGlobal, MAKEINTRESOURCE(frmDoor), hTabControl, (DLGPROC)DwProc);

		tabs[3] = CreateDialog(hGlobal, MAKEINTRESOURCE(frmScroll), hTabControl, (DLGPROC)ScrollWndProc);

		tabs[4] = CreateDialog(hGlobal, MAKEINTRESOURCE(fraSS), hTabControl, (DLGPROC)SSProc);

		hCurrentTab = tabs[0];

		//	SetWindowSubclass(tabs[0], ComboProc, subclasscounter++, 0);
		//	SetWindowSubclass(tabs[1], ComboProc, subclasscounter++, 0);
		//	SetWindowSubclass(tabs[2], ComboProc, subclasscounter++, 0);
		//	SetWindowSubclass(tabs[3], ComboProc, subclasscounter++, 0);
		//	SetWindowSubclass(tabs[4], ComboProc, subclasscounter++, 0);
		ShowWindow(hCurrentTab, 1);
		UiState::stateManager->StatusBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE, "No data", hwnd, 9000);
	}

	ClearGlobals();


	

	//SetWindowSubclass(hwnd, ComboProc, subclasscounter++, 0);
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
	GlobalVars::gblVars->imgTileset->Create(16 * 17 + 1, 1024);
	/*GlobalVars::gblVars->ViewClip.SetCnt(GetDlgItem(hwnd, ID_MAP_SHOWCLIPDATA));
	
	GlobalVars::gblVars->checkBoxClip.SetCnt(GetDlgItem(hwnd, chkClip));
	GlobalVars::gblVars->checkBoxViewF.Init(hwnd, ID_MAP_VIEWFOREGROUND);
	GlobalVars::gblVars->checkBoxViewL.Init(hwnd, ID_MAP_VIEWLEVELLAYER);
	GlobalVars::gblVars->checkBoxViewB.Init(hwnd, ID_MAP_VIEWBACKLAYER);

	chkDoTrans.SetCheckState(true);*/
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


	for (int i = 0; i < 3; i++)
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

	UiState::stateManager->ResizeMap(hTabControl);
	CalculateMapScrolls(CurMapWidth, CurMapHeight);

}
BOOL CALLBACK   DialogProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	LeakFinder::finder->PollHeap();
	switch (message)
	{
	case WM_PAINT:
		break;
	case WM_INITDIALOG:
		InitMainControls(hwnd);
		//LoadTrans("[MainMenu]", 0, hwnd);
		return true;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE: // ******* message sent because someone changed the tab selection (clicked on another tab)
		{
			LeakFinder::finder->PollHeap();
			int curTab = TabCtrl_GetCurSel(hTabControl);
			if (currentRomType == -1 && curTab != 0)
			{
				TabCtrl_SetCurSel(hTabControl, 0);
				break;
			}
			ShowWindow(hCurrentTab, 0); // we don't want the current tab, kill it
			LeakFinder::finder->PollHeap();
			switch (curTab)
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
				if (GlobalVars::gblVars->chkBoxED.value())
				{
					hCurrentTab = tabs[2];
					RD1Engine::theGame->mgrDoors->LoadDoor(0);
				}
				else
				{
					ErrorMessage("Please check Edit Doors before going to this tab.");
					TabCtrl_SetCurSel(hTabControl, 0);

					hCurrentTab = tabs[0];
				}
				LeakFinder::finder->PollHeap();
				break;
			case 3:
				if (!tabs[3])
				{
				}
				if (GlobalVars::gblVars->ScrollCheck.value())
				{
					hCurrentTab = tabs[3];
					RD1Engine::theGame->mgrScrolls->LoadScrollInfo(0);
				}
				else
				{
					ErrorMessage("Please check Edit Scrolls before going to this tab.");
					TabCtrl_SetCurSel(hTabControl, 0);
					hCurrentTab = tabs[0];
				}
				LeakFinder::finder->PollHeap();
				break;
			case 4:
				if (!tabs[4])
				{
				}
				if (GlobalVars::gblVars->chkEditSprites.value())
				{
					hCurrentTab = tabs[4];
					SpriteTabIndex = 0;
					UiState::stateManager->ShowObj();
					LoadCurSprite();
				}
				else
				{
					ErrorMessage("Please check Edit Sprites before going to this tab.");
					TabCtrl_SetCurSel(hTabControl, 0);
					hCurrentTab = tabs[0];
				}
				LeakFinder::finder->PollHeap();
				break;
			}
			ShowWindow(hCurrentTab, 1);
			RECT k;

			//Resize 
			TabResize();
			LeakFinder::finder->PollHeap();
			return TRUE;
		}//End of case
		}//End of Switch
		return TRUE;
		break;
	
	case WM_COMMAND:

		HandleDetections(hwnd, message, wParam, lParam);
		LeakFinder::finder->PollHeap();
		return true;

	case WM_CTLCOLORDLG:
		return (LONG)g_hbrBackground;
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(255, 255, 255));
		SetBkMode(hdcStatic, TRANSPARENT);
		LeakFinder::finder->PollHeap();
		return (LONG)g_hbrBackground;
	}
	return true;
	case WM_DESTROY:
		bRunApp = 0;
		if (GBA.ROM)
			fclose(GBA.ROM);
		// / myBackBuffer.Destroy();
		return true;
	case WM_SIZE:
		//min heigh needs to be at least tileset

		if (lParam != 0)
		{
			TabResize();
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

		return true;
	}

	return false;
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
	DynaRec::instance = new DynaRec();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	g_hbrBackground = CreateSolidBrush(RGB(0x2C, 0x2F, 0x33));
	hwndHeader = NULL;
	FreeImage_Initialise();

	ExtendedOptWND = NULL;
	Logger::log = new Logger();
	LeakFinder::finder = new LeakFinder();

	//init stufff
	GlobalVars::gblVars = new GlobalVars();


	GBAGraphics::VRAM = new GBAGraphics();
	//RD1Engine::theGame = new RD1Engine(SupportedTitles::titleMF, NULL, NULL, NULL, NULL);
	cSpriteSetEditor::SpriteSet = new cSpriteSetEditor();



	cOAMEdit::OamEditor = NULL;

	currentRomType = -1;
	hGlobal = hInstance;
	bRunApp = 1;
	DefRoom = 0;
	DefArea = 0;
	DefAddress = 0;
	Tile = 0;
	keyno = 0;


	MP[0] = MP[1] = 0;

	GameConfiguration* tmpConfig = new GameConfiguration(-1);
	UiState*mgr= NULL;
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
	FreeImage_DeInitialise();
	delete GameConfiguration::mainCFG;
	delete GlobalVars::gblVars;
	delete GBAGraphics::VRAM;
	delete MiniMapClass::miniMapEditor;
	delete cSpriteSetEditor::SpriteSet;


	delete cOAMEdit::OamEditor;


	return 0;

}

int             SetUpCombos()
{
	char *             quick = new char[2048];
	char            cs[128] =
	{
	   0
	};

	comboSpriteSet.Clear();
	comboSpriteSet.Additem("0");
	comboSpriteSet.Additem("1");
	comboSpriteSet.Additem("2");
	comboSpriteSet.SetListIndex(0);
	
		comboArea.Clear();
		cboDoorArea1.Clear();
		cboDoorArea3.Clear();
		cboDoorArea2.Clear();
		cboDoorDoor2.Clear();
		vector<string>* areaNames = RD1Engine::theGame->titleInstance->GetAreaNames();
		for (int i = 0; i < areaNames->size(); i++)
		{
			comboArea.Additem((char*)areaNames->at(i).c_str());
			cboDoorArea1.Additem((char*)areaNames->at(i).c_str());
			cboDoorArea3.Additem((char*)areaNames->at(i).c_str());
			cboDoorArea2.Additem((char*)areaNames->at(i).c_str());
			cboDoorDoor2.Additem((char*)areaNames->at(i).c_str());
		}
	

		if (currentRomType == 0)
		{

			// char que
			sprintf(quick, "%s\\ZM\\clip.txt", GlobalVars::gblVars->AppPath);

		}
		else if (currentRomType == 1)
		{

			sprintf(quick, "%s\\MF\\clip.txt", GlobalVars::gblVars->AppPath);

		}

		LoadCombos(&cboClipData, quick, 255);
		delete[] quick;
	return 0;
}

//Setups vars
int             ClearGlobals()
{
	int             i = 0;

	currentRomType = -1; // Current ROM file

	InputHwnd = NULL;


	memset(&TilesetRect, 0, sizeof(TilesetRect));

	hwndResize = NULL;
	wndHE = NULL;

	movingDoors = 0;
	movingDoorIndex = -1;
	movingSprite = 0;
	movingSpriteIndex = -1;
	SceneWnd = NULL;
	SceneGraph = NULL;
	TSScene = NULL;
	SceneryPic.Destroy();
	SceneryTiles.Destroy();

	memset(&DoorConnections, 0, sizeof(DoorConnections));

	for (i = 0; i < 11; i++)
		nDisplayWidth[i] = nDisplayHeight[i] = 0;
	memset(&GBAGraphics::VRAM->BGBuf, 0, sizeof(GBAGraphics::VRAM->BGBuf));

	if (GlobalVars::gblVars->imgTileset)
	{
		GlobalVars::gblVars->imgTileset->Destroy();
	}

	bRunApp = 0;
	CurDoor = 0;

	return 0;
}

//Loads arcair config
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
			int ssVal = 0;
			if (strcmp(stringchk, "END") == 0)
			{
				return 1;
			}
			else if (room[strlen("Default Room")] == stringchk[strlen("Default Room")])
			{
				memcpy(&valstring, &stringchk[strlen("Default Room:")], strlen(stringchk) - strlen("Default Room:"));
				sscanf(valstring, "%d", &ssVal);
				DefRoom = (unsigned char)ssVal;
			}
			else if (fs[strlen("Start Freespace")] == stringchk[strlen("Start Freespace")])
			{
				memcpy(&valstring, &stringchk[strlen("Start Freespace:")], strlen(stringchk) - strlen("Start Freespace:"));
				sscanf(valstring, "%X", &ssVal);
				DefAddress = (unsigned char)ssVal;
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

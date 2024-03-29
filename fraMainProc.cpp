#include "fraMainProc.h"
#include <map>
#include "MainHeader.h"
#include "AppData.h"
using namespace std;
void TabResize();
void SaveState()
{
	map<string, int>* theMapping = new map<string, int>();


}

void DrawStatusFromUI()
{
	RD1Engine::theGame->DrawStatus.Clipdata = GlobalVars::gblVars->ViewClip.value();

	RD1Engine::theGame->DrawStatus.ScrollsFlag = GlobalVars::gblVars->mnuShowScroll.GetCheckState();

	RD1Engine::theGame->DrawStatus.SpriteRect = !GlobalVars::gblVars->mnuItemHideSprites.GetCheckState();
}

void CheckZoom(int zoomid)
{
	HMENU mainMenu = GetMenu(UiState::stateManager->GetWindow());

	int zoomIDs[3] = { ID_ZOOM_NORMAL,ID_ZOOM_1, ID_ZOOM_2 };
	for (int i = 0; i < 3; i++)
	{
		CheckMenuItem(mainMenu, zoomIDs[i], MF_UNCHECKED);
	}
	CheckMenuItem(mainMenu, zoomIDs[zoomid], MF_CHECKED);

}

void DisableByState(sChecks* chkToUse)
{
	int curval = chkToUse->value();
	GlobalVars::gblVars->checkBoxForeground.value(0);
	GlobalVars::gblVars->checkBoxBackground.value(0);
	GlobalVars::gblVars->checkBoxClip.value(0);
	GlobalVars::gblVars->checkBoxLevel.value(0);
	GlobalVars::gblVars->ScrollCheck.value(0);
	GlobalVars::gblVars->chkEditSprites.value(0);
	GlobalVars::gblVars->chkBoxED.value(0);

	
	chkToUse->value(curval);
	if (GlobalVars::gblVars->chkBoxED.val == 0)
	{
		SendMessage(GetDlgItem(GlobalVars::gblVars->frameControls, chkResizeDoors), BM_SETCHECK, 0, 0);
	}
}

int UpdateHeaderControls()
{
	RHeader* roomHeader = RD1Engine::theGame->mainRoom->roomHeader;
	GlobalVars::gblVars->checkBoxForeground.State(roomHeader->bBg0 == 0x10);
	GlobalVars::gblVars->chkMC[0].State(roomHeader->bBg0 == 0x10);
	GlobalVars::gblVars->checkBoxLevel.State(roomHeader->bBg1 == 16);
	GlobalVars::gblVars->chkMC[1].State(roomHeader->bBg1 == 16);
	GlobalVars::gblVars->checkBoxBackground.State(roomHeader->bBg2 == 0x10);
	GlobalVars::gblVars->chkMC[2].State(roomHeader->bBg2 == 0x10);
	EnableWindow(GetDlgItem(SceneWnd, cmdSEditForeground), (roomHeader->bBg0 == 0x40 ? 1 : 0));
	EnableWindow(GetDlgItem(SceneWnd, cmdSEditBack), (roomHeader->bBg2 == 0x40 ? 1 : 0));
	EnableWindow(GetDlgItem(SceneWnd, cmdSEditBG), (roomHeader->lBg3 == 0x40 ? 1 : 0));
	/*GlobalVars::gblVars->checkBoxViewF.State(((roomHeader->bBg0 == 0x10) || (roomHeader->bBg0 == 0x40)));
	GlobalVars::gblVars->checkBoxViewL.State(roomHeader->bBg1 == 16);
	GlobalVars::gblVars->checkBoxViewB.State(((roomHeader->bBg2 == 0x10) || (roomHeader->bBg2 == 0x40)));*/
	//GlobalVars::gblVars->CheckBoxes[viewBG].State((BaseGame::theGame->mainRoom->roomHeader->lBg3==0x40) || (BaseGame::theGame->mainRoom->roomHeader->lBg3==0xF0));
	GlobalVars::gblVars->checkBoxViewF.SetCheckState(((roomHeader->bBg0 == 0x10) || (roomHeader->bBg0 == 0x40)));
	GlobalVars::gblVars->checkBoxViewL.SetCheckState((roomHeader->bBg1 == 16));
	GlobalVars::gblVars->checkBoxViewB.SetCheckState(((roomHeader->bBg2 == 0x10) || (roomHeader->bBg2 == 0x40)));
	//GlobalVars::gblVars->CheckBoxes[viewBG].value((BaseGame::theGame->mainRoom->roomHeader->lBg3==0x40) || (BaseGame::theGame->mainRoom->roomHeader->lBg3==0xF0));
	//Check for BGi hack installed

	if ((roomHeader->bBg0 == 0x40) || (roomHeader->bBg2 == 0x40) || (roomHeader->lBg3 == 0x40 || roomHeader->lBg3 == 0xF0) || BGiInstall) {
		EnableWindow(GetDlgItem(GlobalVars::gblVars->frameControls, cmdES), 1);
	}
	else {
		EnableWindow(GetDlgItem(GlobalVars::gblVars->frameControls, cmdES), 0);
	}
	return 0;
}

///Returns true if we processed anything controls.
bool ProcessControls2(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
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

	case ID_DAA:
		RD1Engine::theGame->DumpAreaAsImage("heyman.bmp", GlobalVars::gblVars->imgTileset, &SpriteImage, GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage);
		return true;
		break;
	}
	return false;
}

int  HandleDetections2(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
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
	if (RD1Engine::theGame == NULL)
	{
		return -1;
	}
	ProcessControls2(hwnd, message, wParam, lParam);

	switch (LOWORD(wParam))
	{
	case   chkResizeDoors:
		if (GlobalVars::gblVars->chkBoxED.value() == 1) {

		}
		break;
	case chkSprites:
		DisableByState(&GlobalVars::gblVars->chkEditSprites);
		someval = GlobalVars::gblVars->chkEditSprites.value();
		RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(someval == 1 ? editingStates::SPRITE : editingStates::MAP);
		UiState::stateManager->ShowObj();
		break;
	case chkDoors:
		DisableByState(&GlobalVars::gblVars->chkBoxED);
		someval = GlobalVars::gblVars->chkBoxED.value();
		RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(someval == 1 ? editingStates::DOOR : editingStates::MAP);
		UiState::stateManager->ShowObj();
		break;
	

	case mnuIPSP:
		if (currentRomType == -1)
			return 0;
		GBA.ReturnFileName("Select an IPS File to apply\0*.ips", cBuf, 1024);
		ApplyIPS(cBuf, GBA.FileLoc);
		break;


	case ID_MAP_SHOWTRANSPARENCY:
		if (currentRomType == -1)
			return 0;
		RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->Dirty = 1;
		UiState::stateManager->ForceRedraw();
		break;
	case cboClip:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			UiState::stateManager->ForceRedraw();
		}
		break;

	case cmdES:

		if (currentRomType == -1)
			return 0;
		ShowWindow(SceneWnd, SW_SHOW);
		ShowWindow(TSScene, SW_SHOW);
		break;

	case cboSpriteSet:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			RD1Engine::theGame->mainRoom->LoadUpSprites(comboSpriteSet.GetListIndex(), &SpriteImage);
			UiState::stateManager->ForceRedraw();
		}
		break;

	case chkScroll:
		DisableByState(&GlobalVars::gblVars->ScrollCheck);
		EnableWindow(cboScroll.GetHwnd(), GlobalVars::gblVars->ScrollCheck.value());
		if (GBA.ROM &&  GlobalVars::gblVars->ScrollCheck.value() == 1) {
			RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(editingStates::SCROLL);
		}
		else 
		{
			RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(editingStates::MAP);
		}
		if (GlobalVars::gblVars->ScrollCheck.value()) UiState::stateManager->ShowObj();
		UiState::stateManager->ForceRedraw();
		break;
	case cmdSave:
		//throw new exception("SAVE IS BROKE");
		MemFile::currentFile->save();
	
		//if (!BIC || (BIC > 0x7FFFFF)) {
		//	MessageBox(hwnd, "Uh so something went wrong.", "Boomb", MB_OK);
		//}
		//else {


			RD1Engine::theGame->SaveLevel();
		//	UpdateHeaderControls();
		//	UiState::stateManager->UpdateMapObjectWindow();
		//}	RD1EnginareaManager->GetCurrentArea()->Save();// RoomOffsets[comboArea.GetListIndex()] - 0x8000000) + (comboRoom.GetListIndex() * 0x3C);

		break;
	case chkForeground:
		// Check if it's enabled or disabled.
		DisableByState(&GlobalVars::gblVars->checkBoxForeground);
		RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(editingStates::MAP);
		if (GlobalVars::gblVars->checkBoxForeground.value())UiState::stateManager->ShowObj();
		break;

	case cboDScroll:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			UiState::stateManager->ForceRedraw();
		}
		break;
	case  ID_MAP_ANIMATE:
		GlobalVars::gblVars->checkBoxLevel.value(!GlobalVars::gblVars->checkBoxLevel.value());
		break;
	case chkLevel:
		DisableByState(&GlobalVars::gblVars->checkBoxLevel);
		RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(editingStates::MAP);
		if (GlobalVars::gblVars->checkBoxLevel.value())UiState::stateManager->ShowObj();
		break;
	case chkST:
		ShowWindow(UiState::stateManager->GetTilesetWindow(), GlobalVars::gblVars->checkBoxshowtileset.value() * 5);
		break;
	case chkSM:
		ShowWindow(UiState::stateManager->GetMapWindow(), GlobalVars::gblVars->checkBoxshowmap.value() * 5);
		break;
	case chkBacklayer:
		// Check if it's enabled or disabled.
		DisableByState(&GlobalVars::gblVars->checkBoxBackground);
		RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(editingStates::MAP);
		UiState::stateManager->ShowObj();
		break;
	case chkClip:
		// Check if it's enabled or disabled.
		DisableByState(&GlobalVars::gblVars->checkBoxClip);
		RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(editingStates::MAP);
		UiState::stateManager->ShowObj();
		break;
	case ID_MAP_VIEWBACKLAYER:
	case ID_MAP_VIEWFOREGROUND:
	case ID_MAP_VIEWLEVELLAYER:
	case ID_MAP_VIEWBACKGROUND:
		if (!LoadingLevel)
			DrawStatusFromUI();
		UiState::stateManager->ForceRedraw();
		break;
	case chkSRe:
		GlobalVars::gblVars->checkBoxsMove.SetCheckState(false);
		break;
	case chkSMove:
		GlobalVars::gblVars->checkBoxsResize.SetCheckState(false);
		break;

	case ID_MAP_SHOWCLIPDATA:
		DrawStatusFromUI();
		UiState::stateManager->ForceRedraw();

		break;
	case cboArea:
	case cbArea:
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			area = comboArea.GetListIndex();



			LevelCounter = RD1Engine::theGame->titleInstance->GetRoomCount(area);
			comboRoom.Clear();
			RD1Engine::theGame->mgrDoors->SetupDoors(area);
			for (i = 0; i < LevelCounter; i++)
			{
				sprintf(cBuf, "%X", i);
				comboRoom.Additem(cBuf);
			}



			if (DefRoom >= LevelCounter)
				comboRoom.SetListIndex(LevelCounter);
			else
				comboRoom.SetListIndex(DefRoom);

			PostMessage(hwnd, WM_COMMAND, 0x000103ef, 0); // Simulate room combo 
			//											  // clicking.

			break;
		}
	case cboRoom:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			int room = comboRoom.GetListIndex();

			RD1Engine::theGame->LoadRoomSpriteSet(comboArea.GetListIndex(), comboRoom.GetListIndex(), GlobalVars::gblVars->imgTileset, &SpriteImage);
			comboSpriteSet.Clear();
			for (int i = 0; i < RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects.size(); i++)
			{
				char numstr[4] = { 0 };
				sprintf(numstr, "%d", i);
				comboSpriteSet.Additem(numstr);
			}
			comboSpriteSet.SetListIndex(0);
			ShowWindow(ExtendedOptWND, SW_HIDE);

			DumpLayers();

			//RD1Engine::theGame->mainRoom->LoadUpSprites(0);


			//	RoomBuff = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround);
			//	/*RoomBuff->UndoBuff->UndoNum = 0;
			//	RoomBuff->UndoBuff->Set((RoomBuff->X*RoomBuff->Y) * 2, RoomBuff->TileBuf2D);*/

			//	RoomBuff = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData);
			///*	RoomBuff->UndoBuff->UndoNum = 0;
			//	RoomBuff->UndoBuff->Set((RoomBuff->X*RoomBuff->Y) * 2, RoomBuff->TileBuf2D);*/

			//	RoomBuff = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer);
			//	/*RoomBuff->UndoBuff->UndoNum = 0;
			//	RoomBuff->UndoBuff->Set((RoomBuff->X*RoomBuff->Y) * 2, RoomBuff->TileBuf2D);*/

			//	RoomBuff = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata);
			//	/*RoomBuff->UndoBuff->UndoNum = 0;
			//	RoomBuff->UndoBuff->Set((RoomBuff->X*RoomBuff->Y) * 2, RoomBuff->TileBuf2D);*/
			DoesaBridgeExist();
			UserEnableConnections();
			RD1Engine::theGame->mgrScrolls->LoadScrollControls();
			UpdateHeaderControls();
			DrawLevel();

			UiState::stateManager->ResetCursor();


			//memset(&DrawState, 0, sizeof(drawstate));
		
			//MyTSAEditor.DrawThisTileset();
		//	MyTSAEditor.LoadTSA();
			loadit = true;
			SendMessage(hwnd, WM_COMMAND, 0x00010408, 0); // Update cboSprite 
			//											  // with SelChange

			LoadHeaderControls();
			//for (i = 0; i < 16; i++)
			//   ClipBoard.Erase(i, 0xFFFF);
			LoadingLevel = 0;

		
			SendMessage(UiState::stateManager->GetMapWindow(), WM_SIZE, 0, 1); // If lparam is 0 nothing

			RECT            t;
			TabResize();
		
			GetWindowRect(UiState::stateManager->GetMapWindow(), &t);
			MoveWindow(UiState::stateManager->GetMapWindow(), t.left, t.top, 512, 496, 1);
			UiState::stateManager->UpdateMapObjectWindow();

			
			UiState::stateManager->ForceRedraw();
		}
		}
		break;
	default:
		return 0;
	}

	return 0;
}
Image* myLoadedPic;
//Init controls
void InitControls(HWND hwnd)
{
	unsigned short             wndrs[3] =
	{
		hsbC1, hsbC2, hsbC3
	};
	RECT k;
	/*UiState::AutoRect(hwnd, &k);
	MoveWindow(hwnd, k.left, k.top + 8, k.right, k.bottom, true);*/
	GlobalVars::gblVars->frameControls = hwnd;
	CurObjT = 0;
	CurObjNo = 0;



	clrIndex = 0;
	//GlobalVars::gblVars->imgTileset->Create(512, 1024);
	GlobalVars::gblVars->mnuItemHideSprites.Init(hwnd, ID_MAP_SHOWSPRITES);
	GlobalVars::gblVars->mnuShowScroll.Init(hwnd, ID_MAP_SHOWSCROLLS);
	GlobalVars::gblVars->checkBoxForeground.SetCnt(GetDlgItem(hwnd, chkForeground));
	GlobalVars::gblVars->checkBoxLevel.SetCnt(GetDlgItem(hwnd, chkLevel));
	GlobalVars::gblVars->checkBoxLevel.value(2);
	GlobalVars::gblVars->checkBoxClip.SetCnt(GetDlgItem(hwnd, chkClip));
	GlobalVars::gblVars->checkBoxBackground.SetCnt(GetDlgItem(hwnd, chkBacklayer));
	GlobalVars::gblVars->ScrollCheck.SetCnt(GetDlgItem(hwnd, chkScroll));
	GlobalVars::gblVars->checkBoxViewBL.Init(UiState::stateManager->GetWindow(), chkViewBG);
	GlobalVars::gblVars->checkBoxsMove.Init(UiState::stateManager->GetWindow(), chkSMove);
	GlobalVars::gblVars->checkBoxsResize.Init(UiState::stateManager->GetWindow(), chkSRe);
	GlobalVars::gblVars->checkBoxsView.Init(UiState::stateManager->GetWindow(), chkSView);
	GlobalVars::gblVars->checkBoxshowmap.SetCnt(GetDlgItem(hwnd, chkSM));
	GlobalVars::gblVars->checkBoxshowtileset.SetCnt(GetDlgItem(hwnd, chkST));
	GlobalVars::gblVars->chkEditSprites.SetCnt(GetDlgItem(hwnd, chkSprites));
	GlobalVars::gblVars->chkBoxED.SetCnt(GetDlgItem(hwnd, chkDoors));

	// GlobalVars::gblVars->CheckBoxes[chkESC].SetCnt(GetDlgItem(Main,chkSprites));
	GlobalVars::gblVars->chkMC[0].SetCnt(GetDlgItem(hwnd, chkForeground2));
	GlobalVars::gblVars->chkMC[1].SetCnt(GetDlgItem(hwnd, chkLevel2));
	GlobalVars::gblVars->chkMC[2].SetCnt(GetDlgItem(hwnd, chkBacklayer2));
	GlobalVars::gblVars->chkMC[3].SetCnt(GetDlgItem(hwnd, chkClip2));
	chkDoTrans.Init(UiState::stateManager->GetWindow(), ID_MAP_SHOWTRANSPARENCY);
	chkDoTrans.SetCheckState(true);

	GlobalVars::gblVars->checkBoxshowmap.value(1);
	GlobalVars::gblVars->checkBoxshowtileset.value(1);
	GlobalVars::gblVars->chkAnimatez.Init(UiState::stateManager->GetWindow(), ID_MAP_ANIMATE);
	GlobalVars::gblVars->chkAnimatez.SetCheckState(true);




	CreateDialog(hGlobal, MAKEINTRESOURCE(frmSceneryEditor), 0, (DLGPROC)SceneProc);



	CreateDialog(hGlobal, MAKEINTRESOURCE(fraTBE), 0, (DLGPROC)TSAProc);

	CreateDialog(hGlobal, MAKEINTRESOURCE(frmClipboard), 0, (DLGPROC)ClipBoardProc);
	CreateDialog(hGlobal, MAKEINTRESOURCE(fraBGI), 0, (DLGPROC)BGiProc);
	CreateDialog(hGlobal, MAKEINTRESOURCE(frmLPE), 0, (DLGPROC)LPProc);


	for (int i = 0; i < 3; i++)
	{
		GlobalVars::gblVars->scrColors[i].create(GetDlgItem(hwndLPE, wndrs[i]), 0, 255);
		GlobalVars::gblVars->scrColors[i].ChangeScrollbars();
		// SetScrollPos( UiState::stateManager->GetTilesetWindow(), SB_HORZ, 0, 1 );

	}

	cboScroll.Init(GetDlgItem(hwnd, cboDScroll));
	cboScroll.Disable();
	comboArea.Init(GetDlgItem(hwnd, (int)MAKEINTRESOURCE(cbArea)));
	comboRoom.Init(GetDlgItem(hwnd, (int)MAKEINTRESOURCE(cboRoom)));
	cboClipData.Init(GetDlgItem(hwnd, (int)MAKEINTRESOURCE(cboClip)));
	comboSpriteSet.Init(GetDlgItem(hwnd, (int)MAKEINTRESOURCE(cboSpriteSet)));
	cboClipData.Additem("Clipdata");
	cboClipData.SetListIndex(0);
	comboSpriteSet.Additem("0");
	comboSpriteSet.SetListIndex(0);
	comboRoom.Additem("Rooms");
	comboRoom.SetListIndex(0);

	comboArea.Additem("Areas");
	comboArea.SetListIndex(0);

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
	//myLoadedPic = Image::Import(GlobalVars::gblVars->AppPath, "dmansbg.png");

}
BOOL CALLBACK  fraMainProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	char            FilePath[1024] =
	{
		0
	};

	switch (message)
	{

	case WM_INITDIALOG:
		InitControls(hwnd);
		break;

	case  WM_WINDOWPOSCHANGED:



		break;
	case WM_CTLCOLORDLG:
		return (LONG)g_hbrBackground;
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(255, 255, 255));
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LONG)g_hbrBackground;
	}
	break;
	case BN_CLICKED:

		break;
	case WM_COMMAND:

		HandleDetections2(hwnd, message, wParam, lParam);
		break;

	case WM_VSCROLL:

		break;
	case WM_PAINT:

	{

		//hdc = BeginPaint(hwnd, &ps);


		//EndPaint(hwnd, &ps);
		//ReleaseDC(hwnd, hdc);


	}
	}

	return 0;
}

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


void LoadState(map<string, int> * mapping)
{

}

void DisableByState(sChecks* chkToUse)
{
	int curval = chkToUse->value();
	GlobalVars::gblVars->checkBoxForeground.value(0);
	GlobalVars::gblVars->checkBoxBackground.value(0);
	GlobalVars::gblVars->checkBoxClip.value(0);
	GlobalVars::gblVars->checkBoxLevel.value(0);
	GlobalVars::gblVars->ScrollCheck.value(0);
	GlobalVars::gblVars->checkBoxchkES.value(0);
	GlobalVars::gblVars->chkBoxED.value(0);

	SendMessage(GetDlgItem(GlobalVars::gblVars->frameControls, chkResizeDoors), BM_SETCHECK, 0, 0);
	chkToUse->value(curval);
}

int UpdateHeaderControls()
{
	GlobalVars::gblVars->checkBoxForeground.State(RD1Engine::theGame->mainRoom->roomHeader.bBg0 == 0x10);
	GlobalVars::gblVars->chkMC[0].State(RD1Engine::theGame->mainRoom->roomHeader.bBg0 == 0x10);
	GlobalVars::gblVars->checkBoxLevel.State(RD1Engine::theGame->mainRoom->roomHeader.bBg1 == 16);
	GlobalVars::gblVars->chkMC[1].State(RD1Engine::theGame->mainRoom->roomHeader.bBg1 == 16);
	GlobalVars::gblVars->checkBoxBackground.State(RD1Engine::theGame->mainRoom->roomHeader.bBg2 == 0x10);
	GlobalVars::gblVars->chkMC[2].State(RD1Engine::theGame->mainRoom->roomHeader.bBg2 == 0x10);
	EnableWindow(GetDlgItem(SceneWnd, cmdSEditForeground), (RD1Engine::theGame->mainRoom->roomHeader.bBg0 == 0x40 ? 1 : 0));
	EnableWindow(GetDlgItem(SceneWnd, cmdSEditBack), (RD1Engine::theGame->mainRoom->roomHeader.bBg2 == 0x40 ? 1 : 0));
	EnableWindow(GetDlgItem(SceneWnd, cmdSEditBG), (RD1Engine::theGame->mainRoom->roomHeader.lBg3 == 0x40 ? 1 : 0));
	GlobalVars::gblVars->checkBoxViewF.State(((RD1Engine::theGame->mainRoom->roomHeader.bBg0 == 0x10) || (RD1Engine::theGame->mainRoom->roomHeader.bBg0 == 0x40)));
	GlobalVars::gblVars->checkBoxViewL.State(RD1Engine::theGame->mainRoom->roomHeader.bBg1 == 16);
	GlobalVars::gblVars->checkBoxViewB.State(((RD1Engine::theGame->mainRoom->roomHeader.bBg2 == 0x10) || (RD1Engine::theGame->mainRoom->roomHeader.bBg2 == 0x40)));
	//GlobalVars::gblVars->CheckBoxes[viewBG].State((BaseGame::theGame->mainRoom->roomHeader.lBg3==0x40) || (BaseGame::theGame->mainRoom->roomHeader.lBg3==0xF0));
	GlobalVars::gblVars->checkBoxViewF.value(((RD1Engine::theGame->mainRoom->roomHeader.bBg0 == 0x10) || (RD1Engine::theGame->mainRoom->roomHeader.bBg0 == 0x40)));
	GlobalVars::gblVars->checkBoxViewL.value((RD1Engine::theGame->mainRoom->roomHeader.bBg1 == 16));
	GlobalVars::gblVars->checkBoxViewB.value(((RD1Engine::theGame->mainRoom->roomHeader.bBg2 == 0x10) || (RD1Engine::theGame->mainRoom->roomHeader.bBg2 == 0x40)));
	//GlobalVars::gblVars->CheckBoxes[viewBG].value((BaseGame::theGame->mainRoom->roomHeader.lBg3==0x40) || (BaseGame::theGame->mainRoom->roomHeader.lBg3==0xF0));
	//Check for BGi hack installed

	if ((RD1Engine::theGame->mainRoom->roomHeader.bBg0 == 0x40) || (RD1Engine::theGame->mainRoom->roomHeader.bBg2 == 0x40) || (RD1Engine::theGame->mainRoom->roomHeader.lBg3 == 0x40 || RD1Engine::theGame->mainRoom->roomHeader.lBg3 == 0xF0) || BGiInstall) {
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
	case chkViewF:
		GlobalVars::gblVars->ViewForeground = GlobalVars::gblVars->checkBoxViewF.value() == BST_CHECKED;
		mainGame->DrawStatus.BG0 = GlobalVars::gblVars->ViewForeground;
		mainGame->DrawStatus.dirty = true;
		RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, false);
		return true;
		break;
	case chkViewL:
		GlobalVars::gblVars->ViewLevel = GlobalVars::gblVars->checkBoxViewL.value() == BST_CHECKED;
		mainGame->DrawStatus.BG1 = GlobalVars::gblVars->ViewLevel;
		mainGame->DrawStatus.dirty = true;
		RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, false);
		return true;
		break;
	case chkViewB:
		GlobalVars::gblVars->ViewBacklayer = GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED;
		mainGame->DrawStatus.BG2 = GlobalVars::gblVars->ViewBacklayer;
		mainGame->DrawStatus.dirty = true;
		RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, false);
		return true;
		break;
	case chkHS:
		//GlobalVars::gblVars->ViewBacklayer = GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED;
		return true;
		break;
	case ID_DAA:
		RD1Engine::theGame->DumpAreaAsImage("heyman.bmp");
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
	ProcessControls2(hwnd,  message, wParam,  lParam);
	
	switch (LOWORD(wParam))
	{
	case   chkResizeDoors:
		if (GlobalVars::gblVars->chkBoxED.value() == 1) {
			
		}
		break;
	case chkSprites:
		DisableByState(&GlobalVars::gblVars->checkBoxchkES);
		someval = GlobalVars::gblVars->checkBoxchkES.value();
		RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(someval == 1 ? editingStates::SPRITE : editingStates::MAP);
		UiState::stateManager->ShowObj();
		break;
	case chkDoors:
		DisableByState(&GlobalVars::gblVars->chkBoxED);
		someval = GlobalVars::gblVars->chkBoxED.value();
		RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(someval == 1 ? editingStates::DOOR : editingStates::MAP);
		UiState::stateManager->ShowObj();
		break;
	case cmdLPE:
		// if(crf==-1) return  0;
		ShowWindow(hwndLPE, 1);

		break;

	
	case chkHS:
		RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
		break;

	case mnuIPSP:
		if (currentRomType == -1)
			return 0;
		GBA.ReturnFileName("Please Select an Apply File to apply\0*.ips", cBuf, 1024);
		ApplyIPS(cBuf, GBA.FileLoc);
		break;


	case chkDTrans:
		if (currentRomType == -1)
			return 0;
		RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->Dirty = 1;
		RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
		break;
	case cboClip:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
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
			RD1Engine::theGame->mainRoom->LoadUpSprites(Combos[cSpriteSet].GetListIndex());
			RD1Engine::theGame->DrawStatus.dirty = true;
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);

			InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);

		}
		break;
	case chkScroll:
		DisableByState(&GlobalVars::gblVars->ScrollCheck);
		EnableWindow(cboScroll.GetHwnd(), GlobalVars::gblVars->ScrollCheck.value());
		if (GBA.ROM &&  GlobalVars::gblVars->ScrollCheck.value() == 1) {
			RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(editingStates::SCROLL);
			RD1Engine::theGame->DrawStatus.dirty = true;
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);

		}
		else {
			RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetState(editingStates::MAP);
		}
		RD1Engine::theGame->DrawStatus.dirty = true;
		RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
		if (GlobalVars::gblVars->ScrollCheck.value()) UiState::stateManager->ShowObj();
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
		break;
	case cmdSave:
		BIC = (RD1Engine::theGame->RoomOffsets[Combos[cArea].GetListIndex()] - 0x8000000) + (Combos[cRoom].GetListIndex() * 0x3C);

		if (!BIC || (BIC > 0x7FFFFF)) {
			MessageBox(hwnd, "Uh so something fucked up.", "Boomb", MB_OK);
		}
		else {


			SaveLevel(GBA.ROM, BIC);
			UpdateHeaderControls();
			UiState::stateManager->UpdateMapObjectWindow();
		}
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
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
		}
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
	case chkViewB:
	case chkViewF:
	case chkViewL:
	case chkViewBG:
		if (!LoadingLevel)
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);
		break;
	case chkSRe:
		GlobalVars::gblVars->checkBoxsMove.value(0);
		break;
	case chkSMove:
		GlobalVars::gblVars->checkBoxsResize.value(0);
		break;

	case chkVC:

		RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, GlobalVars::gblVars->BGImage, true, true, true, false, false, false, -1);

		break;
	case cboArea:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{

			fclose(GBA.ROM);
			area = Combos[cArea].GetListIndex();
			GBA.ROM = fopen(GBA.FileLoc, "r+b");
			if (currentRomType == 0)
			{

				if (area == 6)
				{
					LevelCounter = 0x70;
				}
				else
				{

					LevelCounter = (RD1Engine::theGame->RoomOffsets[area + 1] - RD1Engine::theGame->RoomOffsets[area]) / 0x3C;

				}
			}
			else if (currentRomType == 1)
			{

				if (area == 9)
				{
					LevelCounter = 3;
				}
				else if (area == 4)
				{
					LevelCounter = 0x34;
				}
				else
				{

					LevelCounter = (RD1Engine::theGame->RoomOffsets[area + 1] - RD1Engine::theGame->RoomOffsets[area]) / 0x3C;
				}

			}
			Combos[cRoom].Clear();

			for (i = 0; i < LevelCounter; i++)
			{
				sprintf(cBuf, "%x", i);
				Combos[cRoom].Additem(cBuf);

			}



			if (DefRoom >= LevelCounter)
				Combos[cRoom].SetListIndex(LevelCounter);
			else
				Combos[cRoom].SetListIndex(DefRoom);

			PostMessage(hwnd, WM_COMMAND, 0x000103ef, 0); // Simulate room combo 
														  // clicking.

			break;
	case cboRoom:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			int room = Combos[cRoom].GetListIndex();
			Combos[cSpriteSet].SetListIndex(0);
			RD1Engine::theGame->LoadRoom(Combos[cArea].GetListIndex(), Combos[cRoom].GetListIndex());
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
			LoadScrollControls(RD1Engine::theGame->mgrScrolls->GetScrollInfo());
			UpdateHeaderControls();
			DrawLevel();
			MyTSAEditor.DrawThisTileset();
			MyTSAEditor.LoadTSA();
			loadit = true;
			//SendMessage(hwnd, WM_COMMAND, 0x00010408, 0); // Update cboSprite 
			//											  // with SelChange

			LoadHeaderControls();
			//for (i = 0; i < 16; i++)
			//   ClipBoard.Erase(i, 0xFFFF);
			LoadingLevel = 0;

			InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
			SendMessage(UiState::stateManager->GetMapWindow(), WM_SIZE, 0, 1); // If lparam is 0 nothing

			RECT            t;
			TabResize();
			GetWindowRect(UiState::stateManager->GetMapWindow(), &t);
			MoveWindow(UiState::stateManager->GetMapWindow(), t.left, t.top, 512, 496, 1);
			UiState::stateManager->UpdateMapObjectWindow();
		
			InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
			//InvalidateRect(MiniMapClass::miniMapEditor->hwndMiniMap, 0, 1);
		}
		}
		break;
	default:
		return 0;
	}

	return 0;
}


BOOL CALLBACK  fraMainProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
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
		GlobalVars::gblVars->frameControls = hwnd;
		CurObjT = 0;
		CurObjNo = 0;



		clrIndex = 0;
		//GlobalVars::gblVars->imgTileset->Create(512, 1024);

		GlobalVars::gblVars->checkBoxForeground.SetCnt(GetDlgItem(hwnd, chkForeground));
		GlobalVars::gblVars->checkBoxLevel.SetCnt(GetDlgItem(hwnd, chkLevel));
		GlobalVars::gblVars->checkBoxLevel.value(2);
		GlobalVars::gblVars->ViewClip.SetCnt(GetDlgItem(hwnd, chkVC));
		GlobalVars::gblVars->checkBoxBackground.SetCnt(GetDlgItem(hwnd, chkBacklayer));
		GlobalVars::gblVars->ScrollCheck.SetCnt(GetDlgItem(hwnd, chkScroll));
		GlobalVars::gblVars->checkBoxClip.SetCnt(GetDlgItem(hwnd, chkClip));
		GlobalVars::gblVars->checkBoxViewF.SetCnt(GetDlgItem(hwnd, chkViewF));
		GlobalVars::gblVars->checkBoxViewL.SetCnt(GetDlgItem(hwnd, chkViewL));
		GlobalVars::gblVars->checkBoxViewB.SetCnt(GetDlgItem(hwnd, chkViewB));
		GlobalVars::gblVars->checkBoxViewBL.SetCnt(GetDlgItem(hwnd, chkViewBG));
		GlobalVars::gblVars->checkBoxsMove.SetCnt(GetDlgItem(hwnd, chkSMove));
		GlobalVars::gblVars->checkBoxsResize.SetCnt(GetDlgItem(hwnd, chkSRe));
		GlobalVars::gblVars->checkBoxsView.SetCnt(GetDlgItem(hwnd, chkSView));
		GlobalVars::gblVars->checkBoxshowmap.SetCnt(GetDlgItem(hwnd, chkSM));
		GlobalVars::gblVars->checkBoxshowtileset.SetCnt(GetDlgItem(hwnd, chkST));
		GlobalVars::gblVars->checkBoxchkES.SetCnt(GetDlgItem(hwnd, chkSprites));
		GlobalVars::gblVars->chkBoxED.SetCnt(GetDlgItem(hwnd, chkDoors));

		// GlobalVars::gblVars->CheckBoxes[chkESC].SetCnt(GetDlgItem(Main,chkSprites));
		GlobalVars::gblVars->chkMC[0].SetCnt(GetDlgItem(hwnd, chkForeground2));
		GlobalVars::gblVars->chkMC[1].SetCnt(GetDlgItem(hwnd, chkLevel2));
		GlobalVars::gblVars->chkMC[2].SetCnt(GetDlgItem(hwnd, chkBacklayer2));
		GlobalVars::gblVars->chkMC[3].SetCnt(GetDlgItem(hwnd, chkClip2));
		chkDoTrans.SetCnt(GetDlgItem(hwnd, chkDTrans));
		chkDoTrans.value(1);
		GlobalVars::gblVars->checkBoxshowmap.value(1);
		GlobalVars::gblVars->checkBoxshowtileset.value(1);





		//CreateDialog(hGlobal, MAKEINTRESOURCE(frmSceneryEditor), 0, SceneProc);

		//

		//CreateDialog(hGlobal, MAKEINTRESOURCE(fraTBE), 0, TSAProc);

		//CreateDialog(hGlobal, MAKEINTRESOURCE(frmClipboard), 0, ClipBoardProc);
		//CreateDialog(hGlobal, MAKEINTRESOURCE(fraBGI), 0, BGiProc);
		//CreateDialog(hGlobal, MAKEINTRESOURCE(frmLPE), 0, LPProc);


		for (int i = 0; i < 3; i++)
		{
			GlobalVars::gblVars->scrColors[i].create(GetDlgItem(hwndLPE, wndrs[i]), 0, 255);
			GlobalVars::gblVars->scrColors[i].ChangeScrollbars();
			// SetScrollPos( UiState::stateManager->GetTilesetWindow(), SB_HORZ, 0, 1 );

		}

		Combos[1].Init(GetDlgItem(hwnd, (int)MAKEINTRESOURCE(IDC_COMBO1))); // GetDlgItem(hwnd,(int)MAKEINTRESOURCE(res))
		if (Combos[1].val != -1)
		{
			Combos[1].SetListIndex(Combos[1].val);
		}
		cboScroll.Init(GetDlgItem(hwnd, cboDScroll));
		cboScroll.Disable();
		Combos[cArea].Init(GetDlgItem(hwnd, (int)MAKEINTRESOURCE(cboArea)));
		Combos[cRoom].Init(GetDlgItem(hwnd, (int)MAKEINTRESOURCE(cboRoom)));
		Combos[cClip].Init(GetDlgItem(hwnd, (int)MAKEINTRESOURCE(cboClip)));
		Combos[cSpriteSet].Init(GetDlgItem(hwnd, (int)MAKEINTRESOURCE(cboSpriteSet)));
		Combos[cClip].Additem("Clipdata");
		Combos[cClip].SetListIndex(0);
		Combos[cSpriteSet].Additem("0");
		Combos[cSpriteSet].SetListIndex(0);
		Combos[cRoom].Additem("Rooms");
		Combos[cRoom].SetListIndex(0);

		Combos[cArea].Additem("Areas");
		Combos[cArea].SetListIndex(0);

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

		break;
	
	case  WM_WINDOWPOSCHANGED:
	


		break;
	case BN_CLICKED:

		break;
	case WM_COMMAND:

		HandleDetections2(hwnd, message, wParam, lParam);
		break;

	case WM_VSCROLL: // exact same idea, but V scroll instead of H scroll

					 // if((HWND)lParam == GetDlgItem(hwnd,vsbMap))
					 // UpdateScroll(hwnd,wParam,1,vsbMap,sVMap);

		break;

	}

	return 0;
}

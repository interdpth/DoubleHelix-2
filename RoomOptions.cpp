#include "MainHeader.h"
#include "GlobalVars.h"
#include "BaseGame.h"
int UpdateHeaderControls();
void ClearLayer(nMapBuffer* clearLayer) {
	for (int i = 0; i < clearLayer->X* clearLayer->Y; i++) {
		clearLayer->TileBuf2D[i] = 0x00;
	}
	clearLayer->Dirty = 1;
	clearLayer->SDirty = 1;
}

//stores a layer to a buffer, and copies the proper portion, idk if it works right now.
void ResizeLayer(nMapBuffer* buffLayer, int newWidth, int newHeight) {
	unsigned long copysize = 0;
	unsigned short *roombuff = new unsigned short[0x10000];
	if (buffLayer->X*buffLayer->Y >= newWidth*newHeight) {
		copysize = newWidth*newHeight;
	}
	else {
		copysize = buffLayer->X*buffLayer->Y;
	}
	memcpy(buffLayer->TileBuf2D, roombuff, copysize);
	buffLayer->X = newWidth;
	buffLayer->Y = newHeight;
    
	delete[] buffLayer->TileBuf2D;
	buffLayer->TileBuf2D = new unsigned short[copysize / 2];
	memcpy(roombuff, buffLayer->TileBuf2D, copysize);
	delete[]roombuff;

}

//Windows procedure for this window
BOOL CALLBACK  ExtendedProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {
	char buf[256];
	unsigned int X = 0;
	unsigned int Y = 0;
	unsigned char Width = 0;
	unsigned char Height = 0;
	int i = 0;

	nMapBuffer* buffForeground;
	nMapBuffer* buffLevel;
	nMapBuffer* buffBackLayer;
	nMapBuffer* buffClipLayer;
	if (RD1Engine::theGame &&RD1Engine::theGame->mainRoom&& RD1Engine::theGame->mainRoom->mapMgr) {
		buffForeground = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround);
		buffLevel = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData);
		buffBackLayer = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer);
		buffClipLayer = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata);
	}
	switch (message)
	{
	case WM_INITDIALOG:
		ExtendedOptWND = hWnd;
		LayerCombo.Clear();
		LayerCombo.Init(GetDlgItem(hWnd, cboLayer));
		LayerCombo.Additem("Foreground");
		LayerCombo.Additem("Level Data");
		LayerCombo.Additem("Back Layer");
		LayerCombo.Additem("Clip Data");
		LayerCombo.SetListIndex(0);
		break;


	case WM_LBUTTONDOWN:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case cmdROSave:
			MessageBox(hWnd, "Automatically Saving the Room and applying effects", "Warning", MB_OK);

			GetWindowText(GetDlgItem(hWnd, txtWidth), buf, 8);
			sscanf(buf, "%x", &X);
			GetWindowText(GetDlgItem(hWnd, txtHeight), buf, 8);
			sscanf(buf, "%x", &Y);
			//make a new copy of RoomBuff the size of the new Width and Height

			//Fill it with zeroes

			i = LayerCombo.GetListIndex();
			if (i == 0) {


				ResizeLayer(buffForeground, X, Y);
				//copy the old level 

				SaveLevel(-1);
				UpdateHeaderControls();
				return 0;
			}
			else if (i == 1) {

				ResizeLayer(buffLevel, X, Y);
				SaveLevel(-1);
				return 0;
			}
			else if (i == 2) {
				ResizeLayer(buffBackLayer, X, Y);
				SaveLevel(-1);
				return 0;
			}
			else if (i == 3) {
				ResizeLayer(buffClipLayer, X, Y);

				SaveLevel(-1);
				return 0;
			}
			//Safety Measure

			break;
		case cboLayer:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				i = LayerCombo.GetListIndex();
				if (i == 0) {//Foreground
					if (RD1Engine::theGame->mainRoom->roomHeader.bBg0 == 0x10) {
						sprintf(buf, "%X", buffForeground->X);
						SetWindowText(GetDlgItem(hWnd, txtWidth), buf);
						sprintf(buf, "%X", buffForeground->Y);
						SetWindowText(GetDlgItem(hWnd, txtHeight), buf);
					}
					else {
						LayerCombo.SetListIndex(1);
						SendMessage(hWnd, WM_COMMAND, wParam, lParam);
					}

				}
				else if (i == 1) {//Level Data

					if (RD1Engine::theGame->mainRoom->roomHeader.bBg1 == 0x10) {
						sprintf(buf, "%X", buffLevel->X);
						SetWindowText(GetDlgItem(hWnd, txtWidth), buf);
						sprintf(buf, "%X", buffLevel->Y);
						SetWindowText(GetDlgItem(hWnd, txtHeight), buf);
					}
					else {
						LayerCombo.SetListIndex(2);
						SendMessage(hWnd, WM_COMMAND, wParam, lParam);
					}
				}
				else if (i == 2) {//Back Layer
					if (RD1Engine::theGame->mainRoom->roomHeader.bBg2 == 0x10) {
						sprintf(buf, "%X", buffBackLayer->X);
						SetWindowText(GetDlgItem(hWnd, txtWidth), buf);
						sprintf(buf, "%X", buffBackLayer->Y);
						SetWindowText(GetDlgItem(hWnd, txtHeight), buf);
					}
					else {
						LayerCombo.SetListIndex(3);
						SendMessage(hWnd, WM_COMMAND, wParam, lParam);
					}
				}
				else if (i == 3) {//Clip Data
					sprintf(buf, "%X", RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata)->X);
					SetWindowText(GetDlgItem(hWnd, txtWidth), buf);
					sprintf(buf, "%X", RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata)->Y);
					SetWindowText(GetDlgItem(hWnd, txtHeight), buf);
				}

			}
			break;

		case cmdCFore:
			ClearLayer(buffForeground);

			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			break;
		case cmdCLevel:
			ClearLayer(buffLevel);

			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
			break;
		case cmdCBack:
			ClearLayer(buffBackLayer);

			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage,&GlobalVars::gblVars->BGImage, 0);
			break;
		case cmdCClip:

			ClearLayer(buffClipLayer);

			break;


		case IDCANCEL:
			ShowWindow(hWnd, 0);
			break;
		}


		break;

		if ((RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->Dirty) || (RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Dirty) || (buffForeground->Dirty))
			RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);




	}

	//	return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}

int LoadExtensions()
{

	//Enable All
	//	SendMessage(GetDlgItem( ExtendedOptWND,cmdCFore),WM_ENABLE

	if (RD1Engine::theGame->mainRoom->roomHeader.bBg0) {
		EnableWindow(GetDlgItem(ExtendedOptWND, cmdCFore), 1);

	}
	else {
		EnableWindow(GetDlgItem(ExtendedOptWND, cmdCFore), 0);
	}
	if (RD1Engine::theGame->mainRoom->roomHeader.bBg1) {
		EnableWindow(GetDlgItem(ExtendedOptWND, cmdCLevel), 1);
	}
	else {
		EnableWindow(GetDlgItem(ExtendedOptWND, cmdCLevel), 0);
	}
	if (RD1Engine::theGame->mainRoom->roomHeader.bBg2) {
		EnableWindow(GetDlgItem(ExtendedOptWND, cmdCBack), 1);
	}
	else {
		EnableWindow(GetDlgItem(ExtendedOptWND, cmdCBack), 0);
	}


	//if(BaseGame::theGame->mainRoom->roomHeader.bBg3)

	return 0;
}



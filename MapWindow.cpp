#include "MainHeader.h"
#include "MapManager.h"
#include "GlobalVars.h"
#include "DoorManager.h"
#include "MapEditingState.h"
#include "clsRoomScrolls.h"
#include "TilesetManager.h"
#include "UiState.h"
#include "MapUtils.h"
RECT toolsRect;
extern HWND hTabControl;
extern HANDLE handle_out;
void LoadScrollInfo(int s, Scroller *scroll);

//Returns the cfurrent editable buffer.
nMapBuffer* GetActiveBuffer() {
	nMapBuffer* tmpBuf = NULL;
	if (GlobalVars::gblVars->checkBoxForeground.value() == 1 || GlobalVars::gblVars->chkMC[0].value() == 1)
	{
		tmpBuf = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround);
	}
	else if (GlobalVars::gblVars->checkBoxLevel.value() == 1 || GlobalVars::gblVars->chkMC[1].value() == 1)
	{
		tmpBuf = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData);
	}
	else if (GlobalVars::gblVars->checkBoxBackground.value() == 1 || GlobalVars::gblVars->chkMC[2].value() == 1)
	{
		tmpBuf = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer);
	}
	else if (GlobalVars::gblVars->checkBoxClip.value() == 1 || GlobalVars::gblVars->chkMC[3].value() == 1)
	{
		tmpBuf = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata);
	}

	return tmpBuf;
}
void UpdateStatusText(int actualX, int actualY)
{
	MapManager* mgrMap = NULL;
	RoomClass* theRoom = NULL;
	char cBuf[256] = { 0 };
	char buffer[512] = { 0 };
	if (RD1Engine::theGame != NULL&&RD1Engine::theGame->mainRoom != NULL)
	{
		theRoom = RD1Engine::theGame->mainRoom;
		mgrMap = theRoom->mapMgr;
	}
	std::string tmp;
	sprintf(buffer, "X: %2X, Y: %2X | Selection %X x %X |", actualX, actualY, mpMap.Width, mpMap.Height);
	if (!mgrMap->GetLayer(4)->TileBuf2D == NULL)
	{
		int poo = cboClipData.GetListIndex();

		cboClipData.SetListIndex(mgrMap->GetLayer(MapManager::Clipdata)->TileBuf2D[actualX + (actualY)* (mgrMap->GetLayer(MapManager::LevelData)->X)]);

		GetWindowText(cboClipData.GetHwnd(), cBuf, 200);


		sprintf(buffer, "%s Clipdata: %s", buffer, cBuf);

		SetWindowText(UiState::stateManager->StatusBar, buffer);

		//cboClipData.SetListIndex(poo);

	}
}

//Handles drawing the room
void Draw(HDC hdc)
{
	RECT viewRect;
	GetWindowRect(UiState::stateManager->GetMapWindow(), &viewRect);


	MapManager* mgrMap;
	if (RD1Engine::theGame != NULL&&RD1Engine::theGame->mainRoom != NULL)
	{
		mgrMap = RD1Engine::theGame->mainRoom->mapMgr;
		nMapBuffer* clipLayer = mgrMap->GetLayer(3);
		if (clipLayer) {


			if (viewRect.right > clipLayer->X * 16)
			{
				viewRect.right = clipLayer->X * 16;
			}

			if (viewRect.bottom > clipLayer->Y * 16)
			{
				viewRect.bottom = clipLayer->Y * 16;
			}
		}
	}



	if (viewRect.right < 64)
	{
		viewRect.right = 64;
	}
	if (viewRect.bottom < 64)
	{
		viewRect.bottom = 64;
	}


	float z = (float)GlobalVars::gblVars->zoomLevel;
	if (z < 1.0)
	{
		z = 1.0;
	}
	float adjustWidth = (float)((float)viewRect.right *z);
	float adjustHeight = (float)((float)viewRect.bottom*z);
	int w = (int)(adjustWidth);
	int h = (int)(adjustHeight);
	StretchBlt(hdc, 0, 0, (int)adjustWidth, (int)adjustHeight, RD1Engine::theGame->ThisBackBuffer.DC(),

		RD1Engine::theGame->mainRoom->currentHorizScroll * 16, RD1Engine::theGame->mainRoom->currentVertScroll * 16, viewRect.right, viewRect.bottom, SRCCOPY);

	if (GlobalVars::gblVars->ViewClip.value() != 1)
	{
		DrawTileRect(hdc, mpMap, 16);
	}
}
MapUtils* utils;
void HandleCopyPasteMessages(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	int copypasteflag = 0;
	switch (message)
	{

	case WM_COPYDATA:
	case WM_COPY:
		copypasteflag = 1;
		break;
	case WM_PASTE:
		copypasteflag = 1;
		break;
	}
}

void HandleScrolling(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	RoomClass* theRoom = NULL;
	if (RD1Engine::theGame != NULL&&RD1Engine::theGame->mainRoom != NULL)
	{
		theRoom = RD1Engine::theGame->mainRoom;

	}
	switch (message)
	{

	case WM_VSCROLL:	// exact same idea, but V scroll instead of H scrollFindMeAScroll
		MapVertScroll->UpdateScroll(wParam, 1, vsbMap, sVMap);
		if (theRoom != NULL)
		{
			RD1Engine::theGame->mainRoom->currentVertScroll = MapVertScroll->GetIndex();
		}
		InvalidateRect(hWnd, 0, 1);
		break;
	case WM_HSCROLL:
		MapHorizScroll->UpdateScroll(wParam, NULL, hsbMap, sHMap);
		if (theRoom != NULL)
		{
			RD1Engine::theGame->mainRoom->currentHorizScroll = MapHorizScroll->GetIndex();
		}
		InvalidateRect(hWnd, 0, 1);
	}
}
void HandleMouseUpDown(int actualX, int actualY, HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	MapManager* mgrMap = NULL;
	RoomClass* theRoom = NULL;
	if (RD1Engine::theGame != NULL&&RD1Engine::theGame->mainRoom != NULL)
	{
		theRoom = RD1Engine::theGame->mainRoom;
		mgrMap = theRoom->mapMgr;
	}
	if (mgrMap == NULL || !mgrMap->created)
	{
		return;
	}

	switch (message)
	{

	case WM_LBUTTONDOWN:
		if (wParam & MK_LBUTTON)
		{
			editingStates thisState = mgrMap->GetState()->GetState();//Wait what
			utils->HandleLeftClick(thisState, actualX, actualY, comboSpriteSet.GetListIndex(), wParam, lParam);
		}
		break;
	case WM_RBUTTONDOWN:

		if (LOWORD(wParam) == MK_RBUTTON)
		{

			editingStates thisState = mgrMap->GetState()->GetState();//Wait what
			utils->HandleRightClick(thisState, actualX, actualY, comboSpriteSet.GetListIndex());
		}
		break;
	case WM_RBUTTONUP:

		mgrMap->GetState()->SetAction(editingActions::PLACETILE);//Return to default action
		break;
	case WM_LBUTTONUP:
		mgrMap->GetState()->SetAction(editingActions::PLACETILE);
		break;
	}
}
#define theTimer 2242443242

//Update the sprites
bool UpdateSprites()
{

	bool updateWindow = false;
	if (!RD1Engine::theGame)
	{
		return false;
	}
	if (!GlobalVars::gblVars->chkAnimatez.GetCheckState())
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
	return updateWindow;
}


LRESULT CALLBACK MapProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{

	char *buffer = new char[256];
	memset(buffer, 0, 256);
	MapManager* mgrMap = NULL;
	RoomClass* theRoom = NULL;
	if (RD1Engine::theGame != NULL&&RD1Engine::theGame->mainRoom != NULL)
	{
		theRoom = RD1Engine::theGame->mainRoom;
		mgrMap = theRoom->mapMgr;
	}
	nMapBuffer* Layer;
	PAINTSTRUCT ps;
	HDC hdc;

	int dw, x, y, nx, ny;
	dw = 0;

	x = 0;
	y = 0;
	nx = 0;
	ny = 0;

	RECT Drawme;

	int curX = GetX(lParam) / 16;
	int curY = GetY(lParam) / 16;
	int actualX = curX;
	if (MapHorizScroll != NULL) {
		actualX += MapHorizScroll->GetIndex();
	}

	int actualY = curY;
	if (MapVertScroll != NULL) {
		actualY += MapVertScroll->GetIndex();
	}

	switch (message)
	{
	case WM_CREATE:
		utils = new MapUtils(mgrMap);
		GetWindowRect(GetDlgItem(UiState::stateManager->GetWindow(), grpMap), &toolsRect);
		MapHorizScroll = new WindowScrollbar(hWnd, false);
		MapVertScroll = new WindowScrollbar(hWnd, true);
		MapVertScroll->ChangeScrollbars(vsbMap, sVMap);
		MapHorizScroll->ChangeScrollbars(hsbMap, sHMap);
		SetTimer(hWnd, theTimer + 1, 15, (TIMERPROC)NULL);
		break;
	case WM_TIMER:
		//Update sprite animations and tileset
		//	TilesetManager::
		if (!RD1Engine::theGame || !RD1Engine::theGame->mainRoom)
		{
			break;
		}

		if (wParam == theTimer + 1)
		{
			if (UpdateSprites()) {
				if (RD1Engine::theGame&&RD1Engine::theGame->mainRoom&&RD1Engine::theGame->mainRoom->mapMgr)
				{
					RD1Engine::theGame->DrawStatus.dirty = true;
					RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);
					InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
				}
			}
		}
		break;
	case WM_PAINT:
		if (theRoom != NULL)
		{

			hdc = BeginPaint(hWnd, &ps);

			Draw(hdc);

			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_VSCROLL:
	case WM_HSCROLL:
		HandleScrolling(hWnd, message, wParam, lParam);
		break;
	case WM_COPYDATA:
	case WM_COPY:
	case WM_PASTE:
		HandleCopyPasteMessages(hWnd, message, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		HandleMouseUpDown(actualX, actualY, hWnd, message, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
	{
		if (mgrMap == NULL || !mgrMap->created)
		{
			break;
		}
		if (!RD1Engine::theGame || !RD1Engine::theGame->mainRoom || !RD1Engine::theGame->mainRoom->mapMgr || !RD1Engine::theGame->mainRoom->mapMgr || !RD1Engine::theGame->mainRoom->mapMgr->created)
		{
			return 0;
		}
		editingStates thisState = mgrMap->GetState()->GetState();
		editingActions thisAction = mgrMap->GetState()->GetAction();
		if (thisAction == editingActions::RESIZE && LOWORD(wParam) == MK_RBUTTON) {
			if (thisState == editingStates::SCROLL) {

				int scrollid = RD1Engine::theGame->mgrScrolls->Findmeascroll(actualX, actualY, cboScroll.GetListIndex());
				if (scrollid != -1)
				{
					RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(scrollid);
				}
				UiState::stateManager->ShowObj();
			}
			RD1Engine::theGame->mainRoom->mapMgr->Resize(thisState, thisAction, wParam, lParam, &mpMap);
		}
		else if (LOWORD(wParam) == MK_LBUTTON) {
			RD1Engine::theGame->mainRoom->mapMgr->MoveObject(lParam);
		}
		else
		{
			mpMap.sY = curY;
			mpMap.sX = curX;

		}
		InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
		//	UiState::stateManager->UpdateMapObjectWindow(); 
		UpdateStatusText(actualX, actualY);
		if (wParam == MK_LBUTTON)
		{
			SendMessage(hWnd, 0x201, wParam, lParam);
		}

	}



	break;
	case WM_RBUTTONDBLCLK:
		///Reset the rects 

		UiState::stateManager->ResetCursor();
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
		InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
		break;

	case WM_KEYDOWN:							// Here's what you're interested in! -- check to see if someone preGlobalVars::gblVars->SSEd a key.
												//c=67
												//ctrl=17
												//p=80

		if (wParam&VK_CONTROL) {


			switch (wParam)
			{
				/*	case 'Z':
				sprintf(buffer, "Map-Undo's left: %d", Layer->UndoBuff->UndoNum);
				SetWindowText(UiState::stateManager->GetMapWindow(), buffer);


				Layer->UndoBuff->Undo();


				Drawme.top = 0;
				Drawme.left = 0;
				Drawme.bottom = BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y;
				Drawme.right = BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X;
				Layer->DrawRect.top = 0;
				Layer->DrawRect.left = 0;
				Layer->DrawRect.bottom = BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y;
				Layer->DrawRect.right = BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X;


				return TRUE;
				*/
			case 'X':


				return TRUE;

			case 'C':
				Layer = GetActiveBuffer();
				SetWindowText(UiState::stateManager->GetMapWindow(), "Map-Ctrl + C");
				Drawme.top = mpMap.sY;
				Drawme.left = mpMap.sX;
				Drawme.bottom = Drawme.top + mpMap.Height;
				Drawme.right = Drawme.left + mpMap.Width;
				Layer->CopyData->Copy(Layer->X, Drawme, Layer->TileBuf2D);


				return TRUE;

			case 'V':
				Layer = GetActiveBuffer();
				SetWindowText(UiState::stateManager->GetMapWindow(), "Map- Ctrl + v");
				Layer->UndoBuff->Set(Layer->X*Layer->Y * 2, Layer->TileBuf2D);
				Layer->CopyData->Paste(&Layer->TileBuf2D[mpMap.sX + (mpMap.sY*Layer->X)]); //mpMap.sX+( mpMap.sY*Layer->X)]);

				RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->TileImage, &GlobalVars::gblVars->BGImage, -1);

				InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 0);
				return TRUE;

			case 'A':


				return TRUE;
			}


		}

		// set the key that person preGlobalVars::gblVars->SSEd as true.
		return 0;


		break;

	case WM_SIZE:
		//Gotta Make sure the size isn't out of bounds.
		if (lParam != 0) {
			UiState::stateManager->ResizeMap(hTabControl);
		}
		break;
	case WM_DESTROY:

		break;

	}
	delete[] buffer;
	return DefWindowProc(hWnd, message, wParam, lParam);
}

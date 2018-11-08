
#include "UIState.h"
WNDCLASSEX wcTileset;
HINSTANCE hMap;
WNDCLASSEX wcMap;
HINSTANCE hTileset;
extern HWND hTabControl;
BOOL CALLBACK   DialogProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DwProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SSProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
WINUSERAPI
INT_PTR
WINAPI
DialogBoxParamA(
	_In_opt_ HINSTANCE hInstance,
	_In_ LPCSTR lpTemplateName,
	_In_opt_ HWND hWndParent,
	_In_opt_ DLGPROC lpDialogFunc,
	_In_ LPARAM dwInitParam);
class DoorManager;
UiState* UiState::stateManager;

UiState::UiState()
{
	_theWindow = NULL;
	_mapWindow = NULL;
	_tilesetWindow = NULL;
	_theState = WindowState::SINGLE;
}

UiState::UiState(WindowState newState)
{
	_theWindow = NULL;
	_mapWindow = NULL;
	_tilesetWindow = NULL;
	_theState = newState;
}



UiState::~UiState()
{
}

WindowState UiState::GetWindowState()
{
	return _theState;

}

void UiState::SetWindow(HWND thisWindow)
{
	_theWindow = thisWindow;
	SetWindowState(_theState);

}

WindowState UiState :: SetWindowState(WindowState theState)
{
	_theState = theState;
	
	return _theState;
}

HWND UiState ::GetWindow()
{
	return _theWindow;
}

void UiState::MakeWindow()
{
	DestroyWindow(_theWindow);
	if(_theState == WindowState::MULTI)
	{
		_theString = MAKEINTRESOURCE(frmMain);
	}
	else if(_theState == WindowState::EXPERT)	
	{
		_theString = MAKEINTRESOURCE(frmMainExperiment);
	}else
	{
		_theString = MAKEINTRESOURCE(frmMain1);
	}
}

void UiState::UpdateWindow()
{
	MakeWindow();	
	DialogBoxParamA(hGlobal, _theString, 0, DialogProc ,0L);
}


void UiState::ShowObj() {
	
	return;
	if (!RD1Engine::theGame || !RD1Engine::theGame->mainRoom || !RD1Engine::theGame->mainRoom || !RD1Engine::theGame->mainRoom->mapMgr->created)
	{
		return;
	}
	editingStates thisState = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetState();;

	HWND hwndMove;
	int Height;
	int Width;
	int X;
	int Y;


	hwndMove = MapDataWIn;
	Width = 390;
	Height = 300;
	X = 440;
	Y = 24;

	if (thisState == editingStates::DOOR) {
		hwndMove = DoorWin;
		Width = 450;
		Height = 250;
		X = 440;
		Y = 32;
	}
	else if (thisState == editingStates::SCROLL) {

		hwndMove = ScrollWIn;
		Width = 450;
		Height = 250;
		X = 440;
		Y = 32;
	}
	else if (thisState == editingStates::SPRITE) {
		hwndMove = hwndSS;
		Width = 370;
		Height = 300;
		X = 440;
		Y = 24;
	}
	else {
		/*ShowWindow(DoorWin, SW_HIDE);
		ShowWindow(ScrollWIn, SW_HIDE);
		ShowWindow(hwndSS, SW_HIDE);*/
	}
	//Show the chosen one.
	ShowWindow(hwndMove, SW_SHOW);
	ShowObjWindow(hwndMove, X, Y, Width, Height);
}

void UiState::ShowObjWindow(HWND AffectedHwnd, int x, int y, int OriginalWidth, int OriginalHeight) {
	//RECT windowRect;
	//RECT MainWindowRect;
	RECT thisWindowRect;
	//GetWindowRect(Main, &windowRect);
	//GetWindowRect(GetDlgItem(Main, grpBoxObj), &MainWindowRect);

	GetWindowRect(AffectedHwnd, &thisWindowRect);


	MoveWindow(AffectedHwnd, x, y, OriginalWidth, OriginalHeight, true);
}

void UiState::UpdateMapObjectWindow()
{
	if (!RD1Engine::theGame->mainRoom->mapMgr->created)
	{
		return;
	}
	char *buffer = new char[1000];
	sprintf(buffer, "%02X", RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[Combos[cSpriteSet].GetListIndex()].Max());
	SetWindowText(GetDlgItem(MapDataWIn, lblSpriteObjectCount), buffer);
	sprintf(buffer, "%02X", RD1Engine::theGame->mgrScrolls->GetScrollInfo()->Scrolls.size());
	SetWindowText(GetDlgItem(MapDataWIn, lblCurLabelCount), buffer);
	sprintf(buffer, "%02X", RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes.size());
	SetWindowText(GetDlgItem(MapDataWIn, lblCurDoorCount), buffer);

	long BytesUsed = 0;
	int i = 0;
	for (i = 0; i < 4; i++) {
		BytesUsed += RD1Engine::theGame->mainRoom->mapMgr->GetLayer(i)->oSize;
	}
	sprintf(buffer, "%02X", BytesUsed);
	SetWindowText(GetDlgItem(MapDataWIn, lblOldCompressed), buffer);
	BytesUsed = 0;
	for (i = 0; i < 4; i++) {
		BytesUsed += (RD1Engine::theGame->mainRoom->mapMgr->GetLayer(i)->newX*RD1Engine::theGame->mainRoom->mapMgr->GetLayer(i)->newY) * 2;
	}
	SetWindowText(GetDlgItem(MapDataWIn, lblCurrentSize), buffer);

	delete[] buffer;

}

int  UiState::InitTileset() {
	// Fill in the window class structure with parameters 
	// that describe the main window. 

	wcTileset.cbSize = sizeof(wcTileset);		   // size of structure 
	wcTileset.style = CS_DBLCLKS | CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	wcTileset.lpfnWndProc = TilesetProc;	 // points to window procedure 
	wcTileset.cbClsExtra = NULL;				// no extra class memory 
	wcTileset.cbWndExtra = NULL; 			   // no extra window memory 
	wcTileset.hInstance = hGlobal;		   // handle to instance 
	wcTileset.hIcon = NULL; 			 // predefined app. icon 
	wcTileset.hCursor = NULL;				// predefined arrow 
											// wcTileset.hbrBackground = GetStockObject( WHITE_BRUSH);				   // white background brush 
	wcTileset.lpszMenuName = NULL;    // name of menu resource 
	wcTileset.lpszClassName = "TilesetClass";  // name of window class 
	HWND targetWnd = 0L;
	int flags = WS_VSCROLL | WS_VISIBLE;
	RegisterClassEx(&wcTileset);

	DestroyWindow(_tilesetWindow);
	int x = 0;
	int y = 0;
	int width = 280;
	int height = 234;
	if (_theState != WindowState::MULTI)
	{
		targetWnd = _theWindow;
		x = 13;
		y =350;
		flags |= WS_CHILD;
	}
	else
	{
		x = 440;
		y = 180;
		flags |= WS_BORDER | WS_TABSTOP;
	}
	_tilesetWindow = CreateWindowEx(NULL, "TilesetClass", "", flags, x, y, width, height, targetWnd, 0, hGlobal, 0);


	ShowWindow(UiState::stateManager->GetTilesetWindow(), SW_SHOW);
	return 0;
}


int UiState::InitMap()
{
	// Fill in the window class structure with parameters 
	// that describe the main window. 

	wcMap.cbSize = sizeof(wcMap);		   // size of structure 
	wcMap.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW; 				   // redraw if size changes 
	wcMap.lpfnWndProc = MapProc;	  // points to window procedure 
	wcMap.cbClsExtra = NULL;				// no extra class memory 
	wcMap.cbWndExtra = NULL; 			   // no extra window memory 
	wcMap.hInstance = hGlobal;		   // handle to instance 
	wcMap.hIcon = NULL; 			 // predefined app. icon 
	wcMap.hCursor = NULL;				// predefined arrow 
										// wcMap.hbrBackground = GetStockObject( WHITE_BRUSH);				   // white background brush 
	wcMap.lpszMenuName = NULL;    // name of menu resource 
	wcMap.lpszClassName = "MapClass";  // name of window class 
	HWND targetWnd=0;
	int flags = WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

									   // Register the window class. 

	RegisterClassEx(&wcMap);
	DestroyWindow(_mapWindow);
	int x = 0;
	int y = 0;
	int width = 280;
	int height = 234;
	if (_theState != WindowState::MULTI)
	{
		targetWnd = _theWindow;
		x = 450;
		y = 20;
		flags |= WS_CHILD;
	}
	else
	{
		x = 440;
		y = 200;
		flags |= WS_BORDER | WS_SIZEBOX | WS_TABSTOP;
	}
	//Create the Window
	_mapWindow = CreateWindowEx(NULL, "MapClass", "Map",  flags, x, y, width, height, targetWnd, NULL, hGlobal, 0);

	ShowWindow(_mapWindow, SW_SHOW);

	return 0;
}

HWND UiState::GetTilesetWindow()
{
	return _tilesetWindow;
}

HWND UiState::GetMapWindow()
{
	return _mapWindow;
}

void UiState::ResizeMap(HWND srcNeighbor)
{
	RECT viewRect;
	HWND hWnd = this->GetMapWindow();
	int x, y;
	MapManager* mgrMap = RD1Engine::theGame->mainRoom->mapMgr;
	RECT srcCoords;
	GetWindowRect(srcNeighbor, &srcCoords);
	if (this->GetWindowState() == WindowState::MULTI)
	{
		if (RD1Engine::theGame &&  RD1Engine::theGame->mainRoom&&mgrMap && mgrMap->created)
		{
			GetWindowRect(hWnd, &viewRect);
			viewRect.right = viewRect.right - viewRect.left;
			viewRect.bottom = viewRect.bottom - viewRect.top;
			x = mgrMap->GetLayer(MapManager::LevelData)->X;
			y = mgrMap->GetLayer(MapManager::LevelData)->Y;

			x = x > 4 * 16 ? x : 4 * 16;
			y = y > 4 * 16 ? y : 4 * 16;
			if ((viewRect.right / 16) + nVScroll[1] > x)
			{
				viewRect.right -= ((viewRect.right / 16) + nHScroll[sHMap] - x - 1) * 16;
				viewRect.right >>= 4;
				viewRect.right <<= 4;
			}
			if ((viewRect.bottom / 16) + nVScroll[sVMap] > y) {
				viewRect.bottom -= ((viewRect.bottom / 16) + nVScroll[sVMap] - x - 1) * 16;
				viewRect.bottom >>= 4;
				viewRect.bottom <<= 4;
			}





			//if((que.bottom/16)>BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y) que.bottom = (BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y / 16)*16 +16;//+20;
			MoveWindow(hWnd, srcCoords.right-srcCoords.left+32, 20, viewRect.right - srcCoords.left, viewRect.bottom-20, 1);//+16 is for scrollbar
																		  /*Take the current size
																		  Vertical (que.bottom/16
																		  We are displaying (que.bottom/16) tiles
																		  There's a total of BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y tiles
																		  */
																		  //GetWindowRect(hWnd,&que);
																		  //que.right = que.right - que.left;
																		  //que.bottom = que.bottom - que.top;
			nMaxHScroll[sHMap] = x - (viewRect.right / 16); // maximum H scroll


			nMaxVScroll[sVMap] = y - ((viewRect.bottom / 16));


			//UpdateScroll(hWnd, wParam, 1, vsbMap, sVMap);

			//UpdateScroll(hWnd, wParam, NULL, hsbMap, sHMap);
		}
	}
	else
	{
		if (RD1Engine::theGame &&  RD1Engine::theGame->mainRoom&&mgrMap && mgrMap->created)
		{
			GetWindowRect(hWnd, &viewRect);
			

			RECT toolsRect;
			RECT mainRect;
			GetWindowRect(hTabControl, &toolsRect);
			GetWindowRect(UiState::stateManager->GetWindow(), &mainRect);

			
			RECT viewRect;
			

			viewRect.top = 0;
			viewRect.left = toolsRect.right- mainRect.left;
			viewRect.right = mainRect.right - toolsRect.right;
			viewRect.bottom = mainRect.bottom - 16;


			MapManager* mgrMap;
			if (RD1Engine::theGame != NULL&&RD1Engine::theGame->mainRoom != NULL)
			{
				mgrMap = RD1Engine::theGame->mainRoom->mapMgr;
				nMapBuffer* c = mgrMap->GetLayer(3);
				if (c) {


					if (viewRect.right > c->X * 16)
					{
						viewRect.right = c->X * 16;
					}

					if (viewRect.bottom > c->Y * 16)
					{
						viewRect.bottom = c->Y * 16;
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


			x = mgrMap->GetLayer(MapManager::LevelData)->X;
			y = mgrMap->GetLayer(MapManager::LevelData)->Y;

			x = x > 4 * 16 ? x : 4 * 16;
			y = y > 4 * 16 ? y : 4 * 16;
			if ((viewRect.right / 16) + nVScroll[1] > x)
			{
				viewRect.right -= ((viewRect.right / 16) + nHScroll[sHMap] - x - 1) * 16;
				viewRect.right >>= 4;
				viewRect.right <<= 4;
			}
			if ((viewRect.bottom / 16) + nVScroll[sVMap] > y) {
				viewRect.bottom -= ((viewRect.bottom / 16) + nVScroll[sVMap] - x - 1) * 16;
				viewRect.bottom >>= 4;
				viewRect.bottom <<= 4;
			}

			//srcCoords.right - srcCoords.left + 32, 20, viewRect.right - srcCoords.left
			//if((que.bottom/16)>BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y) que.bottom = (BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y / 16)*16 +16;//+20;
			MoveWindow(hWnd, viewRect.left, 20, viewRect.right, viewRect.bottom - 16, 1);

			nMaxHScroll[sHMap] = x/16 - (viewRect.right / 16); // maximum H scroll


			nMaxVScroll[sVMap] = y / 16 - ((viewRect.bottom / 16));
		}
	}
}
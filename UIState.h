#pragma once
#include <Windows.h>
#include <WinUser.h>
#include "resource.h"
#include "MapEditingState.h"
#include "MapManager.h"
#include "SpriteObjectManager.h"
#include "clsRoomScrolls.h"
#include "DoorManager.h"
#include "BaseGame.h"
#include "GlobalVars.h"
//yuck
#include "globals.h"
extern HINSTANCE hGlobal;


extern HWND hwndSS;
extern  HWND DoorWin;
extern  HWND ScrollWIn;

extern  HWND MapDataWIn;
BOOL CALLBACK ScrollWndProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TilesetProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
enum WindowState
{
	SINGLE=0,
	MULTI=1,
	EXPERT=2
};


class UiState
{
public:
	UiState();
	UiState(WindowState newState);
	~UiState();
	static void MoveOrigin(HWND src, int x, int y, int width, int height, int refresh, RECT* origin);
	static void AutoRect(HWND src, RECT* tgt, bool zeroOut=false);
	void ResizeTileset(HWND srcNeighbor);
	static UiState* stateManager;
	void ShowObj();
	int InitMap();
	void ForceRedraw();
	void ResizeMap(HWND srcNeighbor);
	WindowState GetWindowState();
	WindowState SetWindowState(WindowState theState);
	void UpdateMapObjectWindow();
	void ShowObjWindow(HWND AffecTextEditorHwnd, int x, int y, int OriginalWidth, int OriginalHeight);
	void UpdateWindow();
	void ShowTilesetMap(bool set);
	HWND GetWindow();
	HWND StatusBar;
	void MakeWindow();

	void SetWindow(HWND window);
	int  InitTileset();
	HWND GetTilesetWindow();
	HWND GetMapWindow();

	void ResetCursor();
	
private:
	///Loads from config class
	void LoadState();
	void SaveState();
	WindowState _theState;
	HWND _theWindow;
	HWND _tilesetWindow;
	HWND _mapWindow;
	LPSTR _theString;
};
 


#pragma once
#include "MapEditingState.h"
#include "Basegame.h"
#include "Windows.h"
#include "Structures.h"
#include "MapManager.h"
#include "UiState.h"
#include "GlobalVars.h"
class MapUtils
{
private:
MapManager* _mapMgr;
public:
	MapUtils(MapManager*  mgr);
	~MapUtils();
	
	void HandleLeftClick(editingStates thisState, int mouseX, int mouseY, int spritelistindex, int wParam, int lParam);
	void HandleRightClick(editingStates thisState, int mouseX, int mouseY, int objlist);
	int             Gimmeasprite(int X, int Y,int objlist);
	int EditLayers(WPARAM wParam, LPARAM lParam);

	int             EditThisLayer(nMapBuffer * Layer, WPARAM wParam, LPARAM lParam, byte usens, unsigned short ToUse);
};



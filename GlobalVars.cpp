#include "GlobalVars.h"



GlobalVars::GlobalVars()
{
	frameControls = 0;
	frameTables = NULL;
	SSE=false;
	StatEditor = NULL;
	ReadObjectDetailsFromROM = false;
	
	memset(AppPath,0,1024);
	sec = new SamusEditorClass();
	OAMED = false;;

	TextEditor = new MetroidTextEditor();
	TileImage = new TileBuffer();
	imgTileset = NULL;

	BGi = new wndBGImport();
	zoomLevel = 1.0;
}


GlobalVars::~GlobalVars()
{
	delete frameTables;
	delete TextEditor;
	
	delete imgTileset;
	delete StatEditor;
	delete  BGi;
}

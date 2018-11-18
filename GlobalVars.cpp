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
	BGImage = new TileBuffer();
	TileImage = new TileBuffer();
	imgTileset = NULL;
//	StatEditor = new cStatEd(RD1Engine::theGame->currentRomType);
	BGi = new wndBGImport();
}


GlobalVars::~GlobalVars()
{
	delete frameTables;
	delete TextEditor;
	
	delete imgTileset;
	delete StatEditor;
	delete  BGi;
}

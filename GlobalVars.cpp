#include "GlobalVars.h"



GlobalVars::GlobalVars()
{
	frameControls = 0;
	frameTables = NULL;
	SSE=false;
	ReadObjectDetailsFromROM = false;
	
	memset(AppPath,0,1024);

	OAMED = false;;

	TextEditor = new MetroidTextEditor();
	
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

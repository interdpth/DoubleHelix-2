#pragma once
#include "clsScroll.h"
#include "sChecks.h"
#include "MinimapClass.h"
#include "cTextEditor.h"
#include "cStatEd.h"
#include "wndBGImport.h"

#include "OamFrameTable.h"
#include "..\Win32GBAImageClasses\backbuffer.h"
#include "cStatEd.h"
#include "SamusEditorClass.h"
#include "sMenuItem.h"
BOOL CALLBACK  TextEditorProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
class cOAMEdit;
class cStatEd;
class GlobalVars
{
public:
	GlobalVars();
	~GlobalVars();
	
	static GlobalVars * gblVars;
	TileBuffer     BGImage;
	TileBuffer*		TileImage;
	clsUIScroll scrColors[3];
	//sChecks    CheckBoxes[256];
	sChecks  chkMC[4];
	sChecks ScrollCheck;
	sChecks ViewClip;
	sChecks checkBoxForeground;
	sChecks checkBoxLevel;
	sChecks checkBoxBackground;
	sChecks checkBoxClip;
	sChecks chkBoxED;
	sChecks chkHideSprites;
	bool ViewForeground;
sMenuItem checkBoxViewF;
	bool ViewLevel;
	sMenuItem checkBoxViewL;
	bool ViewBacklayer;
	sMenuItem checkBoxViewB;
	sChecks checkBoxshowmap;
	sChecks checkBoxshowtileset;
	sMenuItem chkAnimatez;

	sMenuItem checkBoxViewBL;
	sMenuItem checkBoxsMove;
	sMenuItem checkBoxsResize;
	sMenuItem checkBoxsView;
	sMenuItem chkEditSprites;

    cStatEd* StatEditor;
	
	SamusEditorClass* sec;
	HWND frameControls;
	MetroidTextEditor* TextEditor;
	OamFrameTable* frameTables;
	//Image* imgMap;
	Image* imgTileset;
	
	wndBGImport*  BGi;
	float zoomLevel;
	bool SSE;
	bool ReadObjectDetailsFromROM;
	
	char AppPath[1024];

	bool 	OAMED;
	
};


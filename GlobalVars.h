#pragma once
#include "clsScroll.h"
#include "sChecks.h"
#include "mmD.h"
#include "cTextEditor.h"
#include "cStatEd.h"
#include "wndBGImport.h"
#include "cOAMEdit.h"
#include "OamFrameTable.h"
#include "backbuffer.h"
#include "cStatEd.h"
BOOL CALLBACK  TextEditorProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
class cOAMEdit;
class cStatEd;
class GlobalVars
{
public:
	GlobalVars();
	~GlobalVars();
	
	static GlobalVars * gblVars;
	TileBuffer*      BGImage;
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
	bool ViewForeground;
	sChecks checkBoxViewF;
	bool ViewLevel;
	sChecks checkBoxViewL;
	bool ViewBacklayer;
	sChecks checkBoxViewB;
	sChecks checkBoxshowmap;
	sChecks checkBoxshowtileset;


	sChecks checkBoxViewBL;
	sChecks checkBoxsMove;
	sChecks checkBoxsResize;
	sChecks checkBoxsView;
	sChecks checkBoxchkES;

    cStatEd* StatEditor;
	

	HWND frameControls;
	MetroidTextEditor* TextEditor;
	OamFrameTable* frameTables;
	//Image* imgMap;
	Image* imgTileset;
	
	wndBGImport*  BGi;
	bool SSE;
	bool ReadObjectDetailsFromROM;
	
	char AppPath[1024];

	bool 	OAMED;
	
};


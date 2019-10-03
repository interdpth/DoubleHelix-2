#pragma once
#include <Windows.h>
#include "SprGBuf.h"
#include "..\Win32GBAImageClasses\image.h"
#include "sList.h"
#include "sCombo.h"
#include "GlobalVars.h"
#include "GBAGraphics.h"
#include "cEnitityManager.h"

class cEntityManager;
class cSpriteSetEditor {//Move most of this class to it's own class....
public:
	HWND  me;
	sList SpriteSets;
	sList Population;
	Image vramImage;
	sCombo cboDetail;
	HWND  PalView;
	HWND  SprTilesView;
	int   Create(HINSTANCE mainInstance);
	SpriteObject* SpritePreview;
	GBAMethods* _gbaMethods;
	unsigned long     GUT;
	unsigned long     PUT;
	int SlightChange(int TitleChoice, unsigned char SpriteSetSelect, cEntityManager* mgr);
	int GetSetData(int TitleChoice, unsigned char SpriteSetSelection, cEntityManager* mgr);
	int GetSpritesPalAndTiles(bool romSwitch);
	int SetupPreview(SpriteObject* SprG, int TitleChoice);
	SpritesetData SpriteSetData;
	int SetInfo();
	TileBuffer SprGraphics;
	int CreateTileWindow(HINSTANCE mainInstance, WNDPROC proc);
	int CreatePalWindow(HINSTANCE myInstance, WNDPROC proc);
	int total;
	static cSpriteSetEditor *cSpriteSetEditor::SpriteSet;
	cSpriteSetEditor();
	~cSpriteSetEditor();
	
};


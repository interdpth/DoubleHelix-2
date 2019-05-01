#pragma once
#include <Windows.h>
#include "SprGBuf.h"
#include "image.h"
#include "sList.h"
#include "sCombo.h"
#include "GlobalVars.h"
#include "GBAGraphics.h"
#include "cEnitityManager.h"
#include "BaseGame.h"
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
	SprGBuf* SpritePreview;
	GBAMethods* _gbaMethods;
	unsigned long     GUT;
	unsigned long     PUT;
	int SlightChange(int TitleChoice, unsigned char SpriteSetSelect, cEntityManager* mgr);
	int GetSet(int TitleChoice, unsigned char SpriteSetSelection, cEntityManager* mgr);
	int DecodeSet(bool romSwitch);
	int SetupPreview(SprGBuf* SprG, int TitleChoice);
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


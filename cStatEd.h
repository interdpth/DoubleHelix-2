#pragma once
#include <windows.h>
#include "Structures.h"
#include "SprGBuf.h"
#include "GlobalVars.h"
#include "MemFile.h"

class cStatEd {
public:
	cStatEd(int crf);
	int LoadSprite();
	~cStatEd();
	int Switch();
	int InitDialog();
	int SetUpForZM();
	int SetUpForMF();
	int LoadEnemy();
	int SetupPreview();
	int SaveSprite();
	SprGBuf* SpritePreview;
	sList   SpriteList;
	ZMS     ZMSprite;
	MFS     FusionSprite;
	int currentRomType;
	HWND me;
	HWND Health;
	HWND Damage;
	sCombo Res1;
	sCombo Res2;
	//ZM stuff
	HWND  shp;
	HWND  lhp;
	HWND  m;
	HWND  sm;
	HWND  pb;
	//MF
	HWND  u1;
	HWND  u2;
	HWND  u3;
	HWND  ud;
	sChecks chkRes[8];

};

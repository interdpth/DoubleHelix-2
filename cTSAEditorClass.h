#pragma once
#include <windows.h>

#include "sChecks.h"
#include "sCombo.h"
#include "Structures.h"
#include "..\Win32GBAImageClasses\image.h"
struct tBlock {
	unsigned short Tile;
	unsigned short  HFlip;
	unsigned short VFlip;
	unsigned short  Pal;
};

class cTSAEditorClass {
public:
	cTSAEditorClass();
	~cTSAEditorClass();
	tBlock  CurrentBlock[4];
	sTilesetBlockStructure    blockset;

	HWND TilesetPic;

	HWND Preview;
	HWND tMain;
	Image Tileset;
	Image iPreview;
	Image thisTSA;
	sChecks HorChk[4];
	sChecks VerChk[4];
	sCombo cboPals[4];
	int Create();
	int DrawThisTileset();
	int DrawPreview();

	int LoadTSA();
	int CreateTilesetWind();
	int CreatePreviewWind();

	int curtsa;
	int SaveBlock();

};
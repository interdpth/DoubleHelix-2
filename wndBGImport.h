#pragma once
#include <Windows.h>
#include "..\Win32GBAImageClasses\image.h"
#include "..\RNA_UI\sCombo.h"
class wndBGImport {
public:
	wndBGImport();
	~wndBGImport();
	HWND me;
	Image Pic;
	long  PCPal[256];
	short GPal[256];
	unsigned char*gfxbuffer;
	unsigned char    palpos;
	unsigned short*tilemap;

	unsigned short   palsize;
	unsigned long   tilesize;
	unsigned long   mapsize;
	int   Type;
	RECT  map;//Left Will the position and right will be the size of the palette
			  //top will be starting point of tiles in RAM
	sCombo	 cboPal;
	TileBuffer Tiles;
	int   picloaded;
	char  FileString[1024];
	int   MakeFiles();
	int   LoadFiles();
	int   DelFiles();
	char  picname[1024];
	int   Draw();
	int   FillCustBG();
	int   SaveCustBGROM();
	int   SaveCustTileset();
	int   pW;
	int   pH;
	int   FillOutArray(unsigned char* array);

};

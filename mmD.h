#pragma once
#include <windows.h>
#include "sCombo.h"
#include "image.h"
#include "sChecks.h"


class MiniMapClass{

public:
	static MiniMapClass* miniMapEditor;
	MiniMapClass();
   ~MiniMapClass();

   
	int Create(Image* map, Image* Tileset);//Sets colors and arrays up
    int Clear();//Clears palette and graphics	
	long   MapList[0xB];
	unsigned char    TilesetGraphics[0x4000];
	long    Palette[0x400];
	unsigned short*    RawMap;
	TileBuffer ts;
	int DecompressMap(int area);
	int Destroy();
   
    int DrawTileset(Image*, int);
	int DrawMap(Image* dstImage);
	int SaveMap();
	unsigned long DSize;
	unsigned short curtile;

	
private:
    int Zero();//Will 0 out every erray and return the average to make sure it's all 0
    
};    
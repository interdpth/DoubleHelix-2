#include "MinimapClass.h"
#include "globals.h"
#include "resource.h"

MiniMapClass* MiniMapClass::miniMapEditor;

MiniMapClass::MiniMapClass() {
	Zero();
}


MiniMapClass::~MiniMapClass() {
	Zero();
	Destroy();

}

int MiniMapClass::Zero()
{
	int a = 0;
	int i = 0;
	
	
	RawMap = NULL;
	
	memset(&MapList, 0, sizeof(long) * 0xB);
	memset(&TilesetGraphics, 0, 0x4000);	
	memset(&Palette, 0, sizeof(long) * 0x400);
	
	DSize=-1;
	curtile=-1;
	return a;
}

int MiniMapClass::Clear() {
	memset(&TilesetGraphics, 0, 0x3000);
	memset(&Palette, 0, sizeof(Palette));
	return 0;
}
int MiniMapClass::Create() {

	int x = 0;
	int y = 0;
	int tc = 0;
	int i = 0;
	switch (currentRomType) {
	case 0:
		if (GBA.ROM) {
			//Need to read our arrays and translate shit in
			MemFile::currentFile->seek(0x7601EC);
			MemFile::currentFile->fread(&MapList, 4, 0xB);
			MemFile::currentFile->seek(0x40E0C4);
			MemFile::currentFile->fread(&TilesetGraphics, 1, 0x3000);
			MemFile::currentFile->seek(0x411360);
			MemFile::currentFile->fread(&GBA.GBAPal, 2, 0xA0);
			GBA.DecodePal(GBA.GBAPal, Palette, 0xB, 0);
			ts.Load(TilesetGraphics, 384);
		}
		else
		{
			return 1;
		}

		break;
	case 1:
		if (GBA.ROM) {
			//Need to read our arrays and translate shit in
			MemFile::currentFile->seek(0x79BE5C);
			MemFile::currentFile->fread(&MapList, 4, 0xB);
			MemFile::currentFile->seek(0x561fa8);
			MemFile::currentFile->fread(&TilesetGraphics, 1, 0x4000);
			MemFile::currentFile->seek(0x5657AA);
			GBA.GBAPal[0] = 0;
			MemFile::currentFile->fread(&GBA.GBAPal[1], 2, 0xE0);
			GBA.DecodePal(GBA.GBAPal, Palette, 0xB, 0);
			ts.Load(TilesetGraphics, 430);
		}
		else
		{
			return 1;
		}

		break;
	}

	Tileset->SetPalette(Palette, 0, 0xa0);
	Map->SetPalette(Palette, 0, 0xA0);

	//cboMArea.SetListIndex(comboArea.GetListIndex);
	DrawTileset(Tileset, 8);
	return 0;
}

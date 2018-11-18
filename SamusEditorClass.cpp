#include "SamusEditorClass.h"



SamusEditorClass::SamusEditorClass()
{
	hwndSamusEditor = NULL;
	hwndSamusSpritePreview = NULL;
	SpritePreview = new SprGBuf();

}


SamusEditorClass::~SamusEditorClass()
{
	delete SpritePreview;
}


int SamusEditorClass::SetupPreview(SamusBase* base)
{
	
	long GFXPnt = 0;
	long PalPnt = 0;
	long palsize = 0;
	long addybuf = 0;
	long size = 0;
	int i = 0;
	int ii = 0;
	unsigned char*  compBuffer = new unsigned char[64691];
	GBAMethods* GBA = new GBAMethods();


	unsigned char *decompbuf = new unsigned char[32687];

	
	CompHeader thiscompheader;
	unsigned short transferpal[256] = { 0 };

	int  X = 0;
	long off = 0;
	int x = 0;
	

	GBA->DecodePal((short*)base->Suit_color, SpritePreview->PreviewPal, 16, 0);

	memcpy(SpritePreview->PreRAM, base->Sprite_tiles, 0x8000);
	

	//SpritePreview->maxparts = 0;
//	SpritePreview->OAM.clear();
	i = 0;
	for (i = 0; i < base->theSprite->maxparts; i++) SpritePreview->OAM.push_back(base->theSprite->OAM[i]);
	SpritePreview->maxparts = i;
	
	SpritePreview->id = 0xFF;

	//RD1Engine::theGame->mgrOAM->DecodeOAM(GBA->ROM, true, SpritePreview,(unsigned long) base->SamusOAMPointer - 0x8000000);
	SpritePreview->PreviewSprite.GetFullImage()->Clear();
	RD1Engine::theGame->mgrOAM->DrawPSprite(SpritePreview);

	delete[] compBuffer;
	delete[] decompbuf;
	return 0;
}


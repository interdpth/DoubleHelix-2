#pragma once
#include <windows.h>
#include "Image.h"
#include "BackBuffer.h"
#include "SprGBuf.h"
#include "SamusBase.h"
#include "GBAMethods.h"
#include "BaseGame.h"
class SamusEditorClass
{
public:
	HWND	hwndSamusEditor;
	HWND   hwndSamusSpritePreview;
	Image* Samus;
	SpriteObject* SpritePreview;

	int animCounter = 0;
	unsigned short currentTile = -1;
	BackBuffer animBuffer;
	int SetupPreview(SamusBase* base);
	SamusEditorClass();
	~SamusEditorClass();
};


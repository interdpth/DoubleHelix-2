#pragma once
#include <windows.h>
#include "..\Win32GBAImageClasses\Image.h"
#include "..\Win32GBAImageClasses\BackBuffer.h"
#include "SprGBuf.h"
#include "SamusBase.h"
#include "GBAMethods.h"
#include "BaseGame.h"
class SamusEditorClass
{
public:
static	HWND	hwndSamusEditor;
	HWND   hwndSamusSpritePreview;
	Image* Samus;
	SpriteObject* SpritePreview;

	int animCounter = 0;
	unsigned short currentTile = 0xffff;
	BackBuffer animBuffer;
	int SetupSamusPreview(SamusBase* base);
	SamusEditorClass();
	~SamusEditorClass();
};

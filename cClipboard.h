#pragma once
#include <Windows.h>
#include "sCombo.h"
#include "..\Win32GBAImageClasses\image.h"
class cClipboard {
public:
	cClipboard();
	~cClipboard();
	unsigned char copy;
	unsigned char curclip;
	sCombo cboCurClip;
	unsigned short thisclipboard[16][256][4];
	Image  cbi[16];
	HWND me;
	HWND cb;

	int DrawSlots();
	int CheckforBlanks(HDC hdc);
	int CopyTiles(LPARAM lParam, int surface = 0);
	int Create(HINSTANCE globalinstanc);
	int Erase();
	int Erase(int board, int slot);
	int inuse;
	int CreatePic(HINSTANCE mainInstance, WNDPROC imgproc);
};
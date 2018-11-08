#pragma once
#include <Windows.h>
#include "sList.h"
#include "image.h"
#include "sCombo.h"
#include "GBAMethods.h"
#include "resource.h"
#include <map>
#include <iostream>
#include <string>
using namespace std;
class MetroidTextEditor {

public:
	MetroidTextEditor();
	~MetroidTextEditor();
	map<int,string> StringTable;
	HWND tMain;
	HWND textpic;
	sList cOffsets;
	int Create(int TitleChoice, HINSTANCE MainInstance, WNDPROC wnd);
	char curtext[0x1000];
	int DecodeText(unsigned long offset);
	int LoadTable();
	Image* Text;
	int CreateTextpicTileImage(HINSTANCE MainInstance, WNDPROC wnd);
	int Draw();
	int EncodeText(int TitleChoice);
	TileBuffer Tiles;
	int BiCat(unsigned short* val, char* character);
	unsigned long ZMOffsets[16][0x200];
	unsigned long Offsets[0x200];
	sCombo cboTxt;
	int DecodeText(unsigned short* data, int dataLen);
	unsigned short  binarystring[1024];
	unsigned char    Graphics[0xF000];
	long   FontPal[0x200];
	long   txtlength;
	int    ResetList(int TitleChoice);
	char pixelflag;
	unsigned short GetEncodedEntry(char* data);


	vector<unsigned short>* EncodeString(char* txt);
};
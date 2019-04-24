#pragma once
#pragma once
#include <windows.h>
#include "sChecks.h"
#include "sList.h"
#include "sCombo.h"
#include "image.h"
#include "Structures.h"
#include "GBAMethods.h"
#include "Frames.h"
#include "resource.h"
#include "GlobalVars.h"
LRESULT CALLBACK cSSEPalProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
class Frame;
extern GBAMethods GBA;
extern HINSTANCE hGlobal;
enum EditType {
	SPRITES,
	SAMUS
};
class cOAMEdit {
public:
	int GetSpriteData( int id, int titleType);
	cOAMEdit();
	~cOAMEdit();
	
class	FrameManager* currentFrames=NULL;

static cOAMEdit* OamEditor;
	int UpdateSize();
	EditType currentEditing;
	sCombo cboPal;
	sCombo cboFrames;
	sCombo cboFrameTable;
	sCombo cboPartNo;
	sCombo cboShapes;
	sCombo cboXPos;
	sCombo cboYPos;
	sCombo cboSizes;//Changes based on cboShape
	sCombo cboBgPriority;
	sList  lstSprite;
	sChecks VChk;
	sChecks HChk;
	
	HWND   hwndAnimationWindow;
	HWND   hwndSpritePreview;
	HWND   hwndTiles;
	HWND   hwndPal;
	HWND   hwndPartWindow;
	HWND   _oamWindow;
	Image* tileImage;
	
	

	RECT  rPart;
	int maxframe();
	int CreatePalHWND();
	int CreateTileHWND();
	int CreateSpriteHWND();
	int CreateSpriteAnimationHWND();
	int CreatePartHWND();

	
	int UpdateCurrentSprite();
	int DrawSelObj(HDC hdc);
	int ExportPal();
	int UpdateTiles();
	int UpdatePal();	
	int ExportGraphics();
	int ImportGraphics();

	void SaveFrames();
	void SaveFrameData();
	unsigned long GetNewOffset(int size);
	unsigned long Save(void* variableAddress);
	unsigned long Save(SaveOptions savetype, char* fileName);

	int GetFrames(unsigned long offset,  int spriteID, int titleType);
	int SetFrames(int index);
	int UpdatePartUI(bool updateSizes = true);
	int Create();
	int LoadTiles(Image* tileImage, Frame* targetFrame);
};

LRESULT CALLBACK OAMPartProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
///Windos proc for handling the animation window
LRESULT CALLBACK AnimationProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OAMSpriteProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OAMTileProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK OAMPalProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
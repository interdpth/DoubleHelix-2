#pragma once
#pragma once
#include <windows.h>
#include "..\RNA_UI\stxts.h"
#include "..\RNA_UI\sChecks.h"
#include "..\RNA_UI\sList.h"
#include "..\RNA_UI\sCombo.h"
#include "..\Win32GBAImageClasses\image.h"
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
	sCombo  lstSprite;
	sChecks VChk;
	sChecks HChk;
	txtWndClass txtFrameOffset;
	txtWndClass txtTimerv;
	txtWndClass txtGlobalGfx;
	txtWndClass txtGlobalPalc;
	txtWndClass txtCurrentGfx;
	txtWndClass txtCurrentPal;
	HWND   hwndAnimationWindow;
	HWND   hwndSpritePreview;
	HWND   hwndTiles;
	HWND   hwndPal;
	HWND   _oamWindow;
	Image* tileImage;
	
	

	RECT  rPart;
	int maxframe();
	int CreatePalHWND();
	int CreateTileHWND();
	int CreateSpriteHWND();
	int CreateSpriteAnimationHWND();
	int CreatePartHWND();
	int CreateInfoWindow();
	
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
	int InitDlg();
	int LoadTiles(Image* tileImage, Frame* targetFrame);
};

LRESULT CALLBACK OAMPartProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
///Windos proc for handling the animation window
LRESULT CALLBACK AnimationProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OAMSpriteProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OAMTileProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK OAMPalProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
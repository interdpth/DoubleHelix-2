#pragma once
#ifndef EXT
#define EXT extern
#endif
#include "WindowScrollbar.h"
#include "SprGBuf.h"
#include "ResourceAndDefines.h"
#include "Door Connections.h"
#include "..\Win32GBAImageClasses\backbuffer.h"
#include "GBAMethods.h"
#include "nMapBuffer.h"
#include "GlobalFunctions.h"
#include "..\RNA_UI\sChecks.h"
#include "..\RNA_UI\sCombo.h"
#include "..\RNA_UI\sMenuItem.h"
//int DrawPSprite( SprGBuf* SpriteDetails);
//int DecodeSprite(SprGBuf* tSprite,unsigned long Offset);
EXT WINDOWPLACEMENT IsTSA;
EXT int currentRomType;//Current ROM file
EXT HWND  InputHwnd;
EXT HWND ExtendedOptWND;
//For scenery editor
EXT HWND SceneWnd;
EXT HWND SceneGraph;
EXT HWND TSScene;
EXT HWND hwndMM;
class WindowScrollBar;

EXT HINSTANCE hGlobal;
EXT RECT  TilesetRect;
EXT RectBuff TileCurs;
EXT HWND hwndResize;
EXT HWND wndHE;

#define Square		0
#define Horizontal  1
#define Vertical	2


EXT Image       SceneryPic;
EXT Image       SceneryTiles;

EXT Drc         DoorConnections;


EXT WindowScrollbar* MapHorizScroll;
EXT WindowScrollbar* MapVertScroll;
//PLEASE REMOVE ASAP.
EXT int			nHScroll[32];
EXT int			nVScroll[32];
EXT int			nDisplayWidth[32];
EXT int			nDisplayHeight[32];
EXT int			nMaxHScroll[32];
EXT int			nMaxVScroll[32];

//END REMOVAL

EXT int			bRunApp;
EXT int         CurDoor;
//EXT long        DoorOffsets[8];
//EXT long        MFDoorOffsets[10];

EXT HWND        hwndHeader;


EXT sCombo comboArea;
EXT sCombo cboDoorTyp;
EXT sCombo doorConnection;
EXT sCombo cboDoorOwner;
EXT sCombo cboDoorArea1;
EXT sCombo cboDoorArea2;
EXT sCombo cboDoorArea3;
EXT sCombo cboDoorDoor1;
EXT sCombo cboDoorDoor2;
EXT sCombo cboDoorAreaConnect;

EXT sCombo comboRoom;
EXT sCombo cboClipData;
EXT sCombo comboSpriteSet;

EXT sCombo cboSBGsize; 
EXT sCombo cboSSPal;
//int MoveObj(unsigned char ObjNum, unsigned char Where);


EXT HBRUSH g_hbrBackground;
EXT GBAMethods GBA;

EXT TileBuffer      SpriteImage;

EXT char procstring[1024];

EXT unsigned char CurMapWidth, CurMapHeight;
EXT LONG oldEditProc;
EXT short Tile;



EXT unsigned char LoadingLevel;

EXT HWND MapDataWIn;
EXT HWND ScrollWIn;
EXT HWND DoorWin;
BOOL CALLBACK MapDataProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);


EXT int CurObjT;
EXT int CurObjNo;
EXT unsigned char ConnectExists;
EXT unsigned char PSCE[0x101];
int UserEnableConnections();

int initDoorWnd();

EXT int movingSprite;
EXT int movingSpriteIndex;

EXT int movingDoors;
EXT int movingDoorIndex;

EXT int movingScroll;
EXT int movingScrollIndex;


#define vSS 31
EXT int SpriteTabIndex;
EXT int dispic;
EXT HWND hwndSS;

int InitScrolls(int CurrentArea, int Curroom);


EXT MousePointer mpTileset; 
EXT MousePointer mpMap; 
EXT MousePointer mpScene; 
EXT MousePointer mpSTS;
EXT int curDoor;

//Combobox Defines each for a combo
EXT int DM[3];
EXT int TP[3];
EXT int MP[3];

EXT int InputType;
EXT int SceneTile;
EXT unsigned char  SceneType;




EXT BackBuffer BBTileset;


EXT unsigned char  MPToUse;
EXT sMenuItem chkDoTrans;


EXT unsigned char DefRoom;
EXT unsigned char DefArea;
EXT unsigned long DefAddress;

EXT HWND hConn; 
unsigned short cprs_gba_lz77(RECORD *dst, const RECORD *src);
EXT int  drawmm; 
EXT sCombo LayerCombo;
EXT int  ScP[2];
EXT int  SmP[2];
EXT int  SceneryCopySource; //Scenery Copy Source


EXT unsigned char   import;

EXT int BGiInstall;

EXT HWND hwndLPE;
EXT int  palscroll[3];

EXT unsigned char clrIndex;

EXT int       curclr;

EXT sCombo cboScroll;
EXT BackBuffer  BB;
EXT sCombo cboTSAPAL;

EXT HWND hwndInsert; 

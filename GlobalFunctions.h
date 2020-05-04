#pragma once
#include "clsRoomScrolls.h"
#define GetX(lParam) LOWORD(lParam)
#define GetY(lParam) HIWORD(lParam)

int Gimmeasprite(int X, int Y);
//yPos = GET_Y_LPARAM(lParam); 
// define your globals like so:
int SetCurSprite();
int  ClearGlobals();
int LoadExtensions();
int LoadCurSprite();
int InitSceneryWindow();
int GetBitField(long reg, long bitStart, long bitLength);
int InitTSWindow();
int DrawGraphicTiles();
int InitPosArray();
int DrawThisScene(unsigned char Scene);

int LoadData();
int DrawScrolls(int ScrollToDraw, Scroller *scroll);
int SetupHeaderControls();
int LoadHeaderControls();
int ChangeSprites();
int SaveHeader(unsigned char call);
int DrawTileRect(HDC hdc, MousePointer mpointer, int mul);
int GetArrays();
int HandleDetections(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);
int SetUpCombos();
int GetRoomCount(long RoomTableOffset);



int sMessage(char* messagestring);
int LoadHeader(long Offset);
int DrawLevel();

int DrawRoom(bool DrawBackLayer, bool DrawLevelLayer, bool DrawForeground);


int InitTileset();
int InitMap();


int DrawClipIdent();
//int Findmeascroll(int X, int Y);

void ChangeCompression(unsigned char oldlayervalue, unsigned char newval, unsigned char layer, nMapBuffer* buffer);


int LoadInput(long val);

int DrawSSprite(SpriteObject* SpriteDetails);

void setsize(nMapBuffer* buffer);
void ChangeCompression(unsigned char oldlayervalue, unsigned char newval, unsigned char layer, nMapBuffer* buffer);
int SaveCurScene();
int WriteDataToROM(unsigned char* datastream, unsigned long size, unsigned long offset);
int DrawLayer(nMapBuffer* Map, unsigned char ctype);
void DrawSpritePAl(HDC hdc, long* palette, int size);
int SetTiles(LPARAM lParam);

HWND hwndMain();
int CheckTheSize(sprite_entry* SE);
int EditThisLayer(nMapBuffer * Layer, WPARAM wParam, LPARAM lParam, unsigned char usens, unsigned short ToUse);
int LoadConfigFile(char* ConfigLoc);
int ExportRoom();
int ImportRoom();
int HandleRightClick(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam, bool bScrollCheck);
BOOL CALLBACK StatEditorProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK TSAProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SceneProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OAMProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK TSAProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK TextEditorProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DwProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SSProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK HeaderProced(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SpriteSetEditorProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ClipBoardProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK BGiProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK LPProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK InsertProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ExtendedProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MiniTilesetProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MiniGraphicProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TextPicProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SWProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TSWProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TilesetProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MapProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InputBoxProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK cSSETileProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK cSSEPalProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CBImageProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OAMPalProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OAMTileProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OAMSpriteProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);


int GetLayerData(unsigned char compression, unsigned char Layer, unsigned long offset);

int DrawSpritePAl(HDC hdc, long* palette, int X, int Y, int palcol, int size);
int LoadColor(long Pal);
int ChangeColor();

int LoadTrans(char* Directive, int DataType, HWND hwnd);
void TransMenu(char* MenuName,  HWND hwnd);
unsigned short ApplyMenuString(int tMenu, char* string, HWND hwnd, unsigned short Counter);

#pragma once

#define s32 signed long
#define s16 signed short
#define s8 signed char




#include <Windows.h>
#include <stdio.h>
#include <time.h>

//#include "Misc.h"
#include <vector>
#include <algorithm>
#include <string>
#include "image.h"
#include "resource.h"
#include "Structures.h"
#include "GBAMethods.h"
#include "MapManager.h"
#include "Door Connections.h"
#include "cTSAEditorClass.h"
#include "globals.h"
#include "GlobalVars.h"
#include "GBAGraphics.h"
#include "IPSPatch.h"
#include "UIState.h"
//using namespace std;
int DoScroll(LPARAM lParam);
int EditLayers(WPARAM wParam,LPARAM lParam);



#define RGBA(r,g,b,a)         ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))  | a << 32)
void  ChangeScrollbars(HWND dialogwnd,int Res,int Scroll);
void UpdateScroll(HWND dialogwnd,WPARAM how,int isvertical,int Res,int Scroll);




BOOL CALLBACK ScrollWndProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MiniProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);

EXT cTSAEditorClass MyTSAEditor;

EXT int keys;
EXT int keyno;

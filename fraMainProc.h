#pragma once
#include <Windows.h>
#include "resource.h"
#include "GlobalVars.h"
#include "globals.h"
#include "UiState.h"
void OpenRom();
void DumpLayers();
BOOL CALLBACK  fraMainProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);
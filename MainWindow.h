#pragma once
#include "globals.h"
void MakeSingleWindow();
void MakeMultiWindow();
void ModifyWindows();
void ClearContainer();
LRESULT CALLBACK ContainerProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
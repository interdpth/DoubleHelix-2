#pragma once
#include <windows.h>
///#include "UIState.h"
class WindowScrollbar
{
private:
	HWND _window;
	int _max;
	int _index;
	bool _vert;
public:
	
	WindowScrollbar(HWND hwnd, bool vertical);
	~WindowScrollbar();
	int GetMax();
	void SetMax(int max);
	int GetIndex(); 
	void SetIndex(int newindex);
	int ResizeScrollBar(int Res, int Min, int Max);
	void UpdateScroll( WPARAM how);
	void ChangeScrollbars();
};


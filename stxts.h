#pragma once
#include <Windows.h>
#include <stdio.h>
class stxts {
	//Currenly only support string to int
public:

	stxts();
	~stxts();
	int SetHwnd(HWND hwnd);
	int GetText();
	int SetText(char* string);
private:
	HWND thisHwnd;

};
#pragma once
#include <Windows.h>
class sList {
public:
	sList();
	~sList();
	long GetListIndex();
	int cClear();
	int Clear();
	int Click();
	int SetListIndex(int Value);
	int Additem(char* string);
	int sListStrings(int index);
	int Init(HWND sListHwnd);
	int Disable();
	int Enable();
	HWND thishwnd();
private:
	HWND hCombo;

};
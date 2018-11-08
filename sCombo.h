#pragma once
#include <Windows.h>
class sCombo
{
public:
	sCombo();
	~sCombo();
	long GetListIndex();
	int cClear();
	int Clear();
	int Click();
	int SetListIndex(int Value);
	int Additem(char* string);
	int sList(int index);
	int Init(HWND sComboHwnd);
	int Disable();
	int Enable();
	//storing states
	bool inited;
	int val;
	HWND GetHwnd();
private:
	HWND hCombo;
};


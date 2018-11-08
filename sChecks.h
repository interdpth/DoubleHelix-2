#pragma once
#include <windows.h>
class sChecks {
public:
	sChecks();
	~sChecks();
	int cClear();

	int SetCnt(HWND hwnd);

	int value();

	int value(unsigned char Val);
	int sChecks::State(bool flag);

	int sChecks::State();

	//storing states
	bool inited;
	int val;
private:
	HWND check;

};

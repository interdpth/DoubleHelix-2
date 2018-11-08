#include "stxts.h"



stxts::stxts()
{
}


stxts::~stxts()
{
}

int             stxts::SetHwnd(HWND hwnd)
{
	thisHwnd = hwnd;
	return 0;
}
int             stxts::GetText()
{

	int             val = 0;
	char            buf[100];

	GetWindowText(thisHwnd, buf, 4);
	sscanf(buf, "%d", &val);
	return val;
}

int             stxts::SetText(char *string)
{
	SetWindowText(thisHwnd, string);
	return 0;
}
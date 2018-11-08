#include "sChecks.h"




sChecks::~sChecks()
{
	cClear();
}
sChecks::sChecks()
{
	cClear();
	inited = false;
}

int             sChecks::cClear()
{
	check = NULL;
	return 0;
}

int             sChecks::SetCnt(HWND hwnd)
{
	check = hwnd;
	inited = true;
	val = 0;
	return 0;
}

int             sChecks::value()
{
	int returnVal = SendMessage(check, BM_GETCHECK, 0, 0);
	if (returnVal == -1 && inited)
	{
		returnVal = val;
	}
	return returnVal;

}

int             sChecks::value(unsigned char Val)
{
	if (Val < 3)
	{
		SendMessage(check, BM_SETCHECK, Val, 0);
		val = Val;
	}
	else
	{
		MessageBox(0, "ayy", "Bad checkbox value", MB_OK);
	}
	return 0;
}

int             sChecks::State(bool flag)
{
	EnableWindow(check, flag);
	return 0;
}

int             sChecks::State()
{

	return IsWindowEnabled(check);
}

#include "sList.h"

sList::~sList()
{
	Clear();
}
sList::sList()
{
	cClear();
}

int             sList::Init(HWND sListHwnd)
{
	hCombo = sListHwnd;
	Clear();
	return 0;
}
int             sList::Disable()
{
	EnableWindow(hCombo, 0);
	return 0;
}
int             sList::Enable()
{
	EnableWindow(hCombo, 1);
	return 0;
}
int             sList::cClear()
{
	hCombo = NULL;
	return 0;
}
int             sList::Clear()
{
	SendMessage(hCombo, LB_RESETCONTENT, 0, 0);
	return 0;
}
long            sList::GetListIndex()
{
	return SendMessage(hCombo, LB_GETCURSEL, 0, 0);
}

int             sList::SetListIndex(int Value)
{
	SendMessage(hCombo, LB_SETCURSEL, Value, 0);
	return 0;
}
int             sList::sListStrings(int index)
{
	SendMessage(hCombo, LB_GETITEMDATA, index, 0);
	return 0;
}

int             sList::Additem(char *string)
{
	SendMessage(hCombo, LB_ADDSTRING, 0, (long)string);
	return 0;
}

HWND            sList::thishwnd()
{
	return hCombo;
}
#include "sCombo.h"



sCombo::~sCombo()
{
	inited = false;
	val = -1;
	Clear();
}
sCombo::sCombo()
{
	cClear();
}

int             sCombo::Init(HWND sComboHwnd)
{
	hCombo = sComboHwnd;
	Clear();
	inited = true;	
	return 0;
}
int             sCombo::Disable()
{
	EnableWindow(hCombo, 0);
	return 0;
}
int             sCombo::Enable()
{
	EnableWindow(hCombo, 1);
	return 0;
}
int             sCombo::cClear()
{
	hCombo = NULL;
	return 0;
}
int             sCombo::Clear()
{
	SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
	return 0;
}

string        sCombo::GetText(int index)
{

	char returnVal[1024] = { 0 };
	 SendMessage(hCombo, CB_GETLBTEXT, index, (LPARAM)returnVal);
	
	return string(returnVal);
}

long            sCombo::GetListIndex()
{

	int returnVal = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
	if (returnVal == -1 && inited)
	{
		returnVal = val;
	}
	return returnVal;
}

int             sCombo::SetListIndex(int Value)
{
	SendMessage(hCombo, CB_SETCURSEL, Value, 0);
	val = Value;
	return 0;
}
int             sCombo::sList(int index)
{
	SendMessage(hCombo, CB_GETITEMDATA, index, 0);
	return 0;
}
int             sCombo::Click()
{
	return 0;
}

int             sCombo::Additem(char *string)
{
	SendMessage(hCombo, CB_ADDSTRING, 0, (long)string);
	return 0;
}

HWND            sCombo::GetHwnd()
{
	return hCombo;
}
#pragma once
#include <Windows.h>
class IObject
{
public:
	void SetXPosition(int X);
	void SetYPosition(int Y);
	//Returns left from Rect;
	virtual int XPosition();
	//Returns top from Rect;
	virtual int YPosition();
	//Returns right from Rect;
	int Width();
	//Returns bottom from Rect;
	int Height();
	//Returns reference to  _rect;
	RECT* GetRect();

	//Constructor
	IObject();

	//Deconstructor
	~IObject();
private:
	RECT _rect;
};


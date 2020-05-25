#include "IObject.h"

IObject::IObject()
{
	memset(&_rect, 0, sizeof(_rect));
}

IObject::~IObject()
{
}

void IObject::SetXPosition(int X)
{
	_rect.left = X;
}

void IObject::SetYPosition(int Y )
{
	_rect.top = Y;
}

int IObject::XPosition()
{
	return _rect.left;
}
int IObject::YPosition()
{
	return _rect.top;
}

int IObject::Width()
{
	return _rect.right;
}

int IObject::Height()
{
	return _rect.top;
}

RECT* IObject::GetRect()
{
	return &_rect;
}
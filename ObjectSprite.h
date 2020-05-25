#pragma once
#include "IObject.h"
#include "../RD1Engine/structures.h"
class ObjectSprite :
	public IObject
{
public:

	int Creature;
	void Apply();
	ObjectSprite(int Creature, int X, int Y);
	ObjectSprite(nEnemies* enemy);
	~ObjectSprite();
private:
	nEnemies _enemy;
};


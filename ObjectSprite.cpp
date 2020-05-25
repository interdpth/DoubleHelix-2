#include "ObjectSprite.h"

ObjectSprite::ObjectSprite(int creature, int X, int Y)
{
	_enemy.Creature = creature;
	_enemy.X = X;
	_enemy.Y = Y;
}

ObjectSprite::ObjectSprite(nEnemies* enemy)
{
	memcpy(&_enemy, enemy, sizeof(nEnemies)); 
	Apply();

}

ObjectSprite::~ObjectSprite()
{
}

void ObjectSprite::Apply()
{
	Creature = _enemy.Creature;
	SetXPosition(_enemy.X);

	SetYPosition(_enemy.Y);
}

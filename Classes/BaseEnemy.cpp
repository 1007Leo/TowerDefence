#include "BaseEnemy.h"

enemies::BaseEnemy::BaseEnemy()
{
	sprite = Sprite::create("sprites/enemySprites/baseEnemy.png");
	damage = 1;
	health = 1;
	speed = 70;
	points = 1;
}

Sprite* enemies::BaseEnemy::getSprite()
{
	return this->sprite;
}

enemies::Direction enemies::BaseEnemy::getMoveDirection()
{
	return this->moveDirection;
}

enemies::Direction enemies::BaseEnemy::getPrevDirection()
{
	return prev;
}

enemies::State enemies::BaseEnemy::getState()
{
	return this->state;
}

double enemies::BaseEnemy::getSpeed()
{
	return this->speed;
}

void enemies::BaseEnemy::setState(enemies::State state)
{
	this->state = state;
}

void enemies::BaseEnemy::setMoveDirection(Direction direction)
{
	moveDirection = direction;
}

void enemies::BaseEnemy::setPrevDirection(Direction direction)
{
	prev = direction;
}

void enemies::BaseEnemy::attack(Tower* tower, float delta)
{
	Vec2 curCoord = this->sprite->getPosition();
	Vec2 towerCoord = tower->getSprite()->getPosition();

	Size spriteSize = this->sprite->getContentSize();
	float scale = this->sprite->getScale();

	spriteSize.width *= scale;
	spriteSize.height *= scale;

	if (curCoord.getDistance(towerCoord) - scale <= std::min(spriteSize.width, spriteSize.height))
	{
		tower->makeDamage(this->damage * delta);
	}
	
}

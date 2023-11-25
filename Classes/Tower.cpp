#include "Tower.h"

Tower::Tower()
{
	this->mapPos = { 0.0, 0.0 };
	this->health = 100;
	this->maxHealth = 100;
	this->sprite = Sprite::create("sprites/towerSprites/towerFull.png");
}

Tower::Tower(Point pos, float hp)
{
	this->mapPos = pos;
	this->health = hp;
	this->sprite = Sprite::create("sprites/towerSprites/towerFull.png");
}

Sprite* Tower::getSprite()
{
	return this->sprite;
}

Point Tower::getPos()
{
	return this->mapPos;
}

void Tower::setNewHealth(float hp)
{
	this->health = hp;
	this->maxHealth = hp;
}

void Tower::makeDamage(float dmg)
{
	this->health -= dmg;
}

bool Tower::isAlive()
{
	return this->health > 0;
}

void Tower::update()
{
	if (this->health <= 0)
	{
		this->sprite->setTexture("sprites/towerSprites/towerDestroyed.png");
		Director::getInstance()->end();
	}
	else if (this->health <= this->maxHealth / 2)
	{
		this->sprite->setTexture("sprites/towerSprites/towerHalf.png");
	}
}

#pragma once
#include "cocos2d.h"
#include "Tower.h"

USING_NS_CC;

namespace enemies 
{
	const enum Direction {none = 0, up = 1, right = 2, down = 3, left = 4};
	const enum State { disabled = 0, drawn = 1, enabled = 2, dead = 3 };

	class BaseEnemy
	{
	public:
		BaseEnemy();
		//~BaseEnemy();

		Sprite* getSprite();
		Direction getMoveDirection();
		Direction getPrevDirection();
		State getState();
		float getHealth();
		float getDamage();
		float getSpeed();
		float getPoints();

		void setState(State state);
		void setHealth(double newHealth);
		void setMoveDirection(Direction direction);
		void setPrevDirection(Direction direction);
		void setPoints(float points);

		void attack(Tower* tower, float delta);

	protected:
		Sprite* sprite;

		State state = disabled;
		float health;
		float damage;
		float speed;
		int points;

		Direction moveDirection = enemies::none;
		Direction prev = enemies::none;
	};
}
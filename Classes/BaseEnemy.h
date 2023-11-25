#include "cocos2d.h"
#include "Tower.h"

USING_NS_CC;

namespace enemies 
{
	const enum Direction {none = 0, up = 1, right = 2, down = 3, left = 4};
	const enum State { disabled = 0, drawn = 1, enabled = 2 };

	class BaseEnemy
	{
	public:
		BaseEnemy();
		//~BaseEnemy();

		Sprite* getSprite();
		Direction getMoveDirection();
		Direction getPrevDirection();
		State getState();
		double getHealth();
		double getDamage();
		double getSpeed();
		double getPoints();

		void setState(State state);
		void setHealth(double newHealth);
		void setMoveDirection(Direction direction);
		void setPrevDirection(Direction direction);

		void attack(Tower* tower, float delta);

	protected:
		Sprite* sprite;

		State state = disabled;
		double health;
		double damage;
		double speed;
		int points;

		Direction moveDirection = enemies::none;
		Direction prev = enemies::none;
	};
}
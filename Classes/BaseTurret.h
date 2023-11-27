#pragma once
#include "cocos2d.h"
#include "BaseEnemy.h"

namespace defences
{
	const enum Types {baseTurret = 0, machineGun = 1, gun = 2, artillery = 3};

	class BaseTurret
	{
	public:
		BaseTurret();
		BaseTurret(float cellSize);

		Sprite* getSprite();
		Sprite* getHitSprite();

		virtual void update(std::list< std::unique_ptr<enemies::BaseEnemy> >& enemiesList, float delta);
	protected:
		float rotationAngle = 0;
		float rotationSpeed;

		float damage;
		float damageRadius;
		float attackRadius;

		Sprite* sprite;
		Sprite* hitSprite;

		float getAngleOfTwoVectors(Vec2 v1, Vec2 v2);
		float getAngleDifference(float angle1, float angle2);
		bool rotateToTarget(enemies::BaseEnemy* enemy, float delta);
		enemies::BaseEnemy* findNextTarget(std::list< std::unique_ptr<enemies::BaseEnemy> >& enemiesList);

		virtual void attack(enemies::BaseEnemy* enemy, float delta);
		virtual void drawHitMark(enemies::BaseEnemy* enemy);
	};
}
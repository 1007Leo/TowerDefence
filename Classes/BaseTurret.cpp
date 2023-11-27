#include "BaseTurret.h"

defences::BaseTurret::BaseTurret()
{
	sprite = Sprite::create("sprites/defencesSprites/baseTurret.png");
	hitSprite = Sprite::create("sprites/defencesSprites/hitSprites/normalHitSprite.png");
	rotationSpeed = 70;
	damage = 10;
	attackRadius = 3 * 40;
}

defences::BaseTurret::BaseTurret(float cellSize)
{
	sprite = Sprite::create("sprites/defencesSprites/baseTurret.png");
	hitSprite = Sprite::create("sprites/defencesSprites/hitSprites/normalHitSprite.png");
	rotationSpeed = 70;
	damage = 10;
	attackRadius = 3 * cellSize;
}

Sprite* defences::BaseTurret::getSprite()
{
	return this->sprite;
}

Sprite* defences::BaseTurret::getHitSprite()
{
	return this->hitSprite;
}

void defences::BaseTurret::update(std::list<std::unique_ptr<enemies::BaseEnemy>>& enemiesList, float delta)
{
	enemies::BaseEnemy* enemy = findNextTarget(enemiesList);

	if (!rotateToTarget(enemy, delta))
	{
		this->hitSprite->setZOrder(-1);
	}
	else
	{
		this->attack(enemy, delta);
	}
}

void defences::BaseTurret::attack(enemies::BaseEnemy* enemy, float delta)
{
	if (enemy != nullptr && enemy->getState() == enemies::State::drawn)
	{
		enemy->setHealth(enemy->getHealth() - this->damage * delta);
	}
	drawHitMark(enemy);
}

float defences::BaseTurret::getAngleOfTwoVectors(Vec2 v1, Vec2 v2)
{
	auto vectorFromVec1ToVec2 = v2 - v1;
	return CC_RADIANS_TO_DEGREES(-vectorFromVec1ToVec2.getAngle());
}

float defences::BaseTurret::getAngleDifference(float angle1, float angle2)
{
	float diffAngle = (angle1 - angle2);

	if (diffAngle >= 180.f)
	{
		diffAngle -= 360.f;
	}
	else if (diffAngle < -180.f)
	{
		diffAngle += 360.f;
	}
	return diffAngle;
}

bool defences::BaseTurret::rotateToTarget(enemies::BaseEnemy* enemy, float delta)
{
	float angle = 0;
	if (enemy != nullptr && enemy->getState() == enemies::State::drawn)
	{
		angle = getAngleOfTwoVectors(this->sprite->getPosition(), enemy->getSprite()->getPosition());
	}
	else
	{
		Vec2 zero = this->sprite->getPosition();
		zero.y += 1;
		angle = getAngleOfTwoVectors(this->sprite->getPosition(), zero);
	}
	float angleDiff = getAngleDifference(this->rotationAngle, angle - 90);

	if (ceil(abs(angleDiff)) == 180)
	{
		return true;
	}
	else
	{
		int sign = angleDiff / abs(angleDiff);
		this->rotationAngle = this->rotationAngle + sign * this->rotationSpeed * delta;
		if (abs(this->rotationAngle) > 360)
		{
			this->rotationAngle -= sign * 360;
		}
		this->sprite->setRotation(rotationAngle);
	}

	return false;
}

enemies::BaseEnemy* defences::BaseTurret::findNextTarget(std::list<std::unique_ptr<enemies::BaseEnemy>>& enemiesList)
{
	enemies::BaseEnemy* closestTarget = nullptr;
	float minDist = this->attackRadius + 1;
	for (auto& enemy : enemiesList)
	{
		if (enemy.get()->getState() != enemies::State::drawn)
			continue;
		Point enemyPos = enemy.get()->getSprite()->getPosition();
		Point turretPos = this->sprite->getPosition();
		float dist = turretPos.getDistance(enemyPos);
		if (dist < minDist)
		{
			minDist = dist;
			closestTarget = enemy.get();
		}
	}
	return closestTarget;
}

void defences::BaseTurret::drawHitMark(enemies::BaseEnemy* enemy)
{
	if (enemy == nullptr || enemy->getState() != enemies::State::drawn)
	{
		this->hitSprite->setLocalZOrder(-1);
		return;
	}
	float scale = enemy->getSprite()->getScale();
	Point enemyPos = enemy->getSprite()->getPosition();
	float randX = RandomHelper::random_real(-5.0, 5.0) * scale,
		  randY = RandomHelper::random_real(-5.0, 5.0) * scale;

	this->hitSprite->setPosition({ enemyPos.x + randX, enemyPos.y + randY });
	this->hitSprite->setLocalZOrder(2);

}

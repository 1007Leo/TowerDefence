#include "Defences.h"

defences::MachineGun::MachineGun()
{
	sprite = Sprite::create("sprites/defencesSprites/machineGun.png");
	hitSprite = Sprite::create("sprites/defencesSprites/hitSprites/normalHitSprite.png");
	rotationSpeed = 100;
	damage = 10;
	attackRadius = 3 * 40;
	damageRadius = 0;
}

defences::MachineGun::MachineGun(float cellSize)
{
	sprite = Sprite::create("sprites/defencesSprites/machineGun.png");
	hitSprite = Sprite::create("sprites/defencesSprites/hitSprites/normalHitSprite.png");
	rotationSpeed = 100;
	damage = 10;
	attackRadius = 3 * cellSize;
	damageRadius = 0;
}

defences::Gun::Gun()
{
	sprite = Sprite::create("sprites/defencesSprites/gun.png");
	hitSprite = Sprite::create("sprites/defencesSprites/hitSprites/normalHitSprite.png");
	rotationSpeed = 80;
	damage = 35;
	reload = 2;
	reloadBuffer = reload;
	attackRadius = 4 * 40;
	damageRadius = 0;
}

defences::Gun::Gun(float cellSize)
{
	sprite = Sprite::create("sprites/defencesSprites/gun.png");
	hitSprite = Sprite::create("sprites/defencesSprites/hitSprites/normalHitSprite.png");
	rotationSpeed = 80;
	damage = 35;
	reload = 2;
	reloadBuffer = reload;
	attackRadius = 4 * cellSize;
	damageRadius = 0;
}

void defences::Gun::attack(enemies::BaseEnemy* enemy, float delta)
{
	if (enemy != nullptr && enemy->getState() == enemies::State::drawn)
	{
		reloadBuffer -= delta;
		if (reloadBuffer <= 0)
		{
			reloadBuffer = reload;
			drawHitMark(enemy);
			enemy->setHealth(enemy->getHealth() - this->damage);

		}
	}
}

defences::Artillery::Artillery()
{
	sprite = Sprite::create("sprites/defencesSprites/artillery.png");
	hitSprite = Sprite::create("sprites/defencesSprites/hitSprites/bigHitSprite.png");
	rotationSpeed = 60;
	damage = 50;
	reload = 5;
	reloadBuffer = reload;
	attackRadius = 4 * 40;
	damageRadius = 2 * 40;
}

defences::Artillery::Artillery(float cellSize)
{
	sprite = Sprite::create("sprites/defencesSprites/artillery.png");
	hitSprite = Sprite::create("sprites/defencesSprites/hitSprites/bigHitSprite.png");
	rotationSpeed = 60;
	damage = 50;
	reload = 5;
	reloadBuffer = reload;
	attackRadius = 4 * cellSize;
	damageRadius = 2 * cellSize;
}

void defences::Artillery::update(std::list<std::unique_ptr<enemies::BaseEnemy>>& enemiesList, float delta)
{
	enemies::BaseEnemy* enemy = findNextTarget(enemiesList);
	
	if (!rotateToTarget(enemy, delta))
	{
		this->hitSprite->setZOrder(-1);
	}
	else
	{
		if (enemy == nullptr)
			return;
		auto allEnemiesToDamage = getEnemiesInRadius(enemiesList, enemy->getSprite()->getPosition(), this->damageRadius);
		this->attack(allEnemiesToDamage, delta);
	}
}

void defences::Artillery::attack(std::list<enemies::BaseEnemy*> enemies, float delta)
{
	reloadBuffer -= delta;
	bool flag = true;
	if (reloadBuffer <= 0)
	{
		for (auto& enemy : enemies)
		{
			if (enemy->getState() == enemies::State::drawn)
			{	
				if (flag)
				{
					drawHitMark(enemy);
					flag = false;
				}
				enemy->setHealth(enemy->getHealth() - this->damage);
			}
		}
		reloadBuffer = reload;
	}
}

std::list<enemies::BaseEnemy*> defences::Artillery::getEnemiesInRadius(
	std::list<std::unique_ptr<enemies::BaseEnemy>>& enemiesList, 
	Vec2 center,
	float radius)
{
	std::list<enemies::BaseEnemy*> res;

	for (auto& enemy : enemiesList)
	{
		float dist = center.getDistance(enemy.get()->getSprite()->getPosition());
		if (dist <= radius)
			res.push_back(enemy.get());
	}
	return res;
}

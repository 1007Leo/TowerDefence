#include "Enemies.h"

enemies::NormalEnemy::NormalEnemy()
{
	sprite = Sprite::create("sprites/enemySprites/normalEnemy.png");
	damage = 10;
	health = 20;
	speed = 50;
	points = 100;
}

enemies::ThickEnemy::ThickEnemy()
{
	sprite = Sprite::create("sprites/enemySprites/thickEnemy.png");
	damage = 20;
	health = 40;
	speed = 25;
	points = 150;
}

enemies::FastEnemy::FastEnemy()
{
	sprite = Sprite::create("sprites/enemySprites/fastEnemy.png");
	damage = 5;
	health = 15;
	speed = 100;
	points = 120;
}

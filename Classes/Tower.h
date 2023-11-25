#include "cocos2d.h"

USING_NS_CC;

class Tower
{
public:
	Tower();
	Tower(Point pos, float hp = 100);

	Sprite* getSprite();
	Point getPos();
	
	void setNewHealth(float hp);
	void makeDamage(float dmg);
	bool isAlive();

	void update();
private:
	Point mapPos;
	Sprite* sprite;
	float health;
	float maxHealth;
};
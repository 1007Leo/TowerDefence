#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "Enemies.h"
#include "Defences.h"

const enum BlockTypes { unknown = 0, wall = 'w', mount = 'm', road = 'r', tower = 't' };

struct Wave
{
	float waveInterval;
	float enemyInterval;
	int baseEnemyCount;
	int normEnemyCount;
	int thickEnemyCount;
	int fastEnemyCount;
};

class GameManager
{
public:
	GameManager();

	static Vec2 pixelToCoord(Point pixelCoord, Size spriteSize);
	static Point coordToPixel(Vec2 boardCoord, Size spriteSize);

	bool loadLevel(int level);
	bool prepareWave(int waveNumber);
	int nextLevel();

	Tower* getTower();
	Point getEnemyStart();
	std::vector< std::vector<BlockTypes> >& getCurrentLevelMatrix();
	std::list< std::unique_ptr<enemies::BaseEnemy> > & getEnemies();
	std::list< std::unique_ptr<defences::BaseTurret> >& getDefences();
	std::vector< Wave >& getWaves();
	int getCurrentWave();
	bool isLevelInProgress();
	float getPlayerMoney();
	void setPlayerMoney(float money);

	void update(float delta);

private:
	Tower objTower;
	Point enemyStart;
	int currentWave = -1;
	int currentLevel = 0;
	bool levelInProgress = false;
	float playerMoney = 0;
	std::vector< std::vector<BlockTypes> > levelData;
	std::list< std::unique_ptr<enemies::BaseEnemy> > enemies;
	std::list< std::unique_ptr<defences::BaseTurret> > defences;
	std::vector< Wave > waves;

	void updateEnemy(enemies::BaseEnemy* enemy, float delta);
	bool enemyAbleToMove(enemies::BaseEnemy* enemy, enemies::Direction direction);
	void moveEnemy(enemies::BaseEnemy* enemy, enemies::Direction direction, float delta);

	bool readMap(std::ifstream& levelFile);
	bool readWave(std::ifstream& levelFile);
	bool readTower(std::ifstream& levelFile);

	void unloadCurrentLevel();
};
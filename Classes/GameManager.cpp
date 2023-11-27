#include "GameManager.h"

GameManager::GameManager()
{
}

Vec2 GameManager::pixelToCoord(Point pixelCoord, Size spriteSize)
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	pixelCoord.y = visibleSize.height - pixelCoord.y;
	Vec2 res = { floor((pixelCoord.x - origin.x)  / spriteSize.width), floor((pixelCoord.y + origin.y) / spriteSize.height) };
	return res;
}

Point GameManager::coordToPixel(Vec2 boardCoord, Size spriteSize)
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	origin.y += visibleSize.height;

	Point res = {origin.x + boardCoord.x * spriteSize.width + spriteSize.width / 2, 
				 origin.y - boardCoord.y * spriteSize.height - spriteSize.height / 2};

	return res;
}

bool GameManager::loadLevel(int level)
{
	unloadCurrentLevel();
	std::string fileName = "levels/level_" + std::to_string(level) + ".txt";
	std::ifstream levelFile(fileName);

	if (levelFile.is_open())
	{
		while (levelFile) 
		{
			std::string tag;
			levelFile >> tag;

			if (tag == "-map")
			{
				readMap(levelFile);
			}
			else if (tag == "-wave")
			{
				readWave(levelFile);
			}
			else if (tag == "-tower")
			{
				readTower(levelFile);
			}
			else if (tag == "-startMoney")
			{
				levelFile >> this->playerMoney;
			}
		}
		
		levelFile.close();

		this->levelInProgress = true;
		return true;
	}

	return false;
}

bool GameManager::prepareWave(int waveNumber)
{
	if (waveNumber >= waves.size())
	{
		this->levelInProgress = false;
		return false;
	}

	enemies.clear();

	currentWave = waveNumber;
	int overallEnemyCount =
		waves[waveNumber].baseEnemyCount +
		waves[waveNumber].normEnemyCount +
		waves[waveNumber].thickEnemyCount +
		waves[waveNumber].fastEnemyCount;

	while (overallEnemyCount > 0)
	{
		int nextEnemy = RandomHelper::random_int(0, 3);

		switch (nextEnemy)
		{
		case 0:
			if (waves[waveNumber].baseEnemyCount > 0)
			{
				enemies::BaseEnemy enemy = enemies::BaseEnemy();
				this->enemies.push_back(std::make_unique<enemies::BaseEnemy>(enemy));
				waves[waveNumber].baseEnemyCount--;
				overallEnemyCount--;
			}
			break;
		case 1:
			if (waves[waveNumber].normEnemyCount > 0)
			{
				enemies::NormalEnemy enemy = enemies::NormalEnemy();
				this->enemies.push_back(std::make_unique<enemies::NormalEnemy>(enemy));
				waves[waveNumber].normEnemyCount--;
				overallEnemyCount--;
			}
			break;
		case 2:
			if (waves[waveNumber].thickEnemyCount > 0)
			{
				enemies::ThickEnemy enemy = enemies::ThickEnemy();
				this->enemies.push_back(std::make_unique<enemies::ThickEnemy>(enemy));
				waves[waveNumber].thickEnemyCount--;
				overallEnemyCount--;
			}
			break;
		case 3:
			if (waves[waveNumber].fastEnemyCount > 0)
			{
				enemies::FastEnemy enemy = enemies::FastEnemy();
				this->enemies.push_back(std::make_unique<enemies::FastEnemy>(enemy));
				waves[waveNumber].fastEnemyCount--;
				overallEnemyCount--;
			}
			break;
		default:
			break;
		}
	}

	return true;
}

int GameManager::nextLevel()
{
	this->currentLevel++;
	return this->currentLevel;
}

Tower* GameManager::getTower()
{
	return &objTower;
}

Point GameManager::getEnemyStart()
{
	return this->enemyStart;
}

std::vector<std::vector<BlockTypes>>& GameManager::getCurrentLevelMatrix()
{
	return this->levelData;
}

std::list<std::unique_ptr<enemies::BaseEnemy>>& GameManager::getEnemies()
{
	return this->enemies;
}

std::list<std::unique_ptr<defences::BaseTurret>>& GameManager::getDefences()
{
	return this->defences;
}

std::vector<Wave>& GameManager::getWaves()
{
	return waves;
}

int GameManager::getCurrentWave()
{
	return this->currentWave;
}

bool GameManager::isLevelInProgress()
{
	return this->levelInProgress;
}

float GameManager::getPlayerMoney()
{
	return this->playerMoney;
}

void GameManager::setPlayerMoney(float money)
{
	this->playerMoney = money;
}

void GameManager::update(float delta)
{
	for (auto& turret : defences)
	{
		turret.get()->update(enemies, delta);
	}
	for (auto& enemy : enemies)
	{
		updateEnemy(enemy.get(), delta);
	}
	
	objTower.update();
}

void GameManager::updateEnemy(enemies::BaseEnemy* enemy, float delta)
{
	if (enemy->getState() != enemies::drawn)
	{
		if (enemy->getState() == enemies::dead)
		{
			this->playerMoney += enemy->getPoints();
			enemy->setPoints(0);
		}
			
		return;
	}
	enemies::Direction moveDirection = enemy->getMoveDirection();

	if (moveDirection == enemies::Direction::none)
	{
		Size size = enemy->getSprite()->getContentSize();
		float scale = enemy->getSprite()->getScale();
		Vec2 pos = enemy->getSprite()->getPosition();

		size.width *= scale;
		size.height *= scale;

		int h = levelData.size(), w = levelData[0].size();

		Point curPos = pixelToCoord(pos, size);

		Point dirs[5];
		dirs[1] = { curPos.x, curPos.y - 1 }; // up
		dirs[2] = { curPos.x + 1, curPos.y }; // right
		dirs[3] = { curPos.x, curPos.y + 1 }; // down
		dirs[4] = { curPos.x - 1, curPos.y };// left

		for (int i = 1; i < 5; i++)
		{
			if (dirs[i].x >= 0 && dirs[i].x < w && dirs[i].y >= 0 && dirs[i].y < h)
			{
				enemies::Direction newDirection = (enemies::Direction)i;
				if (enemyAbleToMove(enemy, newDirection))
				{
					enemy->setMoveDirection(newDirection);
					break;
				}
			}
		}
	}

	enemy->attack(&this->objTower, delta);

	if (enemyAbleToMove(enemy, enemy->getMoveDirection()))
	{
		moveEnemy(enemy, enemy->getMoveDirection(), delta);
	}
	else
	{
		enemy->setMoveDirection(enemies::Direction::none);
	}
}

bool GameManager::enemyAbleToMove(enemies::BaseEnemy* enemy, enemies::Direction direction)
{
	if (enemy->getState() != enemies::drawn)
		return false;
	auto position = enemy->getSprite()->getPosition();
	auto size = enemy->getSprite()->getContentSize();
	auto scale = enemy->getSprite()->getScale();
	bool res = false;

	size.width *= scale;
	size.height *= scale;

	int h = levelData.size(), w = levelData[0].size();

	switch (direction)
	{
	case enemies::none:
		break;
	case enemies::left:
		position.x -= size.width / 2 + 1;
		break;
	case enemies::right:
		position.x += size.width / 2 + 1;
		break;
	case enemies::up:
		position.y += size.height / 2 + 1;
		break;
	case enemies::down:
		position.y -= size.height / 2 + 1;
		break;
	default:
		break;
	}

	Vec2 curCoord = pixelToCoord({ position.x, position.y}, size);


	if (curCoord.y >= 0 && curCoord.y < h && curCoord.x >= 0 && curCoord.x < w && 
		this->levelData[curCoord.y][curCoord.x] == BlockTypes::road && 
		enemy->getPrevDirection() != direction)
	{
		res = true;
	}

	return res;
}

void GameManager::moveEnemy(enemies::BaseEnemy* enemy, enemies::Direction direction, float delta)
{
	auto position = enemy->getSprite()->getPosition();
	double speed = enemy->getSpeed();

	switch (direction)
	{
	case enemies::none:
		break;
	case enemies::left:
		enemy->setPrevDirection(enemies::right);
		position.x -= speed * delta;
		break;
	case enemies::right:
		enemy->setPrevDirection(enemies::left);
		position.x += speed * delta;
		break;
	case enemies::up:
		enemy->setPrevDirection(enemies::down);
		position.y += speed * delta;
		break;
	case enemies::down:
		enemy->setPrevDirection(enemies::up);
		position.y -= speed * delta;
		break;
	default:
		break;
	}
	enemy->getSprite()->setPosition(position);
}

bool GameManager::readMap(std::ifstream& levelFile)
{
	int rows, cols;
	levelFile >> rows >> cols;
	this->levelData = std::vector< std::vector<BlockTypes> >(rows, std::vector<BlockTypes>(cols));

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			char block;
			levelFile >> block;
			switch (block)
			{
			case 's':
				levelData[i][j] = road;
				enemyStart = { (float)j, (float)i };
				break;
			case 'w':
				levelData[i][j] = wall;
				break;
			case 'r':
				levelData[i][j] = road;
				break;
			case 'm':
				levelData[i][j] = mount;
				break;
			case 't':
				levelData[i][j] = tower;
				objTower = Tower({ (float)j, (float)i });
				break;
			default:
				levelData[i][j] = unknown;
				break;
			}
		}
	}

	return true;
}

bool GameManager::readWave(std::ifstream& levelFile)
{
	Wave wave;

	levelFile >> wave.waveInterval >> wave.enemyInterval;
	levelFile >> wave.baseEnemyCount;
	levelFile >> wave.normEnemyCount;
	levelFile >> wave.thickEnemyCount;
	levelFile >> wave.fastEnemyCount;
	waves.push_back(wave);
	return true;
}

bool GameManager::readTower(std::ifstream& levelFile)
{
	float towerHP;
	levelFile >> towerHP;
	this->objTower.setNewHealth(towerHP);
	return true;
}

void GameManager::unloadCurrentLevel()
{
	currentWave = -1;
	levelData.clear();
	enemies.clear();
	defences.clear();
	waves.clear();
}

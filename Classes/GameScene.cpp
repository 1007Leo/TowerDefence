/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include <fstream>

#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseUp = CC_CALLBACK_1(GameScene::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    auto _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    gameManager = GameManager();

    /*auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();*/


    this->scheduleUpdate();
    return true;
}

bool GameScene::setLevel(GameManager& gameManager)
{
    this->removeAllChildren();
    std::vector< std::vector<BlockTypes> >& board = gameManager.getCurrentLevelMatrix();
    std::list< std::unique_ptr<enemies::BaseEnemy> >& enemies = gameManager.getEnemies();

    int rows = board.size(), cols = 0;
    if (rows != 0) 
    {
        cols = board[0].size();
    }
    else
    {
        return false;
    }
    std::vector< std::vector<Sprite*> > levelSpriteMatrix(rows, std::vector<Sprite*>(cols));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            BlockTypes el = board[i][j];
            switch (el)
            {
            case wall:
                levelSpriteMatrix[i][j] = Sprite::create("sprites/levelSprites/wall.png");
                break;
            case mount:
                levelSpriteMatrix[i][j] = Sprite::create("sprites/levelSprites/mount.png");
                break;
            case road:
                levelSpriteMatrix[i][j] = Sprite::create("sprites/levelSprites/road.png");
                break;
            case tower:
                levelSpriteMatrix[i][j] = Sprite::create("sprites/levelSprites/road.png");
                break;
            default:
                levelSpriteMatrix[i][j] = Sprite::create("sprites/levelSprites/noTexture.png");
                break;
            }

            if (levelSpriteMatrix[i][j] == nullptr)
            {
                //problemLoading("level section '" + el);
                return false;
            }
            else
            {
                setSpriteAtPos(levelSpriteMatrix[i][j], { (float)j, (float)i });
                levelSpriteMatrix[i][j]->setTag(Tags::block);
                this->addChild(levelSpriteMatrix[i][j], 0);   
            }
        }
    }

    setSpriteAtPos(gameManager.getTower()->getSprite(), gameManager.getTower()->getPos());
    this->addChild(gameManager.getTower()->getSprite(), 1);

    float scale = findScale(gameManager.getTower()->getSprite()->getContentSize(), rows, cols);

    setLabels(scale);

    return true;
}

void GameScene::showHideEnemies()
{
    int deadCnt = 0;
    std::list< std::unique_ptr<enemies::BaseEnemy> >& enemiesList = gameManager.getEnemies();
    for (auto& enemy : enemiesList)
    {
        if (enemy.get()->getState() == enemies::State::drawn)
        {
            enemy.get()->getSprite()->setLocalZOrder(1);
        }
        else if (enemy.get()->getState() == enemies::State::dead)
        {
            enemy.get()->getSprite()->setLocalZOrder(-1);
            deadCnt++;
        }
    }
    if (deadCnt == enemiesList.size())
    {
        for (auto& enemy : enemiesList)
        {
            enemy.get()->setState(enemies::State::disabled);
            this->removeChild(enemy.get()->getSprite());
        }
    }
}

void GameScene::setSpriteAtPos(Sprite* sprite, Point pos)
{
    int rows = gameManager.getCurrentLevelMatrix().size(),
        cols = gameManager.getCurrentLevelMatrix()[0].size();

    float scale = findScale(sprite->getContentSize(), rows, cols);
    sprite->setScale(scale);

    Size size = sprite->getContentSize();
    size.width *= scale;
    size.height *= scale;

    sprite->setPosition(gameManager.coordToPixel(pos, size));
}

float GameScene::findScale(Size spriteSize, int rows, int cols)
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    float maxHeight = visibleSize.height,
        maxWidth = (int)(visibleSize.width * 0.66),
        nativeHeight = spriteSize.height * rows,
        nativeWidth = spriteSize.width * cols;


    float scale = std::min(maxHeight / nativeHeight, maxWidth / nativeWidth);
    return scale;
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_1:
        this->selectedTurret = defences::Types::machineGun;
        break;
    case EventKeyboard::KeyCode::KEY_2:
        this->selectedTurret = defences::Types::gun;
        break;
    case EventKeyboard::KeyCode::KEY_3:
        this->selectedTurret = defences::Types::artillery;
        break;
    default:
        break;
    }
}

void GameScene::update(float delta)
{
    if (gameManager.isLevelInProgress())
    {
        setNextWaveIfNoEnemies();
        gameManager.update(delta);
        showHideEnemies();
        updateLabels();
    }
    else
    {
        if (!gameManager.loadLevel(gameManager.nextLevel()))
        {
            Director::getInstance()->end();
        }
        else
        {
            if (!setLevel(gameManager))
            {
                Director::getInstance()->end();
            }
        }
    }
}

bool GameScene::setNextWaveIfNoEnemies()
{
    for (auto& enemy : gameManager.getEnemies()) 
    {
        if (enemy.get()->getState() != enemies::State::disabled)
            return true;
    }

    int nextWave = gameManager.getCurrentWave() + 1;

    if (!gameManager.prepareWave(nextWave))
        return false;

    for (auto& enemy : gameManager.getEnemies())
    {
            setSpriteAtPos(enemy.get()->getSprite(), gameManager.getEnemyStart());
            enemy.get()->getSprite()->setTag(Tags::enemy);
            this->addChild(enemy.get()->getSprite(), -1);
            enemy.get()->setState(enemies::State::enabled);
    }

    float waveInterval = gameManager.getWaves()[gameManager.getCurrentWave()].waveInterval;
    auto waveSequence = Sequence::createWithTwoActions(
        DelayTime::create(waveInterval),
        CallFunc::create([&]()
            {
                float enemyInterval = gameManager.getWaves()[gameManager.getCurrentWave()].enemyInterval;
                int ind = 0;
                for (auto& enemy : gameManager.getEnemies())
                {
                    auto enemySequence = Sequence::createWithTwoActions(
                        DelayTime::create(enemyInterval * ind),
                        CallFunc::create([&]()
                            {
                                enemy.get()->setState(enemies::State::drawn);
                            }
                    ));
                    runAction(enemySequence);
                    ind++;
                }
            }
    ));
    runAction(waveSequence);
    return true;
}

bool GameScene::setLabels(float scale)
{
    labels.clear();

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    std::string currentMoney = "Current money: " + std::to_string((int)gameManager.getPlayerMoney());

    auto moneyLabel = Label::createWithTTF(currentMoney, "fonts/Marker Felt.ttf", 16);
    moneyLabel->setTag(Tags::labelMoney);
    auto selectedTurretLabel = Label::createWithTTF("Selected turret: \nnone\n", "fonts/Marker Felt.ttf", 16 );
    selectedTurretLabel->setTag(Tags::labelSelected);
    auto turretsSelecorLabel = Label::createWithTTF("\nKeyboard keys\n1 - machinegun\n2 - regular gun\n3 - artillery", "fonts/Marker Felt.ttf", 16);
    turretsSelecorLabel->setTag(Tags::label);

    labels.push_back(moneyLabel);
    labels.push_back(selectedTurretLabel);
    labels.push_back(turretsSelecorLabel);

    int ind = 0;
    for (auto& label : labels)
    {
        label->setPosition(
            Vec2(origin.x + visibleSize.width * 0.66 + label->getContentSize().width / 2 + 10,
                 origin.y + visibleSize.height - label->getContentSize().height - 20 - label->getContentSize().height*ind));
        this->addChild(label, 1);
    }
    return true;
}

void GameScene::updateLabels()
{
    for (auto& label : labels)
    {
        if (label->getTag() == Tags::labelMoney)
        {
            label->setString("Current money: " + std::to_string((int)gameManager.getPlayerMoney()));
        }
        if (label->getTag() == Tags::labelSelected)
        {
            std::string selectedStr = "";

            switch (this->selectedTurret)
            {
            case defences::Types::none:
                selectedStr = "None";
                break;
            case defences::Types::baseTurret:
                selectedStr = "Base turret";
                break;
            case defences::Types::machineGun:
                selectedStr = "Machinegun";
                break;
            case defences::Types::gun:
                selectedStr = "Regular gun";
                break;
            case defences::Types::artillery:
                selectedStr = "Artillery";
                break;
            default:
                break;
            }

            label->setString("Selected turret: \n" + selectedStr + "\n");
        }
    }

}

bool GameScene::setTurret(Point coord, Size size, defences::Types turretType)
{
    auto it = gameManager.getDefences().begin();
    for (it; it != gameManager.getDefences().end(); it++)
    {
        Point itCoord = gameManager.pixelToCoord(it->get()->getSprite()->getPosition(), size);
        if (itCoord == coord)
        {
            this->removeChild(it->get()->getSprite());
            this->removeChild(it->get()->getHitSprite());
            gameManager.getDefences().erase(it);
            break;
        }
    }

    float turretSize = std::min(size.width, size.height);

    switch (turretType)
    {
    case defences::Types::none:
        return false;
        break;
    case defences::Types::baseTurret:
        gameManager.getDefences().push_back(std::make_unique<defences::BaseTurret>(defences::BaseTurret(turretSize)));
        break;
    case defences::Types::machineGun:
        gameManager.getDefences().push_back(std::make_unique<defences::MachineGun>(defences::MachineGun(turretSize)));
        break;
    case defences::Types::gun:
        gameManager.getDefences().push_back(std::make_unique<defences::Gun>(defences::Gun(turretSize)));
        break;
    case defences::Types::artillery:
        gameManager.getDefences().push_back(std::make_unique<defences::Artillery>(defences::Artillery(turretSize)));
        break;
    default:
        return false;
        break;
    }

    defences::BaseTurret* turret = gameManager.getDefences().back().get();
    if (turret->getPrice() > gameManager.getPlayerMoney())
    {
        gameManager.getDefences().pop_back();
        return false;
    }
    else
    {
        gameManager.setPlayerMoney(gameManager.getPlayerMoney() - turret->getPrice());
    }
    
    setSpriteAtPos(turret->getSprite(), coord);
    setSpriteAtPos(turret->getHitSprite(), coord);
    turret->getSprite()->setTag(Tags::defence);
    turret->getHitSprite()->setTag(Tags::hitMark);
    this->addChild(turret->getSprite(), 1);
    this->addChild(turret->getHitSprite(), -1);
    
    return true;
}

void GameScene::onMouseUp(Event* event)
{
    EventMouse* e = (EventMouse*)event;

    Size size = this->getChildByTag(Tags::block)->getContentSize();
    float scale = this->getChildByTag(Tags::block)->getScale();
    size.width *= scale;
    size.height *= scale;
    Point coord = gameManager.pixelToCoord({ e->getCursorX(), e->getCursorY() }, size);

    auto board = gameManager.getCurrentLevelMatrix();
    int rows = board.size(), cols = board[0].size();
    
    if ((int)coord.y >= 0 && (int)coord.y < rows && (int)coord.x >= 0 && (int)coord.x < cols)
    {
        if (board[(int)coord.y][(int)coord.x] == BlockTypes::mount)
        {
            if (this->selectedTurret != defences::Types::none)
            {
                if (setTurret(coord, size, this->selectedTurret))
                    this->selectedTurret = defences::Types::none;
            }
        }
    }
}
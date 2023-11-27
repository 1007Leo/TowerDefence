#pragma once
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "GameManager.h"

USING_NS_CC;

const enum Tags { block = 0, enemy = 1, defence = 2, hitMark = 3, label = 4, labelMoney = 5, labelSelected = 6};

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    bool setLevel(GameManager& gameManager);
    void showHideEnemies();
    void setSpriteAtPos(Sprite* sprite, Point pos);

    static float findScale(Size spriteSize, int rows, int cols);

    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

    void update(float delta) override;
private:
    GameManager gameManager;
    defences::Types selectedTurret;
    std::list <Label*> labels;

    bool setNextWaveIfNoEnemies();
    bool setLabels(float scale);
    void updateLabels();
    bool setTurret(Point coord, Size size, defences::Types turretType);

    void GameScene::onMouseUp(Event* event);
};

#endif // __GAME_SCENE_H__

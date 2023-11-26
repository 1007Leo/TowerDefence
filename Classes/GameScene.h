#pragma once
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "GameManager.h"

USING_NS_CC;

const enum Tags { block = 0, enemy = 1, defence = 2, hitMark = 3};

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    bool setLevel(GameManager& gameManager);
    void showHideEnemies();
    void setSpriteAtPos(Sprite* sprite, Point pos);

    static float findScale(Size spriteSize, int rows, int cols);

    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

    void update(float delta) override;
private:
    GameManager gameManager;

    bool setNextWaveIfNoEnemies();

    void GameScene::onMouseUp(Event* event);
};

#endif // __GAME_SCENE_H__

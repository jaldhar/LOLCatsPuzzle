#ifndef LOLCATSPUZZLESCENE_H
#define LOLCATSPUZZLESCENE_H

#include "cocos2d.h"
#include "LOLCatsPuzzle.h"

class LOLCatsPuzzleScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LOLCatsPuzzleScene);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
private:
    void redraw();

    std::array<std::array<cocos2d::Sprite*, 4>, 6> _tiles;
    cocos2d::Sprite* _dragImage;
    int _dragX;
    int _dragY;
    LOLCatsPuzzle _puzzle;
    bool _running;


};

#endif // LOLCATSPUZZLESCENE_H

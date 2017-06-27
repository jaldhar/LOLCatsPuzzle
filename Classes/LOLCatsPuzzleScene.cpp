#include "LOLCatsPuzzleScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace std;

static const int TILEHEIGHT = 300;
static const int TILEWIDTH  = 360;

static const int FONTSIZE = 64;
static const int ICONSIZE = 128;
static const int XMARGIN = 96;
static const int YMARGIN = 248;

Scene* LOLCatsPuzzleScene::createScene() {
    // 'layer' is an autorelease object
    return LOLCatsPuzzleScene::create();
}

// on "init" you need to initialize your instance
bool LOLCatsPuzzleScene::init()  {
    if ( !Scene::init() ) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float scale = Director::getInstance()->getContentScaleFactor();

    srand(time(NULL));

    _puzzle.reset();
    _running = false;

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets.plist");

    for (int i = 0; i < _puzzle.height(); i++) {
        for (int j = 0; j < _puzzle.width(); j++) {
            ostringstream name;
            name << "piece" << i << j << ".png";
            _tiles[i][j] = Sprite::createWithSpriteFrameName(name.str());
            _tiles[i][j]->setScale(scale);
            _tiles[i][j]->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
            this->addChild(_tiles[i][j]);
        }
    }

    _dragX = -1;
    _dragY = -1;
    _dragImage = nullptr;

    auto resetItem = MenuItemSprite::create(Sprite::createWithSpriteFrameName("reset.png"),
                                            nullptr, [&](Ref* sender){
                _puzzle.reset();
                _running = false;
                redraw();
            });

    auto shuffleItem = MenuItemSprite::create(Sprite::createWithSpriteFrameName("shuffle.png"),
                                              nullptr, [&](Ref* sender){
                _puzzle.shuffle();
                _running = true;
                redraw();
            });

    auto helpItem = MenuItemSprite::create(Sprite::createWithSpriteFrameName("help.png"),
                                           nullptr, [&](Ref* sender){
                // TODO: Make Help screen.
            });

    auto returnItem = MenuItemSprite::create(Sprite::createWithSpriteFrameName("return.png"),
                                             nullptr, [&](Ref* sender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
                MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    	return;
#endif

                Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                exit(0);
#endif
            });

    auto menu = Menu::create(shuffleItem, resetItem, helpItem, returnItem, nullptr);
    menu->setPosition(visibleSize.width / 2, 64);
    menu->alignItemsHorizontally();
    this->addChild(menu, 2);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LOLCatsPuzzleScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LOLCatsPuzzleScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LOLCatsPuzzleScene::onTouchEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    redraw();

    return true;
}

bool LOLCatsPuzzleScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!_running)
        return false;
    Point p = touch->getLocationInView();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    float scale = Director::getInstance()->getContentScaleFactor();

    int height = TILEHEIGHT * scale;
    int width = TILEWIDTH * scale;
    int top = 0 + (scale * YMARGIN) / 2;
    int left = 0 + (scale * XMARGIN) / 2;
    int right = left + _puzzle.width() * width;
    int bottom = top + _puzzle.height() * height;
    if (p.x < left || p.x >= right || p.y < top || p.y >= bottom) {
        return false;
    } else {
        _dragX = (p.x - left) / width;
        _dragY = (p.y - top) / height;
        pair<int, int> tile = _puzzle.convert(_dragY, _dragX);
        _dragImage = Sprite::create();
        _dragImage->setDisplayFrame(_tiles[tile.first][tile.second]->getDisplayFrame());
        _dragImage->setScale(scale);
        this->addChild(_dragImage);
        _dragImage->runAction(RepeatForever::create(Blink::create(0.33f, 1)));
    }
    return true;
}

void LOLCatsPuzzleScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!_running)
        return;
    Point p = touch->getLocation();
    if (_dragImage) {
        _dragImage->setPosition(p);
    }
}

void LOLCatsPuzzleScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!_running)
        return;
    Point p = touch->getLocationInView();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    float scale = Director::getInstance()->getContentScaleFactor();
    int height = TILEHEIGHT * scale;
    int width = TILEWIDTH * scale;
    int top = 0 + (scale * YMARGIN) / 2;
    int left = 0 + (scale * XMARGIN) / 2;
    int right = left + _puzzle.width() * width;
    int bottom = top + _puzzle.height() * height;
    if (p.x < left || p.x >= right || p.y < top || p.y >= bottom) {
        // TODO: beep or something...
    } else {
        int dropX = (p.x - left) / width;
        int dropY = (p.y - top) / height;
        _puzzle.move(_dragY, _dragX, dropY, dropX);
        redraw();
        if (_puzzle.success()) {
            // TODO: Do something on success
            _running = false;
        }
    }
    if (_dragImage) {
        this->removeChild(_dragImage, true);
    }
    _dragX = -1;
    _dragY = -1;
}

void LOLCatsPuzzleScene::redraw() {
    Size visibleSize = Director::getInstance()->getVisibleSize();

    float scale = Director::getInstance()->getContentScaleFactor();
    int height = TILEHEIGHT * scale;
    int width = TILEWIDTH * scale;
    int top = visibleSize.height - (scale * YMARGIN) / 2;
    int left = 0 + (scale * XMARGIN) / 2;

    for (int i = 0; i < _puzzle.height(); i++) {
        for (int j = 0; j < _puzzle.width(); j++) {
            pair<int, int> k = _puzzle.convert(i, j);
            _tiles[k.first][k.second]->setPosition(left + j * width, top - i * height);
        }
    }
}

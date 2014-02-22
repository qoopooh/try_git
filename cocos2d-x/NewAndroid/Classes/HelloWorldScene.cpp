#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    //if ( !CCLayer::init() )
    ccColor4B myColor = ccc4(255, 255, 255, 255);
    if ( !CCLayerColor::initWithColor(myColor))
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
    pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *player = CCSprite::create("Player.png",
        CCRectMake(0, 0, 27, 40));
    player->setPosition(ccp(player->getContentSize().width/2, winSize.height/2));
    this->addChild(player);

    this->schedule(schedule_selector(HelloWorld::gameLogic), 1.0);
    this->setTouchEnabled(true);

    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::addTarget()
{
  CCSprite *target = CCSprite::create("Target.png", CCRectMake(0, 0, 27, 40));
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  int minY = target->getContentSize().height/2;
  int maxY = winSize.height - target->getContentSize().height/2;
  int rangeY = maxY - minY;
  // srand(TimGetTicks());
  int actualY = ( rand() % rangeY ) + minY;
  target->setPosition(ccp(winSize.width + (target->getContentSize().width/2),
        actualY));
  this->addChild(target);

  // Determine speed of the target
  int minDuration = (int) 2.0;
  int maxDuration = (int) 4.0;
  int rangeDuration = maxDuration - minDuration;
  // srand(TimGetTicks());
  int actualDuration = (rand() % rangeDuration) + minDuration;

  // Create the actions
  CCFiniteTimeAction *actionMove = CCMoveTo::create((float) actualDuration,
      ccp(0 - target->getContentSize().width/2, actualY));
  CCFiniteTimeAction *actionMoveDone = CCCallFuncN::create(this,
      callfuncN_selector(HelloWorld::spriteMoveFinished));
  target->runAction(CCSequence::create(actionMove,
        actionMoveDone, NULL));
}

void HelloWorld::spriteMoveFinished(CCNode *sender)
{
  CCSprite *sprite = (CCSprite *) sender;
  this->removeChild(sprite, true);
}

void HelloWorld::gameLogic(float dt)
{
  this->addTarget();
}

void HelloWorld::ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent)
{
  CCTouch *touch = (CCTouch *) (pTouches->anyObject());
  CCPoint location = touch->getLocationInView();
  location = CCDirector::sharedDirector()->convertToGL(location);

  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  CCSprite *projectile = CCSprite::create("Projectile.png",
      CCRectMake(0, 0, 20, 20));
  projectile->setPosition(ccp(20, winSize.height/2));

  int offX = location.x - projectile->getPosition().x;
  int offY = location.y - projectile->getPosition().y;

  if (offX <= 0) return;

  this->addChild(projectile);

  int realX = winSize.width + (projectile->getContentSize().width/2);
  float ratio = (float) offY / (float) offX;
  int realY = (realX * ratio) + projectile->getPosition().y;
  CCPoint realDest = ccp(realX, realY);
  
  int offRealX = realX - projectile->getPosition().x;
  int offRealY = realY - projectile->getPosition().y;
  float length = sqrtf((offRealX * offRealX)
      + (offRealY * offRealY));
  float velocity = 480/1;
  float realMoveDuration = length/velocity;

  projectile->runAction(CCSequence::create(
        CCMoveTo::create(realMoveDuration, realDest),
        CCCallFuncN::create(this,
          callfuncN_selector(HelloWorld::spriteMoveFinished)),
        NULL));
}



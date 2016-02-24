#include "HelloWorldScene.h"
#include"Hero.h"
#include<iostream>
USING_NS_CC;
using namespace std;


const int LEFT = 0;
const int RIGHT = 1;
int t = 5;


HelloWorldHud*HelloWorld::_hud = NULL;//static 成员
bool HelloWorldHud::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	label = LabelTTF::create("0", "fonts/Marker Felt.ttf", 18.0f, Size(50, 20));
	label->setColor(Color3B(0, 0, 0));
	int margin = 10;
	label->setPosition(visibleSize.width - (label->getDimensions().width / 2) - margin, label->getDimensions().height / 2 + margin);
	this->addChild(label);

	return true;
}
//计分Layer 操作label
void HelloWorldHud::numberCollectedChanged(int numberCollected)
{
	char showStr[20];
	sprintf(showStr, "%d", numberCollected);//这里是int 转换为String，也可以用之前的方法
	label->setString(showStr);

}
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);
	auto hudLayer = HelloWorldHud::create();
	_hud = hudLayer;
	scene->addChild(hudLayer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	//schdule 函数
	scheduleUpdate();
	
	std::string file = "TileMap.tmx";
	auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
	tileMap = TMXTiledMap::createWithXML(str->getCString(), "");
	//background = tileMap->layerNamed("Background");
	background = tileMap->getLayer("Background");
	meta = tileMap->getLayer("Meta");
	meta->setVisible(false);
	foreground = tileMap->getLayer("Foreground");


	TMXObjectGroup*objects = tileMap->getObjectGroup("Objects");
	//CCASSERT(NULL != objects, "'object groups not found'");
	auto spawnPoint = objects->getObject("SpawnPoint");
	//CCASSERT(!spawnPoint.empty(), "'spawnPoint object not found'");

	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();
	player = Sprite::create("greenr01.png");
	player->setPosition(x, y);
	player->setAnchorPoint(Point(0.5f, 0));
	addChild(player);
	setViewPointCenter(player->getPosition());

	/*Label*lable = Label::create();
	lable->setPosition(120, 100);
	addChild(lable);
	int wstr = tileMap->getMapSize().width;
	String* strScore = String::createWithFormat("%d", wstr);
	lable->setString(strScore->getCString());*/

	addChild(tileMap, -1);
	//创建敌人
	
	for (auto& eSpawnPoint : objects->getObjects())
	{
		ValueMap& dict = eSpawnPoint.asValueMap();
		if (dict["Enermy"].asInt() == 1)
		{
			x = dict["x"].asInt();
			y = dict["y"].asInt();
			this->addEnermyAtPos(Point(x, y));
			
		}
	}

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
	//点击事件的监听
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [](Touch* t, Event* e)
	{
		return true;
	};
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	//键盘事件的监听
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
	numCollected = 0;
	direction = LEFT;//初始化direction
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void HelloWorld::setViewPointCenter(Point position)
{
	auto winSize = Director::getInstance()->getWinSize();
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, tileMap->getMapSize().width*tileMap->getTileSize().width - winSize.width / 2);
	y = MIN(y, tileMap->getMapSize().height*tileMap->getTileSize().height - winSize.height / 2);
	auto actualPosition = Point(x, y);
	auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
	auto viewPoint = centerOfView - actualPosition;
	this->setPosition(viewPoint);

	
	
}
//游戏中武器和角色在一个layer上，为了效率，会考虑将bullet, effect和 PhysicsParticle分别放到不用的层上，对应的层上使用batchnode来提高效率
//
//武器和PhysicsParticleLauncher(粒子发射器)绑定，发射的时候，会向PhysicsParticleLayer的武器相同的位置上生成一个物理粒子特效
//
//会经常用到convertToNodeSpace和convertToWorldSpace转换坐标
void HelloWorld::setPlayerPosition(Point position)
{
	Point tileCoord = this->tileCoordForPosition(position);
	int tileGid = meta->getTileGIDAt(tileCoord);
	if (tileGid)
	{
		auto properties = tileMap->getPropertiesForGID(tileGid).asValueMap();
		if (!properties.empty()){
			auto collision = properties["Collidable"].asString();
			if ("true" == collision)
			{
		
				return;
			}
			auto collection = properties["Collectable"].asString();
			if ("true" ==collection)
			{
				meta->removeTileAt(tileCoord);
				foreground->removeTileAt(tileCoord);
				this->numCollected++;
				this->_hud->numberCollectedChanged(numCollected);
			}
		}
		
	}
	
	player->setPosition(position);
}
void HelloWorld::onTouchEnded(Touch *t, Event*e)
{
	//this 是什么？是根据this 为基准点来算相对坐标的
	auto touchLocation = t->getLocation();
	touchLocation = this->convertToNodeSpace(touchLocation);//转换坐标系
	auto playerPos = player->getPosition();
	auto diff = touchLocation - playerPos;
	
	if (abs(diff.x) > abs(diff.y)) {
		if (diff.x > 0) {
			playerPos.x += tileMap->getTileSize().width;
		}
		else {
			playerPos.x -= tileMap->getTileSize().width;
		}
	}
	else {
		if (diff.y > 0) {
			playerPos.y += tileMap->getTileSize().height;
		}
		else {
			playerPos.y -= tileMap->getTileSize().height;
		}
	}
	//????
	auto winSize = Director::getInstance()->getWinSize();
	if (playerPos.x <=(tileMap->getMapSize().width * tileMap->getTileSize().width)&&
		playerPos.y<=(tileMap-> getMapSize().height * tileMap->getTileSize().height)&&
		playerPos.y >= 0 &&
		playerPos.x >= 0)
	{
		this->setPlayerPosition(playerPos);
		
	}
	this->setViewPointCenter(player->getPosition());
}

Point HelloWorld::tileCoordForPosition(Point position)//坐标转换，cocos2d 转换tile地图坐标
{
	int x = position.x / tileMap->getTileSize().width;
	int y = (tileMap->getMapSize().height*tileMap->getTileSize().height - position.y) / tileMap->getTileSize().height;
	return Point(x, y);
}

Animate* HelloWorld::heroMoved(int direction)
{
	Animation* animationl = Animation::create();
	Animation*animationr = Animation::create();
	for (int i = 1; i <= 5; i++)
	{
		String* stringl = String::createWithFormat("green0%d.png", i);
		String *stringr = String::createWithFormat("greenr0%d.png", i);
		SpriteFrame*framel = SpriteFrame::create(stringl->getCString(),Rect(0,0,33,65));
		SpriteFrame*framer = SpriteFrame::create(stringr->getCString(), Rect(0, 0, 33, 65));
		animationl->addSpriteFrame(framel);
		animationr->addSpriteFrame(framer);


	}
	animationl->setDelayPerUnit(0.15f);
	animationr->setDelayPerUnit(0.15f);
	animationl->setLoops(-1);
	animationr->setLoops(-1);
	animationl->setRestoreOriginalFrame(true);
	animationr->setRestoreOriginalFrame(true);

	Animate*animatel = Animate::create(animationl);
	Animate*animater = Animate::create(animationr);

	if (direction == LEFT)
	{
		return animatel;
	}
	else if (direction == RIGHT)
		return animater;
	
}
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* e)
{
	float currentX = player->getPosition().x;
	float currentY = player->getPosition().y;

	
	
	if (keyCode == EventKeyboard::KeyCode::KEY_W)
	{
		direction = LEFT;
		Animate* animate = heroMoved(LEFT);
		
		
		moveto = MoveBy::create(0.01*ccpDistance(player->getPosition(), Vec2(currentX, currentY + 30 * t)), Vec2(0,  + 30 * t));
			player->runAction(Spawn::create(animate, moveto, NULL));
		

	}
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		direction = RIGHT;
		Animate* animate = heroMoved(RIGHT);
		
		
		moveto = MoveBy::create(0.01*ccpDistance(player->getPosition(), Vec2(currentX, currentY - 30 * t)), Vec2(0,  - 30 * t));
			player->runAction(Spawn::create(animate, moveto, NULL));
	
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		direction = LEFT;
		Animate* animate = heroMoved(LEFT);
		
		
		moveto = MoveBy::create(0.01*ccpDistance(player->getPosition(), Vec2(currentX - 30 * t, currentY)), Vec2( - 30 * t, 0));
			player->runAction(Spawn::create(animate, moveto, NULL));
		
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		direction = RIGHT;
		Animate* animate = heroMoved(RIGHT);
		
		moveto = MoveBy::create(0.01*ccpDistance(player->getPosition(), Vec2(currentX + 30 * t, currentY)), Vec2( + 30 * t, 0));
			player->runAction(Spawn::create(animate, moveto, NULL));


	}
}
void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event*e)
{
	
	t = 5;
	player->stopAllActions();
	Point position=player->getPosition();
	removeChild(player,true);
	if (direction == LEFT)
	{
		player = Sprite::create("green01.png");
	}
	else if (direction == RIGHT)
	{
		player = Sprite::create("greenr01.png");
	}
	player->setPosition(position);
	player->setAnchorPoint(Point(0.5, 0));
	addChild(player);
}

 void HelloWorld::update(float dt)
{ 
	 t++;
	 auto playerPos = player->getPosition();

	 this->setPlayerPosition(playerPos);
    this->setViewPointCenter(player->getPosition());

}

 bool HelloWorld::predictWalks(Point predictPos)
 {
	 if (predictPos.x <= (tileMap->getMapSize().width * tileMap->getTileSize().width) &&
		 predictPos.y <= (tileMap->getMapSize().height * tileMap->getTileSize().height) &&
		 predictPos.y >= 0 &&
		 predictPos.x >= 0)
	 {


		 Point tileCoord = this->tileCoordForPosition(predictPos);
		 int tileGid = meta->getTileGIDAt(tileCoord);
		 if (tileGid)
		 {
			 auto properties = tileMap->getPropertiesForGID(tileGid).asValueMap();
			 if (!properties.empty()){
				 auto collision = properties["Collidable"].asString();
				 if ("true" == collision)
				 {
					 return true;

				 }
				 else return false;
			 }
		 }
	 }
	 return false;
 }

 void HelloWorld::addEnermyAtPos(Point pos)
 {
	 auto enermy = Sprite::create("enermy1.png");
	 enermy->setPosition(pos);
	 this->addChild(enermy);
	 this->animateEnemy(enermy);
 }
 void HelloWorld::enermyMoveFinished(Object *pSender)
 {
	 
	 Sprite* enermy = (Sprite*)pSender;
	 
	  this->animateEnemy(enermy);
 }
 // a method to move the enemy 10 pixels toward the player
 void HelloWorld::animateEnemy(Sprite *enemy)
 {
	 //speed of the enermy
	 float actualDuration = 0.1f;
	 auto position = (player->getPosition() - enemy->getPosition()).getNormalized();

	 //怪物面朝主角
	 auto diff = player->getPosition() - enemy->getPosition();
	// if (diff.x*diff.x+diff.y*diff.y<=)
		 float angleRadian = atanf((float)diff.y / (float)diff.x);
		 float angleDegree = CC_RADIANS_TO_DEGREES(angleRadian);
		 float cocosAngle = -1 * angleDegree;
		 if (diff.x < 0) {
			 cocosAngle += 180;
		 }
		 enemy->setRotation(cocosAngle);


		 auto actionMove = MoveBy::create(actualDuration, position);
		 auto actionMoveDone = CallFuncN::create(CC_CALLBACK_1(HelloWorld::enermyMoveFinished, this));
		 enemy->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
	 
	
	 
 }
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;


class HelloWorldHud :public Layer
{
public:
	void numberCollectedChanged(int numCollected);
	virtual bool init();
	CREATE_FUNC(HelloWorldHud);
	LabelTTF* label;
};
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void setViewPointCenter(Point position);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	//点击监听
	void onTouchEnded(Touch*t, Event*e);
	void setPlayerPosition(Point position);
    //坐标转换，cocos2d 转换tile地图坐标
	Point tileCoordForPosition(Point point);
	//设置向左向右走动动画
	Animate* heroMoved(int direction);
	//键盘监听
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* e);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event*e);
	//schedule函数
	virtual void update(float dt);
	bool HelloWorld::predictWalks(Point predictPos);
	//在指定的位置添加敌人
	void addEnermyAtPos(Point pos);
	void enermyMoveFinished(Object *pSender);
	// a method to move the enemy 10 pixels toward the player
	void animateEnemy(Sprite *enemy);
private:
	TMXTiledMap *tileMap;
	TMXLayer* background;
	TMXLayer* meta;
	TMXLayer* foreground;
	Sprite* player;
	int numCollected;
	static HelloWorldHud *_hud;
	MoveBy* moveto;
	int direction;//记录运动方向
};


#endif  __HELLOWORLD_SCENE_H__

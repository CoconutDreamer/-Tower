#pragma once
#include"cocos2d.h"
USING_NS_CC;

#include"cocostudio/CocoStudio.h"

class Hero: public Node
{
public:
	Hero();
	~Hero();
	void initHeroSprite(char *hero_name, int ilevel);
	Sprite* getHero();
	void setAnimation(const char* frameName, float delay, int run_direction);
	void stopAnimation();
	void jumpUpAnimation(const char*frameName, float delay, int run_direction);
	void jumpDownAimation(const char* frameName, float delay, int run_direction);
	void jumpEnd();
	void atackAnimation(const char* frameName, float delay, int run_direction);
	void atackEnd();
	void deadAnimation(const char* frameName, float delay, int run_direction);
	void deadEnd();
	void hurtAnimation(const char* frameName, float delay, int run_direction);
	void hurtEnd();
	bool JudgePositosn(Size visibleSize);
	
	bool hIsDead;

	float hCurrentHp;
	float hTotleHp;
	float hCurrentMp;
	float hTotleMp;

	float hpercentage;
	float hSpeed;

	bool hIsAction;          // 查看当前是否已经在打怪了  
	bool hIsJumping;         // 查看是否在跳  
	bool hIsRunning;  // 判断是否在跑动画  
	bool hIsAttack;  // 判断是否在攻击动画  

	bool hIsHurt; // 判断是否受伤  

	bool HeroDirecton; // 英雄运动的方向  
	bool hCanCrazy; // 判断是否处于狂暴状态  

	CREATE_FUNC(Hero);
private:
	Sprite* HeroSprite; // 精灵  
	char *Hero_name; // 用来保存初始状态的精灵图片名称
};


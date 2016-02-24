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

	bool hIsAction;          // �鿴��ǰ�Ƿ��Ѿ��ڴ����  
	bool hIsJumping;         // �鿴�Ƿ�����  
	bool hIsRunning;  // �ж��Ƿ����ܶ���  
	bool hIsAttack;  // �ж��Ƿ��ڹ�������  

	bool hIsHurt; // �ж��Ƿ�����  

	bool HeroDirecton; // Ӣ���˶��ķ���  
	bool hCanCrazy; // �ж��Ƿ��ڿ�״̬  

	CREATE_FUNC(Hero);
private:
	Sprite* HeroSprite; // ����  
	char *Hero_name; // ���������ʼ״̬�ľ���ͼƬ����
};


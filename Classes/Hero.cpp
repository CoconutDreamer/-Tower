#include "Hero.h"
#include"ActionTool.h"

Hero::Hero()
{
}


Hero::~Hero()
{
}
void Hero::initHeroSprite(char *hero_name, int ilevel)
{
	HeroSprite = Sprite::create(hero_name);
}
Sprite* Hero::getHero()
{
	return HeroSprite;
}
void Hero::setAnimation(const char* frameName, float delay, int run_direction)
{
	if (HeroDirecton != run_direction)
	{
		HeroDirecton = run_direction;
		HeroSprite->setFlipX(run_direction);

	}
	if (hIsRunning || hIsHurt || hIsDead)
		return;
	//��������
	Animate* action = ActionTool::animationWithFrameName(frameName, -1, delay);
	HeroSprite->runAction(action);
	hIsRunning = true;

}
void Hero::stopAnimation()
{
	HeroSprite->stopAllActions();
}
void Hero::jumpUpAnimation(const char*frameName, float delay, int run_direction)
{

}
void Hero::jumpDownAimation(const char* frameName, float delay, int run_direction)
{

}
void Hero::jumpEnd()
{

}
void Hero::atackAnimation(const char* frameName, float delay, int run_direction)
{

}
void Hero::atackEnd()
{

}
void Hero::deadAnimation(const char* frameName, float delay, int run_direction)
{

}
void Hero::deadEnd()
{

}
void Hero::hurtAnimation(const char* frameName, float delay, int run_direction)
{
	if (hIsHurt || hIsDead)
		return;
	if (hIsRunning || hIsAttack)
	{
		HeroSprite->stopAllActions();
		this->removeChild(HeroSprite, true);//��ԭ���ľ���ɾ����  
		HeroSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(Hero_name));//�ָ�����ԭ������ͼ����  
		HeroSprite->setFlippedX(HeroDirecton);
		this->addChild(HeroSprite);
		hIsRunning = false;
		hIsAttack = false;
	}
}
void Hero::hurtEnd()
{

}
bool Hero::JudgePositosn(Size visibleSize)
{
	return true;
}
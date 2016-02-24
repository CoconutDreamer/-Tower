#include "ActionTool.h"


ActionTool::ActionTool()
{
}


ActionTool::~ActionTool()
{
}
Animate* ActionTool::animationWithFrameName(const char* frameName, int loops, float delay)
{
	SpriteFrame*frame=NULL;
	Animation* animation = Animation::create();
	int index = 1;
	do
	{
		String* name = String::createWithFormat("%s%d", frameName, index++);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name->getCString());
		if (frame == NULL)
		{
			break;
		}
		animation->addSpriteFrame(frame);
	} while (true);
	animation->setDelayPerUnit(delay);
	animation->setLoops(loops);
	animation->setRestoreOriginalFrame(true);
	Animate* animate = Animate::create(animation);
	return animate;

}
 Animate *ActionTool::animationWithFrameAndNum(const char* frameName, int loops, float delay, int num)
{
	 SpriteFrame* frame = NULL;
	 Animation* animation = Animation::create();
	 for (int i = 1; i <= num; i++)
	 {
		 String* name = String::createWithFormat("%s%d", frameName, i);
		 frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name->getCString());
		 animation->addSpriteFrame(frame);
	 }
	 animation->setDelayPerUnit(delay);
	 animation->setLoops(loops);
	 animation->setRestoreOriginalFrame(true);
	 Animate* animate = Animate::create(animation);
	 return animate;
}

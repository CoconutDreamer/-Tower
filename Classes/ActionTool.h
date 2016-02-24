#pragma once
#include"cocos2d.h"
USING_NS_CC;

class ActionTool
{
public:
	ActionTool();
	~ActionTool();
	static Animate* animationWithFrameName(const char* frameName, int loops, float delay);
	static Animate *animationWithFrameAndNum(const char* frameName, int loops, float delay, int num);

};


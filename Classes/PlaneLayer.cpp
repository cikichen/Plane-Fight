//PlaneLayer.cpp
#include "PlaneLayer.h"


PlaneLayer* PlaneLayer::sharedPlane=NULL;//静态变量要在cpp外初始化

PlaneLayer::PlaneLayer(void)
{
	isAlive=true;
	score=0;
}

PlaneLayer::~PlaneLayer(void)
{
}


PlaneLayer* PlaneLayer::create() 
{ 
	PlaneLayer *pRet = new PlaneLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		sharedPlane=pRet;//获得静态指针sharedPlane的值
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool PlaneLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();

		//创建飞机精灵前要先调用CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("shoot.plist");加载全局资源
		//我把这个调用plist放到welcome.cpp中了。不然plane会空指针。 
		CCSprite* plane=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero1.png"));
		plane->setPosition(ccp(winSize.width/2,plane->getContentSize().height/2));//飞机放置在底部中央
		this->addChild(plane,0,AIRPLANE);//添加精灵，AIRPLANE是tag

		CCBlink *blink=CCBlink::create(1,3);//闪烁动画

		CCAnimation* animation=CCAnimation::create();
		animation->setDelayPerUnit(0.1f);
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero1.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero2.png"));
		CCAnimate* animate=CCAnimate::create(animation);//帧动画

		plane->runAction(blink);//执行闪烁动画
		plane->runAction(CCRepeatForever::create(animate));// 执行帧动画

		bRet=true;
	} while (0);

	return bRet;
}

void PlaneLayer::MoveTo(CCPoint location)
{
	if(isAlive && !CCDirector::sharedDirector()->isPaused())
	{
		CCPoint actualPoint;
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSize planeSize=this->getChildByTag(AIRPLANE)->getContentSize();
		if (location.x<planeSize.width/2)
		{
			location.x=planeSize.width/2;
		}
		if (location.x>winSize.width-planeSize.width/2)
		{
			location.x=winSize.width-planeSize.width/2;
		}
		if (location.y<planeSize.height/2)
		{
			location.y=planeSize.width/2+10;
		}
		if (location.y>winSize.height-planeSize.height/2)
		{
			location.y=winSize.height-planeSize.height/2;
		}
		this->getChildByTag(AIRPLANE)->setPosition(location);
	}
}

void PlaneLayer::Blowup(int passScore)
{
	if(isAlive)
	{
		isAlive=false;
		score=passScore;
		CCAnimation* animation=CCAnimation::create();
		animation->setDelayPerUnit(0.2f);
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n1.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n2.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n3.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n4.png"));

		CCAnimate* animate=CCAnimate::create(animation);
		CCCallFunc* removePlane=CCCallFunc::create(this,callfunc_selector(PlaneLayer::RemovePlane));
		CCSequence* sequence=CCSequence::createWithTwoActions(animate,removePlane);
		this->getChildByTag(AIRPLANE)->stopAllActions();
		this->getChildByTag(AIRPLANE)->runAction(sequence);
	}
}

void PlaneLayer::RemovePlane()
{
	this->removeChildByTag(AIRPLANE,true);
	//GameOverScene* pScene=GameOverScene::create(score);
	//CCTransitionMoveInT* animateScene=CCTransitionMoveInT::create(0.8f,pScene);
	//CCDirector::sharedDirector()->replaceScene(animateScene);
}
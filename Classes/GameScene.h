#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "GameLayer.h"

class GameScene : public cocos2d::CCScene
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	GameScene(void);  

	~GameScene(void); 

	GameLayer* _gameLayer;

	// implement the "static node()" method manually
	CREATE_FUNC(GameScene);
};

#endif // __HELLOWORLD_SCENE_H__

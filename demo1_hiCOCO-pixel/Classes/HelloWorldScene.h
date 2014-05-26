#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "pixtk.h"
#include "define.h"
#include "player.h"
#include "ground.h"
#include "lava.h"
#include "grass.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
public://touch
	virtual void ccTouchesBegan(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
public://action callback
    void playerShatterFinish();
    void maskPanelFadeShowFinish();
    void restartClickDelayFinish();
public:
    void initGame();
    void cleanGame();
public:
    void update(float dt);
public:
    HelloWorld(){
        m_ground=NULL;
        m_lava=NULL;
        m_grass=NULL;
        m_sheep=NULL;
        m_player=NULL;
        m_isTouchMove=false;
        m_isTouching=false;
        m_isTouchingFoe=false;
        m_uiRoot=NULL;
        m_sceneRoot=NULL;
        m_gameOver=NULL;
        m_maskPanel=NULL;
        m_controlButton_shot=NULL;
        m_controlButton_debug=NULL;
        m_controlButton_restart=NULL;
    }

protected://controlButton
    //shot
    CCControlButton* m_controlButton_shot;
    void controlButtonEvent_shot(CCObject *senderz, CCControlEvent controlEvent);
    //debug
    CCControlButton* m_controlButton_debug;
    void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent);
    //restart
    CCControlButton* m_controlButton_restart;
    void controlButtonEvent_restart(CCObject *senderz, CCControlEvent controlEvent);

protected:
    CCNode*m_uiRoot;
    CCNode*m_sceneRoot;
    Cground*m_ground;
    Clava*m_lava;
    Cgrass*m_grass;
    CdynamicPixelSprite*m_sheep;
    CstaticPixelSprite*m_gameOver;
    CstaticPixelSprite*m_maskPanel;
    Cplayer*m_player;
protected:
    bool m_isTouchMove;
    bool m_isTouching;
    bool m_isTouchingFoe;
    CCPoint m_touchPoint;
    CCPoint m_touchPointFoe;

};

#endif // __HELLOWORLD_SCENE_H__

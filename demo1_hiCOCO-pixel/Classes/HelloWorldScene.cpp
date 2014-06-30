#include "HelloWorldScene.h"
#include "AppMacros.h"
USING_NS_CC;
using namespace cocos2d;
using namespace pixtk;
CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

 /*   /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
*/
    /////////////////////////////
    // 3. add your codes below...
    this->scheduleUpdate();
    //enable touch
	setTouchEnabled( true );
    glClearColor(146.f/255, 207.f/255, 255.f/255, 1.0f);
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    
    //show frame rate info
    CCDirector::sharedDirector()->setDisplayStats(true);
    //set g_showBoundingBox
    g_isShowBoundingBox=g_debugOn;
    
  
    //clean game
    cleanGame();
    //init game
    initGame();

    return true;
}
void HelloWorld::cleanGame(){
    this->removeAllChildrenWithCleanup(true);
}
void HelloWorld::initGame(){
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    //sceneRoot
    m_sceneRoot=new CCNode();
    m_sceneRoot->autorelease();
    m_sceneRoot->init();
    addChild(m_sceneRoot);
    
    //uiRoot
    m_uiRoot=new CCNode();
    m_uiRoot->autorelease();
    m_uiRoot->init();
    addChild(m_uiRoot);
    
    //grass
    m_grass=new Cgrass();
    m_grass->autorelease();
    m_grass->init("grass.png",g_pixelSize);
    m_sceneRoot->addChild(m_grass);
    m_grass->setPosition(ccp(winSize.width/2-9.5*g_pixelSize.width,winSize.height/2-24.5*g_pixelSize.height));
    
    
    //lava
    m_lava=new Clava();
    m_lava->autorelease();
    m_lava->init("lava.png",g_pixelSize);
    m_sceneRoot->addChild(m_lava);
    m_lava->setPosition(ccp(winSize.width/2,winSize.height/2-46.5*g_pixelSize.height));
    
    //ground
    m_ground=new Cground();
    m_ground->autorelease();
    m_ground->init("ground_dynamic.png",g_pixelSize);
    m_sceneRoot->addChild(m_ground);
    m_ground->setPosition(ccp(winSize.width/2-9.5*g_pixelSize.width,winSize.height/2-29.5*g_pixelSize.height));

    //sheep (not used)
    m_sheep=new CdynamicPixelSprite();
    m_sheep->autorelease();
    m_sheep->init("sheep.png",g_pixelSize);
    m_sceneRoot->addChild(m_sheep);
    m_sheep->setPosition(ccp(winSize.width/2,winSize.height/2));
    m_sheep->noise();
    m_sheep->setVisible(false);
    
    //player
    m_player=new Cplayer();
    m_player->autorelease();
    m_player->init("stand.png",m_sceneRoot,g_pixelSize);
    m_sceneRoot->addChild(m_player);
    m_player->setPosition(ccp(winSize.width/2-450,winSize.height/2));
    m_player->setState(eJump);
    m_player->setAcceleration(ccp(0,-g_gravityFactor));
    m_player->setGround(m_ground);
    m_player->setGrass(m_grass);
    m_player->setLava(m_lava);
    
    
    
    //controlButton_shot
    {
        
        CCScale9Sprite* btnUp=CCScale9Sprite::create("button.png");
        CCScale9Sprite* btnDn=CCScale9Sprite::create("button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("shot", "Helvetica", 30);
        CCControlButton* controlButton=CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(winSize.width/2-150,winSize.height/2-150));
        controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&HelloWorld::controlButtonEvent_shot), CCControlEventTouchDown);
        m_uiRoot->addChild(controlButton);
        m_controlButton_shot=controlButton;
    }
    //controlButton_debug
    {
        CCScale9Sprite* btnUp=CCScale9Sprite::create("button.png");
        CCScale9Sprite* btnDn=CCScale9Sprite::create("button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 30);
        CCControlButton* controlButton=CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(200,winSize.height-150));
        controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&HelloWorld::controlButtonEvent_debug), CCControlEventTouchDown);
        m_uiRoot->addChild(controlButton);
        m_controlButton_debug=controlButton;
    }
    
    // instruction
    {
    
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("jump: drag up \r\n shot: press 'shot' button", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-100));
            m_uiRoot->addChild(pLabel);
        }
    
        
    }
    // author info
    {
        CCLabelTTF* pLabel = CCLabelTTF::create("by yang chao (wantnon) 2014-5-4 (last edit 2014-6-30)", "Arial", 30);
        pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - pLabel->getContentSize().height-60));
        m_uiRoot->addChild(pLabel);
    }
    // app title
    {
        // add a label shows "Hello World"
        // create and initialize a label
        
        CCLabelTTF* pLabel = CCLabelTTF::create("hiCOCO-pixel", "Arial", 45);
        
        // position the label on the center of the screen
        pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - pLabel->getContentSize().height));
        
        // add the label as a child to this layer
        m_uiRoot->addChild(pLabel);
    }
    
    //mask panel
    m_maskPanel=new CstaticPixelSprite();
    m_maskPanel->autorelease();
    m_maskPanel->init("whitePixel.png",winSize);
    m_uiRoot->addChild(m_maskPanel);
    m_maskPanel->setPosition(ccp(winSize.width/2,winSize.height/2));
    m_maskPanel->setVisible(false);
    
    //gameOver text
    m_gameOver=new CstaticPixelSprite();
    m_gameOver->autorelease();
    m_gameOver->init("gameOver.png",g_pixelSize);
    m_uiRoot->addChild(m_gameOver);
    m_gameOver->setPosition(ccp(winSize.width/2,winSize.height/2+100));
    m_gameOver->setVisible(false);
    //controlButton_restart
    {
        CstaticPixelSprite*spSprite=new CstaticPixelSprite();
        spSprite->autorelease();
        spSprite->init("restart.png",g_pixelSize);
        CCSpriteFrame*spriteFrame=CCSpriteFrame::createWithTexture(spSprite->getTexture(), spSprite->getTextureRect());
        CCScale9Sprite* btnUp=CCScale9Sprite::createWithSpriteFrame(spriteFrame);
        CCScale9Sprite* btnDn=CCScale9Sprite::createWithSpriteFrame(spriteFrame);
        CCLabelTTF*title=CCLabelTTF::create("", "Helvetica", 30);
        CCControlButton* controlButton=CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
        controlButton->setPreferredSize(spSprite->getContentSize());
        //   controlButton->setPosition(ccp(INFINITY,INFINITY));
        controlButton->setPosition(ccp(winSize.width/2,winSize.height/2-40));
        controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&HelloWorld::controlButtonEvent_restart), CCControlEventTouchDown);
        m_uiRoot->addChild(controlButton);
        m_controlButton_restart=controlButton;
        m_controlButton_restart->setVisible(false);
    }

    
    
}
void HelloWorld::controlButtonEvent_shot(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    m_player->launchAShotBall();
}
void HelloWorld::controlButtonEvent_debug(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    g_debugOn=!g_debugOn;
    g_isShowBoundingBox=g_debugOn;
}
void HelloWorld::controlButtonEvent_restart(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    CCDelayTime*delay=CCDelayTime::create(0.4);
    CCCallFunc*callfunc=CCCallFunc::create(this, callfunc_selector(HelloWorld::restartClickDelayFinish));
    this->runAction(CCSequence::create(delay,callfunc,NULL));
    
}
void HelloWorld::restartClickDelayFinish(){
    cleanGame();
    initGame();
}
void HelloWorld::playerShatterFinish(){
    //hide controlButtons
    m_controlButton_shot->setVisible(false);
    m_controlButton_debug->setVisible(false);
    //mask panel fade show
    m_maskPanel->setVisible(true);
    ccColor3B color={0,0,0};
    m_maskPanel->setColor(color);
    m_maskPanel->setOpacity(0);
    CCFadeTo*fadeTo=CCFadeTo::create(1, 160);
    CCCallFunc*callfunc=CCCallFunc::create(this, callfunc_selector(HelloWorld::maskPanelFadeShowFinish));
    m_maskPanel->runAction(CCSequence::create(fadeTo,callfunc,NULL));
}
void HelloWorld::maskPanelFadeShowFinish(){
    
    m_gameOver->setVisible(true);
    m_controlButton_restart->setVisible(true);
    
}

void HelloWorld::update(float dt){

    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
  
    //----update facingDir
    EPlayerFacingDir oldFacingDir=m_player->getFacingDir();
    EPlayerFacingDir newFacingDir=oldFacingDir;
    if(m_isTouching){
        if(m_touchPoint.x>winSize.width/2){
            newFacingDir=eRight;
        }else{
            newFacingDir=eLeft;
        }
    }
    m_player->setFacingDir(newFacingDir);
  //  cout<<"pos:"<<m_player->getPositionX()<<" "<<m_player->getPositionY()<<endl;
 //   cout<<"facingDir:"<<m_player->getFacingDir()<<endl;
 //   cout<<"m_isTouching"<<m_isTouching<<endl;
 //   cout<<"m_isTouchingFoe"<<m_isTouchingFoe<<endl;
    if(m_player->getFacingDir()==eRight){
        m_player->setScaleX(1);
    }
    if(m_player->getFacingDir()==eLeft){
        m_player->setScaleX(-1);
    }
    
    //----update state, velocity, position
    m_player->updateVelocityAndPosition(dt);
    
    //collision with ground
    CCRect upCollisionRect=m_player->getUpCollisionRectWithGround();
    CCRect dnCollisionRect=m_player->getDnCollisionRectWithGround();
    CCRect leftCollisionRect=m_player->getLeftCollisionRectWithGround();
    CCRect rightCollsionRect=m_player->getRightCollisionRectWithGround();
    
    if(isZeroRect(rightCollsionRect)==false){
        m_player->setVelocity(ccp(MIN(0,m_player->getVelocity().x),m_player->getVelocity().y));
        float targetX=rightCollsionRect.getMinX()-getRectSize(m_player->getBoundingBox()).width/2;
        m_player->setPositionX(m_player->getPositionX()+(targetX-m_player->getPositionX())*0.2);
        
    }
    if(isZeroRect(leftCollisionRect)==false){
        m_player->setVelocity(ccp(MAX(0,m_player->getVelocity().x),m_player->getVelocity().y));
        float targetX=leftCollisionRect.getMaxX()+getRectSize(m_player->getBoundingBox()).width/2;
        m_player->setPositionX(m_player->getPositionX()+(targetX-m_player->getPositionX())*0.2);
    }
    
    if(isZeroRect(upCollisionRect)==false){
        m_player->setVelocity(ccp(m_player->getVelocity().x,MIN(0,m_player->getVelocity().y)));
        float targetY=upCollisionRect.getMinY()-getRectSize(m_player->getBoundingBox()).height/2;
        m_player->setPositionY(m_player->getPositionY()+(targetY-m_player->getPositionY())*0.2);
    }
    
    if(isZeroRect(dnCollisionRect)==false){
        m_player->setVelocity(ccp(m_player->getVelocity().x,MAX(0,m_player->getVelocity().y)));
        float targetY=dnCollisionRect.getMaxY()+getRectSize(m_player->getBoundingBox()).height/2;
        m_player->setPositionY(m_player->getPositionY()+(targetY-m_player->getPositionY())*0.8);
        m_player->setAcceleration(ccp(m_player->getAcceleration().x, MAX(-0.3,m_player->getAcceleration().y)));
    }
    
    
    
    //get ground height below player (for free fall from cliff)
    float groundHeightBelowFoot;

    {
        float rayStartX=(m_player->getCollisionRectByName("dnRect").getMinX()+m_player->getCollisionRectByName("dnRect").getMaxX())/2;
        float rayStartY=m_player->getPositionY();
        float rayLen=10*g_pixelSize.height;
        float rayWidth=m_player->getCollisionRectByName("dnRect").getMaxX()-m_player->getCollisionRectByName("dnRect").getMinX();
        CCPoint foot=m_ground->downDirRayIntersectWithMe(ccp(rayStartX,rayStartY), rayLen, rayWidth);
        groundHeightBelowFoot=foot.y;
    }
    
    //collision with lava
    CCRect dnCollisionRectWithLava=m_player->getDnCollisionRectWithLava();
   
    //
    EPlayerState oldState=m_player->getState();
    if(isZeroRect(dnCollisionRectWithLava)==false){//died
        m_player->setState(eDied);
        //velocity
        m_player->setVelocity(ccp(0,0));
        //acceleration
        m_player->setAcceleration(ccp(0, 0));

        
    }else{//not died
        //run
        if(m_isTouching){
            if(m_player->getState()!=eJump&&m_player->getState()!=eFall){
                //state
                m_player->setState(eRun);
                //velocity
                if(m_player->getFacingDir()==eRight){
                    m_player->setVelocity(ccp(1.6*60,m_player->getVelocity().y));
                }
                if(m_player->getFacingDir()==eLeft){
                    m_player->setVelocity(ccp(-1.6*60,m_player->getVelocity().y));
                }
            }
            
        }
        //stand
        if(m_isTouchingFoe&&m_isTouching==false){
            if(m_player->getState()!=eJump&&m_player->getState()!=eFall){
                //state
                m_player->setState(eStand);
                //velocity
                m_player->setVelocity(ccp(0,m_player->getVelocity().y));
            }
            
        }
        //start jump
        if(m_isTouching&&m_isTouchingFoe){
            if(m_touchPoint.y-m_touchPointFoe.y>5){
                if(m_player->getState()!=eJump&&m_player->getState()!=eFall){
                    //state
                    m_player->setState(eJump);
                    //acceleration
                    m_player->setAcceleration(ccp(m_player->getAcceleration().x, m_player->getAcceleration().y-g_gravityFactor));
                    //velocity
                    m_player->setVelocity(ccp(m_player->getVelocity().x, m_player->getVelocity().y+350));
                    //position
                    m_player->setPositionY(m_player->getPositionY()+MARGIN(5));//move a little up, to avoid by stuck ground
                }
            }
        }
        //in jump
        if(m_isTouching){
            if(m_player->getState()==eJump){
                //velocity
                if(m_player->getFacingDir()==eRight){
                    m_player->setVelocity(ccp(1.6*60,m_player->getVelocity().y));
                }
                if(m_player->getFacingDir()==eLeft){
                    m_player->setVelocity(ccp(-1.6*60,m_player->getVelocity().y));
                }
            }
        }
        //start fall
        if(m_player->getState()!=eFall&&m_player->getState()!=eJump
           &&m_player->getCollisionRectByName("dnRect").getMinY()-groundHeightBelowFoot>0.8*g_pixelSize.height
           ){
            //state
            m_player->setState(eFall);
            //acceleration
            m_player->setAcceleration(ccp(m_player->getAcceleration().x, m_player->getAcceleration().y-g_gravityFactor));
        }
        //in fall
        if(m_isTouching){
            if(m_player->getState()==eFall){
                //velocity
                if(m_player->getFacingDir()==eRight){
                    m_player->setVelocity(ccp(1.6*60,m_player->getVelocity().y));
                }
                if(m_player->getFacingDir()==eLeft){
                    m_player->setVelocity(ccp(-1.6*60,m_player->getVelocity().y));
                }
            }
        }
        
        
        
        //end jump
        if(isZeroRect(dnCollisionRect)==false){
            if(m_player->getState()==eJump){
                //state
                if(m_isTouching){
                    m_player->setState(eRun);
                }else{
                    m_player->setState(eStand);
                }
                //velocity
                m_player->setVelocity(ccp(m_player->getVelocity().x,0));
            }
        }
        
        //end fall
        if(isZeroRect(dnCollisionRect)==false){
            if(m_player->getState()==eFall){
                //state
                if(m_isTouching){
                    m_player->setState(eRun);
                }else{
                    m_player->setState(eStand);
                }
                //velocity
                m_player->setVelocity(ccp(m_player->getVelocity().x,0));
                
                
            }
        }

    }

 
    //cout<<"state:"<<m_player->getState()<<endl;
    //cout<<"old state:"<<oldState<<endl;
    //----update animation
    //update animation only when state is changed
    if(m_player->getState()!=oldState){
        m_player->stopAllActions();//before run new animation, must stop all
        switch (m_player->getState()) {
            case eRun:
                m_player->playRunAnimation();
                break;
            case eStand:
                m_player->playStandAnimation();
                break;
            case eJump:
                m_player->playJumpAnimation();
                break;
            case eFall:
                m_player->playJumpAnimation();
                break;
            case eDied:
            {
                CplayerShatter*shatter=CplayerShatter::create(1.5,80);
                CCCallFunc*callfunc=CCCallFunc::create(this, callfunc_selector(HelloWorld::playerShatterFinish));
                m_player->runAction(CCSequence::create(shatter,callfunc,NULL));
            }
                break;
            default:
                break;
        }
    }

    //camera follow
    CCPoint playerPos=m_player->getPosition();
    CCCamera*camera=m_sceneRoot->getCamera();
    float centerx,centery,centerz;
    float eyex,eyey,eyez;
    float upx,upy,upz;
    camera->getCenterXYZ(&centerx, &centery, &centerz);
    camera->getEyeXYZ(&eyex, &eyey, &eyez);
    camera->getUpXYZ(&upx, &upy, &upz);
    CCPoint wantedEyePos=playerPos-CCPoint(winSize.width/2,winSize.height/2);
    CCPoint newEyePos=CCPoint(eyex,eyey)+ccpMult(wantedEyePos-CCPoint(eyex,eyey),0.2);
    camera->setCenterXYZ(newEyePos.x, newEyePos.y, centerz);
    camera->setEyeXYZ(newEyePos.x, newEyePos.y, eyez);
    camera->setUpXYZ(upx, upy, upz);
    
    
    //lava
    m_lava->wave(dt);
    //shotBall
    m_player->shotBallsMove(dt);
    m_player->shotBallsCollisionWithGround();
    m_player->shotBallsCollisionWithGrass();

}
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}


void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        
        m_touchPointFoe=m_touchPoint;
        m_touchPoint=loc_GLSpace;
        break;
        

    }
    m_isTouchingFoe=m_isTouching;
    m_isTouching=false;
}
void HelloWorld::ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        
        m_touchPointFoe=m_touchPoint;
        m_touchPoint=loc_GLSpace;
        break;
    
        
		
    }
    m_isTouchingFoe=m_isTouching;
    m_isTouching=true;
}

void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSetIterator it;
    CCTouch* touch;
	for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        //CCLOG("loc_GLSpace:%f,%f",loc_GLSpace.x,loc_GLSpace.y);
        
        m_touchPointFoe=m_touchPoint;
        m_touchPoint=loc_GLSpace;
        break;
        
    }
    m_isTouchingFoe=m_isTouching;
    m_isTouching=true;
}


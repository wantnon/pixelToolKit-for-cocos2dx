//
//  player.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-6.
//
//

#ifndef __HelloCpp__player__
#define __HelloCpp__player__

#include <iostream>
#include <vector>
using namespace std;
#include "pixtk.h"
#include "define.h"
#include "ground.h"
#include "shotBall.h"
#include "grass.h"
#include "lava.h"



enum EPlayerState{
    eRun=0,
    eStand,
    eJump,
    eFall,
    eDied,
};
enum EPlayerFacingDir{
    eRight=0,
    eLeft,
};

class Cplayer:public CdynamicPixelActor
{
public:
    Cplayer(){
        m_state=eStand;
        m_facingDir=eRight;
        m_ground=NULL;
        m_layer=NULL;
        m_sceneRoot=NULL;
        m_grass=NULL;
        m_lava=NULL;
    };
    virtual~Cplayer(){
        if(m_ground)m_ground->release();
        if(m_sceneRoot)m_sceneRoot->release();
        if(m_layer)m_layer->release();
        if(m_grass)m_grass->release();
        if(m_lava)m_lava->release();
    }
    bool init(const string&texFileName,CCNode*sceneRoot,CCSize perPixelSize=g_defaultPerPixelSize);
    void playRunAnimation();
    void shatter(float r);
    void shotBallsMove(float dt);
    bool launchAShotBall();
    void playStandAnimation();
    void playJumpAnimation();
    void setState(EPlayerState state){m_state=state;}
    EPlayerState getState()const{return m_state;}
    void setFacingDir(EPlayerFacingDir facingDir){m_facingDir=facingDir;}
    EPlayerFacingDir getFacingDir()const{return m_facingDir;}
    void setVelocity(const CCPoint&velocity){m_velocity=velocity;}
    CCPoint getVelocity()const{return m_velocity;}
    void setAcceleration(const CCPoint&acceleration){m_acceleration=acceleration;}
    CCPoint getAcceleration()const{return m_acceleration;}
    void updateVelocityAndPosition(float dt);
    void setGround(Cground*ground);
    void setGrass(Cgrass*grass);
    void setLava(Clava*lava);
    void setSceneRoot(CCNode*sceneRoot);
    Cground* getdpGround(){return m_ground;}
    CCRect getLeftCollisionRectWithGround();
    CCRect getRightCollisionRectWithGround();
    CCRect getUpCollisionRectWithGround();
    CCRect getDnCollisionRectWithGround();
    CCRect getDnCollisionRectWithLava();
    void shotBallsCollisionWithGround();
    void shotBallsCollisionWithGrass();

protected:
    vector<CfragSprite*> getFragsHitByShotBall(CshotBall_dynamic*shotBall,CdynamicPixelSprite*dpSprite);
protected:
    EPlayerState m_state;
    EPlayerFacingDir m_facingDir;
    CCPoint m_velocity;
    CCPoint m_acceleration;
    Cgrass*m_grass;
    Clava*m_lava;
    Cground*m_ground;
    CCNode*m_sceneRoot;
    CCLayer* m_layer;
    vector<CshotBall_dynamic*> m_shotBallList;
    
};

class CplayerShatter : public CCActionInterval
//ref to CCRotateBy
{
public:
    //creates the action
    static CplayerShatter* create(float fDuration,float r){
        
        CplayerShatter *p = new CplayerShatter();
        p->initWithDuration(fDuration,r);
        p->autorelease();
        
        return p;
    }
    //initializes the action
    bool initWithDuration(float fDuration,float r){
        if (CCActionInterval::initWithDuration(fDuration))
        {
            m_r=r;
            return true;
        }
        
        return false;
    }
    
    
    virtual void startWithTarget(CCNode *pTarget){
        CCActionInterval::startWithTarget(pTarget);
    }
    virtual void update(float time){//the time actually is percentage, namely eclipsedTime/totalDuration
        //the t in action's update(t) and sprite's update(t) is different meaning
        //t in action's update(t) means the progress percentage, see CCActionInterval::step()
        //t in sprite's update(t) means the deltaTime between current frame and previous frame, see CCDirector::drawScene()
        //cout<<"time:"<<time<<endl;
        float progressPercentage=time;
        float curTime=progressPercentage*getDuration();
        ((Cplayer*)m_pTarget)->shatter(m_r);
        
    }
    
    
protected:
    float m_r;
    
};
#endif /* defined(__HelloCpp__player__) */

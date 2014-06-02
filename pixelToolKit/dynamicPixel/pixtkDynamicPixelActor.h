//
//  pixtkDynamicPixelActor.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-5.
//
//

#ifndef __HelloCpp__pixtkDynamicPixelActor__
#define __HelloCpp__pixtkDynamicPixelActor__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "pixtkDynamicPixelModel.h"
namespace_pixtk_begin
class CdynamicPixelAniFrame:public CCObject
{
public:
    CdynamicPixelAniFrame();
    virtual ~CdynamicPixelAniFrame();
    void setdpModel(CdynamicPixelModel*dpModel);
    void setDurTime(float durTime);
    bool init(CdynamicPixelModel*dpModel,float durTime);
    float getDurTime()const{return m_durTime;}
    CdynamicPixelModel*getdpModel()const{return m_dpModel;}
    
protected:
    float m_durTime;
    CdynamicPixelModel* m_dpModel;
};

class CdynamicPixelAniLayer:public CCObject
{
public:
    CdynamicPixelAniLayer(){}
    virtual~CdynamicPixelAniLayer();
    void addAniFrame(CdynamicPixelAniFrame*aniFrame);
    CdynamicPixelAniFrame* getAniFrameByIndex(int index);
    int getAniFrameCount()const{return (int)m_aniFrameList.size();}
    float getAniDurTime()const;
    void setAniName(const string&aniName){m_aniName=aniName;}
    string getAniName()const{return m_aniName;}
protected:
    vector<CdynamicPixelAniFrame*> m_aniFrameList;
    string m_aniName;

};
class CdynamicPixelAniStack:public CCObject
{
public:
    CdynamicPixelAniStack(){};
    virtual~CdynamicPixelAniStack();
    void addAniLayer(CdynamicPixelAniLayer*aniLayer);
    int getAniLayerCount()const{return (int)m_aniLayerList.size();}
    CdynamicPixelAniLayer*getAniLayerByIndex(int index);
    CdynamicPixelAniLayer*getAniLayerByName(const string&aniName);
    int getAniLayerIndexByName(const string&aniName);
protected:
    vector<CdynamicPixelAniLayer*> m_aniLayerList;
};
class CdynamicPixelActor:public CCNode
{
public:
    CdynamicPixelActor();
    virtual~CdynamicPixelActor();
    bool init(const string&texFileName,CCSize perPixelSize=g_defaultPerPixelSize);
    CdynamicPixelAniStack*getAniStack(){return m_aniStack;}
    void setAniStack(CdynamicPixelAniStack*aniStack);
    void setdpModel(CdynamicPixelModel*dpModel);
    CdynamicPixelModel*getdpModel(){return m_dpModel;}
    void aniTo(float t,int aniLayerIndex);
    CdynamicPixelModel* getCurDisplayModel();
    CCRect getBoundingBoxInLocalSpace();
    CCRect getBoundingBox();
    CCRect getCurrentFrameBoundingBoxInLocalSpace();
    CCRect getCurrentFrameBoundingBox();
    CCRect getCollisionRectByIndex(int index){return convertRectToParent(m_dpModel->getCollisionRectByIndex(index));}
    CCRect getCollisionRectByName(const string&name){return convertRectToParent(m_dpModel->getCollisionRectByName(name));}
    CCRect getCurrentFrameCollisionRectByIndex(int index);
    CCRect getCurrentFrameCollisionRectByName(const string&name);
    CCRect convertRectToParent(const CCRect&rectInLocalSpace) {return CCRectApplyAffineTransform(rectInLocalSpace, nodeToParentTransform());}
protected:
    void init_dft();
protected:
    CCNode*m_displayNode;
    CdynamicPixelAniStack*m_aniStack;
    CdynamicPixelModel*m_dpModel;
};

class CdynamicPixelActorAnimate : public CCActionInterval
//ref to CCRotateBy
{
public:
    //creates the action
    static CdynamicPixelActorAnimate* create(float fDuration,int aniLayerIndex);
    //initializes the action
    bool initWithDuration(float fDuration,int aniLayerIndex);
    
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
protected:
    int m_aniLayerIndex;

};
namespace_pixtk_end
#endif /* defined(__HelloCpp__pixtkDynamicPixelActor__) */

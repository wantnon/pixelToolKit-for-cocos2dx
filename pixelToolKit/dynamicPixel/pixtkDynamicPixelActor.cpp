//
//  pixtkDynamicPixelActor.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-5.
//
//

#include "pixtkDynamicPixelActor.h"
namespace_pixtk_begin
CdynamicPixelAniFrame::CdynamicPixelAniFrame(){
    m_durTime=0;
    m_dpModel=NULL;
};
CdynamicPixelAniFrame::~CdynamicPixelAniFrame(){
    if(m_dpModel)m_dpModel->release();
};
void CdynamicPixelAniFrame::setdpModel(CdynamicPixelModel*dpModel){
    assert(dpModel);
    if(m_dpModel==NULL){
        m_dpModel=dpModel;
        m_dpModel->retain();
    }else{
        m_dpModel->release();
        m_dpModel=dpModel;
        m_dpModel->retain();
    }
}
void CdynamicPixelAniFrame::setDurTime(float durTime){
    m_durTime=durTime;
}
bool CdynamicPixelAniFrame::init(CdynamicPixelModel*dpModel,float durTime){
    setdpModel(dpModel);
    setDurTime(durTime);
    return true;
}
//-----------------------------
CdynamicPixelAniLayer::~CdynamicPixelAniLayer(){
    int nAniFrame=(int)m_aniFrameList.size();
    for(int i=0;i<nAniFrame;i++){
        m_aniFrameList[i]->release();
    }
}
void CdynamicPixelAniLayer::addAniFrame(CdynamicPixelAniFrame*aniFrame){
    assert(aniFrame);
    m_aniFrameList.push_back(aniFrame);
    aniFrame->retain();
    
}
CdynamicPixelAniFrame* CdynamicPixelAniLayer::getAniFrameByIndex(int index){
    assert(index>=0&&index<(int)m_aniFrameList.size());
    return m_aniFrameList[index];
}

float CdynamicPixelAniLayer::getAniDurTime()const{
    int nAniFrame=(int)m_aniFrameList.size();
    float timeSum=0;
    for(int i=0;i<nAniFrame;i++){
        timeSum+=m_aniFrameList[i]->getDurTime();
    }
    return timeSum;
}
//---------------------------------
CdynamicPixelAniStack::~CdynamicPixelAniStack(){
    int nAniLayer=(int)m_aniLayerList.size();
    for(int i=0;i<nAniLayer;i++){
        m_aniLayerList[i]->release();
    }
};
void CdynamicPixelAniStack::addAniLayer(CdynamicPixelAniLayer*aniLayer){
    assert(aniLayer);
    m_aniLayerList.push_back(aniLayer);
    aniLayer->retain();
}
CdynamicPixelAniLayer*CdynamicPixelAniStack::getAniLayerByIndex(int index){
    assert(index>=0&&index<(int)m_aniLayerList.size());
    return m_aniLayerList[index];
}
CdynamicPixelAniLayer*CdynamicPixelAniStack::getAniLayerByName(const string&aniName){
    int aniLayerIndex=getAniLayerIndexByName(aniName);
    if(aniLayerIndex==-1){
        return NULL;
    }else{
        return m_aniLayerList[aniLayerIndex];
    }
}
int CdynamicPixelAniStack::getAniLayerIndexByName(const string&aniName){
    int nAniLayer=(int)m_aniLayerList.size();
    for(int i=0;i<nAniLayer;i++){
        if(aniName==m_aniLayerList[i]->getAniName()){
            return i;
        }
    }
    return -1;
}
//-------------------------------------
CdynamicPixelActor::CdynamicPixelActor(){
    m_displayNode=NULL;
    m_aniStack=NULL;
    m_dpModel=NULL;
    init_dft();
}
CdynamicPixelActor::~CdynamicPixelActor(){
    if(m_aniStack)m_aniStack->release();
    if(m_dpModel)m_dpModel->release();
    
}
bool CdynamicPixelActor::init(const string&texFileName,CCSize perPixelSize){
    CdynamicPixelSprite *dpSprite=new CdynamicPixelSprite();
    dpSprite->autorelease();
    dpSprite->init(texFileName,perPixelSize);
    m_dpModel->adddpSprite(dpSprite);
    return true;
}
void CdynamicPixelActor::setAniStack(CdynamicPixelAniStack*aniStack){
    assert(aniStack);
    if(m_aniStack==NULL){
        m_aniStack=aniStack;
        m_aniStack->retain();
    }else{
        m_aniStack->release();
        m_aniStack=aniStack;
        m_aniStack->retain();
    }
}
void CdynamicPixelActor::setdpModel(CdynamicPixelModel*dpModel){
    assert(dpModel);
    if(m_dpModel==NULL){
        m_dpModel=dpModel;
        m_dpModel->retain();
    }else{
        m_dpModel->release();
        m_dpModel=dpModel;
        m_dpModel->retain();
    }
}
void CdynamicPixelActor::aniTo(float t,int aniLayerIndex){//run animation to time t
    CdynamicPixelAniLayer*aniLayer=getAniStack()->getAniLayerByIndex(aniLayerIndex);
    float aniDurTime=aniLayer->getAniDurTime();
    t=t-floor(t/aniDurTime)*aniDurTime;//mod
    //calculate which frame to show at time t
    int frameIndex=0;
    float timeSum=0;
    while(1){
        //add current frameIndex's durTime to timeSum
        timeSum+=aniLayer->getAniFrameByIndex(frameIndex)->getDurTime();
        //if timeSum surpass t, then current frameIndex is the frame to show
        if(timeSum>t){
            break;
        }
        //next frame
        frameIndex++;
        if(frameIndex>=aniLayer->getAniFrameCount()){
            frameIndex=aniLayer->getAniFrameCount();
            break;
        }
    }//frameIndex is the frame to show
    //show the frame
    CdynamicPixelModel*dpModel=aniLayer->getAniFrameByIndex(frameIndex)->getdpModel();
    m_displayNode->removeAllChildrenWithCleanup(true);
    m_displayNode->addChild(dpModel);
    
    
}
CdynamicPixelModel* CdynamicPixelActor::getCurDisplayModel(){
    CCArray*children=m_displayNode->getChildren();
    int nChild=children->count();
    assert(nChild==1);
    return (CdynamicPixelModel*)children->objectAtIndex(0);
}

CCRect CdynamicPixelActor::getBoundingBoxInLocalSpace(){
    return m_dpModel->getBoundingBox();
}

CCRect CdynamicPixelActor::getBoundingBox(){
    return CCRectApplyAffineTransform(getBoundingBoxInLocalSpace(), nodeToParentTransform());
}
CCRect CdynamicPixelActor::getCurrentFrameBoundingBoxInLocalSpace(){
    CdynamicPixelModel*curModel=getCurDisplayModel();
    assert(curModel);
    return curModel->getBoundingBox();
}
CCRect CdynamicPixelActor::getCurrentFrameBoundingBox(){
    return CCRectApplyAffineTransform(getCurrentFrameBoundingBoxInLocalSpace(), nodeToParentTransform());
}
void CdynamicPixelActor::addCollisionRectInLocalSpace(const CcollisionRect&localSpaceCollisionRect){
    m_collisionRectListInLocalSpace.push_back(localSpaceCollisionRect);
}
CCRect CdynamicPixelActor::getCollisionRectByIndex(int index) {
    assert(index>=0&&index<(int)m_collisionRectListInLocalSpace.size());
    return CCRectApplyAffineTransform(m_collisionRectListInLocalSpace[index].getRect(), nodeToParentTransform());
}
int CdynamicPixelActor::getCollisionRectIndexByName(const string&name){
    int nRect=(int)m_collisionRectListInLocalSpace.size();
    int index=-1;
    for(int i=0;i<nRect;i++){
        if(m_collisionRectListInLocalSpace[i].getName()==name){
            index=i;
            break;
        }
    }//got index
    return index;
}
CCRect CdynamicPixelActor::getCollisionRectByName(const string&name){
    int index=getCollisionRectIndexByName(name);
    if(index==-1){
        cout<<"error: not find!"<<endl;
        assert(false);
    }else{
        return getCollisionRectByIndex(index);
    }
}
void CdynamicPixelActor::draw(){
    if(g_isShowBoundingBox){
        int nCollisionRect=(int)m_collisionRectListInLocalSpace.size();
        for(int i=0;i<nCollisionRect;i++){
            CCRect rect=m_collisionRectListInLocalSpace[i].getRect();
            // v3 ---- v2
            //    |            |
            // v0 ---- v1
            CCPoint v0=CCPoint(rect.getMinX(),rect.getMinY());
            CCPoint v1=CCPoint(rect.getMaxX(),rect.getMinY());
            CCPoint v2=CCPoint(rect.getMaxX(),rect.getMaxY());
            CCPoint v3=CCPoint(rect.getMinX(),rect.getMaxY());
            ccDrawLine(v0, v1);
            ccDrawLine(v1, v2);
            ccDrawLine(v2, v3);
            ccDrawLine(v3, v0);
        }
    }
}
void CdynamicPixelActor::init_dft(){
    //m_displayNode
    m_displayNode=CCNode::create();
    this->addChild(m_displayNode,1);
    //m_dpModel
    CdynamicPixelModel*dpModel=new CdynamicPixelModel();
    dpModel->autorelease();
    setdpModel(dpModel);
    m_displayNode->addChild(dpModel);//use dpModel as default displayNode
    
}
//----------------------------
//creates the action
CdynamicPixelActorAnimate* CdynamicPixelActorAnimate::create(float fDuration,int aniLayerIndex){
    
    CdynamicPixelActorAnimate *p = new CdynamicPixelActorAnimate();
    p->initWithDuration(fDuration,aniLayerIndex);
    p->autorelease();
    
    return p;
}
//initializes the action
bool CdynamicPixelActorAnimate::initWithDuration(float fDuration,int aniLayerIndex){
    if (CCActionInterval::initWithDuration(fDuration))
    {
        m_aniLayerIndex=aniLayerIndex;
        return true;
    }
    
    return false;
}


void CdynamicPixelActorAnimate::startWithTarget(CCNode *pTarget){
    CCActionInterval::startWithTarget(pTarget);
}
void CdynamicPixelActorAnimate::update(float time){//the time actually is percentage, namely eclipsedTime/totalDuration
    //the t in action's update(t) and sprite's update(t) is different meaning
    //t in action's update(t) means the progress percentage, see CCActionInterval::step()
    //t in sprite's update(t) means the deltaTime between current frame and previous frame, see CCDirector::drawScene()
    //cout<<"time:"<<time<<endl;
    float progressPercentage=time;
    float curTime=progressPercentage*getDuration();
    ((CdynamicPixelActor*)m_pTarget)->aniTo(curTime,m_aniLayerIndex);
    
}


namespace_pixtk_end

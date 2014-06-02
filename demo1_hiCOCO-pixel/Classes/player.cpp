//
//  player.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-6.
//
//

#include "player.h"

bool Cplayer::init(const string&texFileName,CCNode*sceneRoot,CCSize perPixelSize){
    CdynamicPixelActor::init(texFileName,perPixelSize);
    setSceneRoot(sceneRoot);
    //aniStack
    CdynamicPixelAniStack*aniStack=new CdynamicPixelAniStack();
    aniStack->autorelease();
    //aniLayers
    {
        //----stand
        {
            CdynamicPixelAniLayer*aniLayer=new CdynamicPixelAniLayer();
            aniLayer->autorelease();
            vector<string> picNameList;
            picNameList.push_back("stand.png");
            for(int i=0;i<(int)picNameList.size();i++){
                string picName=picNameList[i];
                //dpSprite
                CdynamicPixelSprite*dpSprite=new CdynamicPixelSprite();
                dpSprite->autorelease();
                dpSprite->init(picName,g_pixelSize);
                //dpModel
                CdynamicPixelModel*dpModel=new CdynamicPixelModel();
                dpModel->autorelease();
                dpModel->adddpSprite(dpSprite);
                //dpAniFrame
                CdynamicPixelAniFrame*dpAniFrame=new CdynamicPixelAniFrame();
                dpAniFrame->autorelease();
                dpAniFrame->init(dpModel, 0.12);
                aniLayer->addAniFrame(dpAniFrame);
                aniLayer->setAniName("stand");
            }
            aniStack->addAniLayer(aniLayer);
        }
        
        //----run
        {
            CdynamicPixelAniLayer*aniLayer=new CdynamicPixelAniLayer();
            aniLayer->autorelease();
            vector<string> picNameList;
            picNameList.push_back("run_1.png");
            picNameList.push_back("run_2.png");
            for(int i=0;i<(int)picNameList.size();i++){
                string picName=picNameList[i];
                //dpSprite
                CdynamicPixelSprite*dpSprite=new CdynamicPixelSprite();
                dpSprite->autorelease();
                dpSprite->init(picName,g_pixelSize);
                //dpModel
                CdynamicPixelModel*dpModel=new CdynamicPixelModel();
                dpModel->autorelease();
                dpModel->adddpSprite(dpSprite);
                //dpAniFrame
                CdynamicPixelAniFrame*dpAniFrame=new CdynamicPixelAniFrame();
                dpAniFrame->autorelease();
                dpAniFrame->init(dpModel, 0.12);
                aniLayer->addAniFrame(dpAniFrame);
                aniLayer->setAniName("run");
            }
            aniStack->addAniLayer(aniLayer);
        }
        //----jump
        {
            CdynamicPixelAniLayer*aniLayer=new CdynamicPixelAniLayer();
            aniLayer->autorelease();
            vector<string> picNameList;
            picNameList.push_back("jump.png");
            for(int i=0;i<(int)picNameList.size();i++){
                string picName=picNameList[i];
                //dpSprite
                CdynamicPixelSprite*dpSprite=new CdynamicPixelSprite();
                dpSprite->autorelease();
                dpSprite->init(picName,g_pixelSize);
                //dpModel
                CdynamicPixelModel*dpModel=new CdynamicPixelModel();
                dpModel->autorelease();
                dpModel->adddpSprite(dpSprite);
                //dpAniFrame
                CdynamicPixelAniFrame*dpAniFrame=new CdynamicPixelAniFrame();
                dpAniFrame->autorelease();
                dpAniFrame->init(dpModel, 0.12);
                aniLayer->addAniFrame(dpAniFrame);
                aniLayer->setAniName("jump");
            }
            aniStack->addAniLayer(aniLayer);
        }
    }
    //attach
    this->setAniStack(aniStack);
    //collision rect
    //  --C--
    //  |   |
    //  C   C
    //  |   |
    //  --C--
    float width=getRectSize(this->getBoundingBox()).width;
    float height=getRectSize(this->getBoundingBox()).height;

    float margin=2;
    CcollisionRect leftRect=CcollisionRect(CCRect(margin-width/2,margin-height/2+height/3,width/3-2*margin,height/3-2*margin),"leftRect");
    CcollisionRect rightRect=CcollisionRect(CCRect(margin-width/2+width/3*2,margin-height/2+height/3,width/3-2*margin,height/3-2*margin),"rightRect");
    CcollisionRect dnRect=CcollisionRect(CCRect(margin-width/2+width/3,margin-height/2,width/3-2*margin,height/3-2*margin),"dnRect");
    CcollisionRect upRect=CcollisionRect(CCRect(margin-width/2+width/3,margin-height/2+height/3*2,width/3-2*margin,height/3-2*margin),"upRect");
    
    this->getdpModel()->addCollisionRectInLocalSpace(leftRect);
    this->getdpModel()->addCollisionRectInLocalSpace(rightRect);
    this->getdpModel()->addCollisionRectInLocalSpace(dnRect);
    this->getdpModel()->addCollisionRectInLocalSpace(upRect);

    //shotBallList
    const int nShotBall=20;
    for(int i=0;i<nShotBall;i++){
        CshotBall_dynamic*shotBall=new CshotBall_dynamic();
        shotBall->autorelease();
        shotBall->init("shotBall.png",g_pixelSize);
        shotBall->setVisible(false);
        m_sceneRoot->addChild(shotBall);
        m_shotBallList.push_back(shotBall);
        
    }
    

    return true;

}
void Cplayer::playRunAnimation(){
    float aniLoopDurTime=this->getAniStack()->getAniLayerByName("run")->getAniDurTime();
    CdynamicPixelActorAnimate*ani=CdynamicPixelActorAnimate::create(aniLoopDurTime,this->getAniStack()->getAniLayerIndexByName("run"));
    this->runAction(CCRepeatForever::create(ani));
}
void Cplayer::shatter(float r){
    //all frags move toward r
    assert(this->getCurDisplayModel()->getdpSpriteCount()>0);
    CdynamicPixelSprite* dpSprite=this->getCurDisplayModel()->getdpSpriteByIndex(0);
    CCPoint center=dpSprite->getPosition();
    int nRow=this->getCurDisplayModel()->getdpSpriteByIndex(0)->getFragMatRowCount();
    int nCol=this->getCurDisplayModel()->getdpSpriteByIndex(0)->getFragMatColCount();
    for(int i=0;i<nRow;i++){
        for(int j=0;j<nCol;j++){
            CfragSprite*frag=this->getCurDisplayModel()->getdpSpriteByIndex(0)->getFragByIndex(i, j);
            if(frag==NULL)continue;
            CCPoint fragPos=frag->getPosition();
            CCPoint dir=ccpNormalize(fragPos-center);
            float disToCenter=ccpLength(center-fragPos);
            float disToEdge=r-disToCenter;
            float disToEdgeWithRandom=disToEdge+rand()%(int)r-r/2;//add random to disToEdge
            float movLen=disToEdgeWithRandom*0.04;
            CCPoint movVec=ccpMult(dir, movLen);
            CCPoint newFragPos=fragPos+movVec;
            frag->setPosition(newFragPos);
            frag->setOpacity(MAX(0,255*disToEdge/r));
            frag->setScale(MAX(0, 2.5*disToEdge/r));
        }
    }
    
    
}
void Cplayer::shotBallsMove(float dt){
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    int nShotBall=(int)m_shotBallList.size();
    for(int i=0;i<nShotBall;i++){
        CshotBall_dynamic*shotBall=m_shotBallList[i];
        if(shotBall->getIsLaunched()){
            float sig=(shotBall->getIsToRight()?1:-1);
            shotBall->setPositionX(shotBall->getPositionX()+sig*7*60*dt);
            //if shotBall out of visible area, then turn it to unlaunched
            CCCamera*camera=m_sceneRoot->getCamera();
            float centerx,centery,centerz;
            float eyex,eyey,eyez;
            float upx,upy,upz;
            camera->getCenterXYZ(&centerx, &centery, &centerz);
            camera->getEyeXYZ(&eyex, &eyey, &eyez);
            camera->getUpXYZ(&upx, &upy, &upz);
            CCRect visibleRect=CCRect(eyex,eyey,winSize.width,winSize.height);
            if(visibleRect.intersectsRect(shotBall->getBoundingBox())==false){
                shotBall->setIsLaunched(false);
                shotBall->setVisible(false);
            }
        }
    }
}
bool Cplayer::launchAShotBall(){
    //find a shotBall which is not launched, turn it to launched
    //if can not find one, then return false
    int nShotBall=(int)m_shotBallList.size();
    for(int i=0;i<nShotBall;i++){
        CshotBall_dynamic*shotBall=m_shotBallList[i];
        if(shotBall->getIsLaunched()==false){
            shotBall->setIsLaunched(true);
            shotBall->setVisible(true);
            shotBall->setPosition(this->getPosition());
            shotBall->setIsToRight(this->getScaleX()>0);
            shotBall->setScaleX(shotBall->getIsToRight()?1:-1);
            return true;
        }
    }
    return false;
}
void Cplayer::playStandAnimation(){
    float aniLoopDurTime=this->getAniStack()->getAniLayerByName("stand")->getAniDurTime();
    CdynamicPixelActorAnimate*ani=CdynamicPixelActorAnimate::create(aniLoopDurTime,this->getAniStack()->getAniLayerIndexByName("stand"));
    this->runAction(CCRepeatForever::create(ani));
}

void Cplayer::playJumpAnimation(){
    float aniLoopDurTime=this->getAniStack()->getAniLayerByName("jump")->getAniDurTime();
    CdynamicPixelActorAnimate*ani=CdynamicPixelActorAnimate::create(aniLoopDurTime,this->getAniStack()->getAniLayerIndexByName("jump"));
    this->runAction(CCRepeatForever::create(ani));
}

void Cplayer::updateVelocityAndPosition(float dt){
    //update velocity
    m_velocity=m_velocity+ccpMult(m_acceleration,dt);
    //update position
    CCPoint oldPos=this->getPosition();
    CCPoint newPos=oldPos+ccpMult(m_velocity,dt);
    this->setPosition(newPos);
}

void Cplayer::setGround(Cground*ground){
    assert(ground);
    if(m_ground==NULL){
        m_ground=ground;
        m_ground->retain();
    }else{
        m_ground->release();
        m_ground=ground;
        m_ground->retain();
    }
}
void Cplayer::setGrass(Cgrass*grass){
    assert(grass);
    if(m_grass==NULL){
        m_grass=grass;
        m_grass->retain();
    }else{
        m_grass->release();
        m_grass=grass;
        m_grass->retain();
    }
}
void Cplayer::setLava(Clava*lava){
    assert(lava);
    if(m_lava==NULL){
        m_lava=lava;
        m_lava->retain();
    }else{
        m_lava->release();
        m_lava=lava;
        m_lava->retain();
    }
}
void Cplayer::setSceneRoot(CCNode*sceneRoot){
    assert(sceneRoot);
    if(m_sceneRoot==NULL){
        m_sceneRoot=sceneRoot;
        m_sceneRoot->retain();
    }else{
        m_sceneRoot->release();
        m_sceneRoot=sceneRoot;
        m_sceneRoot->retain();
    }
}

CCRect Cplayer::getLeftCollisionRectWithGround(){
    CCRect leftRect_parentSpace=this->getCollisionRectByName((getScaleX()>0)?"leftRect":"rightRect");
    vector<CfragSprite*> collisionFrags_left=m_ground->getFragsInRect(leftRect_parentSpace);
    //calculate the min rect contain all the collisionFrags
    CCRect minRect=m_ground->getMinRectForFrags(collisionFrags_left);
    return minRect;
}
CCRect Cplayer::getRightCollisionRectWithGround(){
    CCRect rightRect_parentSpace=this->getCollisionRectByName((getScaleX()>0)?"rightRect":"leftRect");
    vector<CfragSprite*> collisionFrags_right=m_ground->getFragsInRect(rightRect_parentSpace);
    //calculate the min rect contain all the collisionFrags
    CCRect minRect=m_ground->getMinRectForFrags(collisionFrags_right);
    return minRect;
}
CCRect Cplayer::getUpCollisionRectWithGround(){
    CCRect upRect_parentSpace=this->getCollisionRectByName("upRect");
    vector<CfragSprite*> collisionFrags_up=m_ground->getFragsInRect(upRect_parentSpace);
    //calculate the min rect contain all the collisionFrags
    CCRect minRect=m_ground->getMinRectForFrags(collisionFrags_up);
    return minRect;
}
CCRect Cplayer::getDnCollisionRectWithGround(){
    CCRect dnRect_parentSpace=this->getCollisionRectByName("dnRect");
    vector<CfragSprite*> collisionFrags_dn=m_ground->getFragsInRect(dnRect_parentSpace);
    // debug:
    // int nFrag=(int)collisionFrags_dn.size();
    // for(int i=0;i<nFrag;i++){
    // CfragSprite*frag=collisionFrags_dn[i];
    // ccColor3B color={255,0,0};
    // frag->setColor(color);
    // }
    
    //calculate the min rect contain all the collisionFrags
    CCRect minRect=m_ground->getMinRectForFrags(collisionFrags_dn);
    return minRect;
}

CCRect Cplayer::getDnCollisionRectWithLava(){
    CCRect dnRect_parentSpace=this->getCollisionRectByName("dnRect");
    vector<CfragSprite*> collisionFrags_dn=m_lava->getFragsInRect(dnRect_parentSpace);
    
    //calculate the min rect contain all the collisionFrags
    CCRect minRect=m_lava->getMinRectForFrags(collisionFrags_dn);
    return minRect;
}

void Cplayer::shotBallsCollisionWithGround(){
    assert(m_ground);
    int nShotBall=(int)m_shotBallList.size();
    for(int i=0;i<nShotBall;i++){
        CshotBall_dynamic*shotBall=m_shotBallList[i];
        if(shotBall->getIsLaunched()==false)continue;
        vector<CfragSprite*> collisionFrags=getFragsHitByShotBall(shotBall, m_ground);
        int nCollisionFrag=(int)collisionFrags.size();
        if(nCollisionFrag){
            for(int k=0;k<nCollisionFrag;k++){
                CfragSprite*frag=collisionFrags[k];
                //not only frag will be killed, his neighbours may be killed too.
                Cij IJ=frag->getIJ();
                for(int i=IJ.getI()-MARGIN(3);i<=IJ.getI()+MARGIN(3);i++){
                    for(int j=IJ.getJ()-MARGIN(3);j<=IJ.getJ()+MARGIN(3);j++){
                        if(i==IJ.getI()&&j==IJ.getJ()){
                            if(m_ground->getIsHasFrag(i, j)){
                                m_ground->killFrag(i, j);
                            }
                        }else{
                            bool isKill=rand()%2;
                            if(isKill){
                                if(m_ground->getIsHasFrag(i, j)){
                                    m_ground->killFrag(i, j);
                                }
                            }
                        }
                        
                    }
                }
                //shotBall disappear
                shotBall->setIsLaunched(false);
                shotBall->setVisible(false);
            }
        }
    }
}
void Cplayer::shotBallsCollisionWithGrass(){
    assert(m_grass);
    int nShotBall=(int)m_shotBallList.size();
    for(int i=0;i<nShotBall;i++){
        CshotBall_dynamic*shotBall=m_shotBallList[i];
        if(shotBall->getIsLaunched()==false)continue;
        vector<CfragSprite*> collisionFrags=getFragsHitByShotBall(shotBall, m_grass);
        int nCollisionFrag=(int)collisionFrags.size();
        if(nCollisionFrag){
            for(int k=0;k<nCollisionFrag;k++){
                CfragSprite*frag=collisionFrags[k];
                //not only frag will be killed, his neighbours may be killed too.
                Cij IJ=frag->getIJ();
                for(int i=IJ.getI()-MARGIN(3);i<=IJ.getI()+MARGIN(3);i++){
                    for(int j=IJ.getJ()-MARGIN(3);j<=IJ.getJ()+MARGIN(3);j++){
                        if(i==IJ.getI()&&j==IJ.getJ()){
                            if(m_grass->getIsHasFrag(i, j)){
                                m_grass->killFrag(i, j);
                            }
                        }else{
                            bool isKill=rand()%2;
                            if(isKill){
                                if(m_grass->getIsHasFrag(i, j)){
                                    m_grass->killFrag(i, j);
                                }
                            }
                        }
                        
                    }
                }
                //shotBall disappear
                shotBall->setIsLaunched(false);
                shotBall->setVisible(false);
            }
            
        }
    }
}
vector<CfragSprite*> Cplayer::getFragsHitByShotBall(CshotBall_dynamic*shotBall,CdynamicPixelSprite*dpSprite){
    CCRect shotBallRect=shotBall->getBoundingBox();
    vector<CfragSprite*> collisionFrags;
    {
        vector<CfragSprite*> t_collisionFrags=dpSprite->getFragsInRect(shotBallRect);
        //for each frag in collisionFrags, see whether it really collision with shotBall
        int nFrag=(int)t_collisionFrags.size();
        for(int i=0;i<nFrag;i++){
            CfragSprite*frag=t_collisionFrags[i];
            //see if frag collision with shotBall
            vector<CfragSprite*> tt_collisionFrags=shotBall->getFragsInRect(dpSprite->getFragRect(frag));
            if((int)tt_collisionFrags.size()!=0){
                collisionFrags.push_back(frag);
            }
        }//got collisionFrags
    }
    return collisionFrags;
}

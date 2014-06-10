//
//  pixtkDynamicPixelModel.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-6.
//
//

#include "pixtkDynamicPixelModel.h"
namespace_pixtk_begin
void CdynamicPixelModel::adddpSprite(CdynamicPixelSprite*dpSprite){
    assert(dpSprite);
    m_dpSpriteList.push_back(dpSprite);
    addChild(dpSprite);
}
CdynamicPixelSprite*CdynamicPixelModel::getdpSpriteByIndex(int index){
    assert(index>=0&&index<(int)m_dpSpriteList.size());
    return m_dpSpriteList[index];
}
void CdynamicPixelModel::addCollisionRectInLocalSpace(const CcollisionRect&localSpaceCollisionRect){
    m_collisionRectListInLocalSpace.push_back(localSpaceCollisionRect);
}
CCRect CdynamicPixelModel::getCollisionRectByIndex(int index) {
    assert(index>=0&&index<(int)m_collisionRectListInLocalSpace.size());
    return CCRectApplyAffineTransform(m_collisionRectListInLocalSpace[index].getRect(), nodeToParentTransform());
}
int CdynamicPixelModel::getCollisionRectIndexByName(const string&name){
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
CCRect CdynamicPixelModel::getCollisionRectByName(const string&name){
    int index=getCollisionRectIndexByName(name);
    if(index==-1){
        cout<<"error: not find!"<<endl;
        assert(false);
    }else{
        return getCollisionRectByIndex(index);
    }
}
void CdynamicPixelModel::draw(){
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

CCRect CdynamicPixelModel::getBoundingBoxInLocalSpace()const {
    // CCRectApplyAffineTransform
    //calculate the union of all boundingBoxInParentSpace of child dpSprites
    float xmin=pixINF;
    float xmax=-pixINF;
    float ymin=pixINF;
    float ymax=-pixINF;
    int ndpSprite=(int)m_dpSpriteList.size();
    if(ndpSprite==0){
        return CCRect(0,0,0,0);
    }
    for(int i=0;i<ndpSprite;i++){
        CdynamicPixelSprite*dpSprite=m_dpSpriteList[i];
        CCRect rect=dpSprite->getBoundingBox();
        if(rect.getMinX()<xmin)xmin=rect.getMinX();
        if(rect.getMaxX()>xmax)xmax=rect.getMaxX();
        if(rect.getMinY()<ymin)ymin=rect.getMinY();
        if(rect.getMaxY()>ymax)ymax=rect.getMaxY();
    }//got xmin,max,ymin,ymax
    return CCRect(xmin,ymin,xmax-xmin,ymax-ymin);
}
CCRect CdynamicPixelModel::getBoundingBox() {
    return CCRectApplyAffineTransform(getBoundingBoxInLocalSpace(), nodeToParentTransform());
}
namespace_pixtk_end
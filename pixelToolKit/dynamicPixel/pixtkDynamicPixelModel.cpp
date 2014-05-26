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
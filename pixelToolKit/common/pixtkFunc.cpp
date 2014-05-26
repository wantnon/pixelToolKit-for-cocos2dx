//
//  pixtkFunc.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-3-6.
//
//

#include "pixtkFunc.h"
namespace_pixtk_begin
string numberToStr(int n){//number to string
    char numbuf[128];
    sprintf(numbuf, "%i", n);//there is no itoa on iOS
    string numStr=numbuf;
    return numStr;
}

CCTexture2D* scaleTexUsingRTT(CCTexture2D*texture,float scaleX,float scaleY){
    CCTexture2D*newTex=NULL;
    CCSprite*tSprite=CCSprite::createWithTexture(texture);
    //render to texture
    CCRenderTexture*renderTex=CCRenderTexture::create(tSprite->getContentSize().width*scaleX, tSprite->getContentSize().height*scaleY);
    renderTex->begin();
    tSprite->setAnchorPoint(ccp(0,1));//shit
    tSprite->setScaleY(-scaleY);//shit
    tSprite->setScaleX(scaleX);
    tSprite->visit();
    renderTex->end();
    newTex=renderTex->getSprite()->getTexture();
    return newTex;
}

CCSize getRectSize(const CCRect&rect){
    CCSize size=CCSize(rect.getMaxX()-rect.getMinX(),rect.getMaxY()-rect.getMinY());
    return size;
}
namespace_pixtk_end

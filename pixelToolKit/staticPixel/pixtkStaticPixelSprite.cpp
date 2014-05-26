//
//  pixtkStaticPixelSprite.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-4.
//
//

#include "pixtkStaticPixelSprite.h"
namespace_pixtk_begin
bool CstaticPixelSprite::init(const string&texFileName,CCSize perPixelSize){
    m_perPixelSize=perPixelSize;
    //resize the texture using render to texture (so we can avoid initial scaling)
    CCTexture2D*tex=CCTextureCache::sharedTextureCache()->addImage(texFileName.c_str());
    //set texture filter to GL_NEAREST to achieve sharp edge
    ccTexParams texParams={GL_NEAREST,GL_NEAREST,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE};
    tex->setTexParameters(&texParams);
    CCTexture2D*newTex=scaleTexUsingRTT(tex,m_perPixelSize.width,m_perPixelSize.height);
    //note: no need to set newTex's filter to GL_NEAREST
    //use newTex to initialize this sprite
    CCSprite::initWithTexture(newTex);
    return true;
}
namespace_pixtk_end
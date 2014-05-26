//
//  pixtkStaticPixelSprite.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-4.
//
//

#ifndef __HelloCpp__pixtkStaticPixelSprite__
#define __HelloCpp__pixtkStaticPixelSprite__

#include <iostream>
#include <vector>
#include <string>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "pixtkFunc.h"
namespace_pixtk_begin
class CstaticPixelSprite:public CCSprite
{
public:
    CstaticPixelSprite(){}
    virtual~CstaticPixelSprite(){}
    bool init(const string&texFileName,CCSize perPixelSize=CCSize(10,10));
protected:
    CCSize m_perPixelSize;
};
namespace_pixtk_end
#endif /* defined(__HelloCpp__pixtkStaticPixelSprite__) */

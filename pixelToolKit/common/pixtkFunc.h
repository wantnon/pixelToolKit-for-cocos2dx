//
//  pixtkFunc.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-3-6.
//
//

#ifndef __HelloCpp__pixtkFunc__
#define __HelloCpp__pixtkFunc__

#include <iostream>
#include <string>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "pixtkDefine.h"
namespace_pixtk_begin
string numberToStr(int n);
CCTexture2D*scaleTexUsingRTT(CCTexture2D*texture,float scaleX,float scaleY);
CCSize getRectSize(const CCRect&rect);

namespace_pixtk_end
#endif /* defined(__HelloCpp__pixtkFunc__) */

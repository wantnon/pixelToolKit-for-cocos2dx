//
//  pixtkDefine.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-6.
//
//

#ifndef __HelloCpp__pixtkDefine__
#define __HelloCpp__pixtkDefine__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;

#define namespace_pixtk_begin namespace pixtk{
#define namespace_pixtk_end }

namespace_pixtk_begin
extern CCSize g_defaultPerPixelSize;
extern bool g_isShowBoundingBox;
extern bool g_isPrintFragMat;
const float pixINF=0.5*(numeric_limits<float>::max)();



namespace_pixtk_end
#endif /* defined(__HelloCpp__pixtkDefine__) */


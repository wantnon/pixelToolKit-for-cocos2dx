//
//  shotBall.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-9.
//
//

#ifndef __HelloCpp__shotBall__
#define __HelloCpp__shotBall__

#include <iostream>
using namespace std;
#include "pixtk.h"
using namespace::pixtk;
class CshotBall_dynamic:public CdynamicPixelSprite
{
public:
    CshotBall_dynamic(){
        m_isLaunched=false;
        m_isToRight=true;
    }
    virtual~CshotBall_dynamic(){}
    void setIsLaunched(bool value){m_isLaunched=value;}
    bool getIsLaunched()const{return m_isLaunched;}
    void setIsToRight(bool value){m_isToRight=value;}
    bool getIsToRight()const{return m_isToRight;}
protected:
    bool m_isLaunched;
    bool m_isToRight;
};
#endif /* defined(__HelloCpp__shotBall__) */

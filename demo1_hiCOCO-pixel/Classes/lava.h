//
//  lava.h
//  HelloCpp
//
//  Created by apple on 14-5-23.
//
//

#ifndef __HelloCpp__lava__
#define __HelloCpp__lava__

#include <iostream>
using namespace std;
#include "pixtk.h"
using namespace pixtk;
class Clava:public CdynamicPixelSprite
{
public:
    Clava(){
        m_time=0;
    }
    bool init(const string&texFileName,CCSize perPixelSize=g_defaultPerPixelSize){
        CdynamicPixelSprite::init(texFileName,perPixelSize);
        //table_sinJW and table_cosJW
        const float w=1.0/5;
        int nCol=getFragMatColCount();
        for (int j=0; j<nCol; j++) {
            table_sinJW.push_back(sinf(w*j));
            table_cosJW.push_back(cosf(w*j));
        }
        
        return true;
    }
    void wave(float dt){
        m_time+=dt;
        if(m_time>=pixINF)m_time=0;
        const float vx=5;
        float sinTimeVx=sinf(m_time*vx);
        float cosTimeVx=cosf(m_time*vx);
        int nRow=getFragMatRowCount();
        int nCol=getFragMatColCount();
        for(int i=0;i<nRow;i++){
            for(int j=0;j<nCol;j++){
                CfragSprite*fragSprite=m_fragMat[i][j];
                if(fragSprite==NULL)continue;
                float sinJWaddTimeVx=table_sinJW[j]*cosTimeVx+sinTimeVx*table_cosJW[j];//sin(j*w+m_time*vx)
                float offsety=8*(sinJWaddTimeVx-1);
                fragSprite->setPosition(fragSprite->getInitPosition()+CCPoint(0,offsety*(1-(float)i/(nRow-1))));
            }
        }
    }
protected:
    float m_time;
    vector<float> table_sinJW;
    vector<float> table_cosJW;
  

    
    
};
#endif /* defined(__HelloCpp__lava__) */

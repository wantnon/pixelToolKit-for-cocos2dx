//
//  pixtkDynamicPixelSprite.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-4.
//
//

#ifndef __HelloCpp__pixtkDynamicPixelSprite__
#define __HelloCpp__pixtkDynamicPixelSprite__

#include <iostream>
#include <vector>
#include <string>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "pixtkFunc.h"
#include "pixtkSimpleClass.h"
#include "pixtkDefine.h"
namespace_pixtk_begin
vector<vector<ccColor4B> > getColorMatFromImage(CCImage *image);
bool isZeroRect(const CCRect&rect);
class CdynamicPixelSprite;
class CfragSprite:public CCSprite{
public:
    CfragSprite(){}
    virtual~CfragSprite(){}
    bool init(const string&texFileName);
    bool init(CCTexture2D*texture);
    void setIJ(int i,int j){m_ij=Cij(i,j);}
    int getI()const{return m_ij.getI();}
    int getJ()const{return m_ij.getJ();}
    Cij getIJ()const{return m_ij;}
    void storeInitPosition(const CCPoint&point){m_initPosition=point;}
    CCPoint getInitPosition()const{return m_initPosition;}
protected:
    Cij m_ij;
    CCPoint m_initPosition;
};
class CdynamicPixelSprite:public CCNode
{
public:
    CdynamicPixelSprite();
    virtual~CdynamicPixelSprite(){}
    bool init(const string&texFileName,CCSize perPixelSize=g_defaultPerPixelSize);
    void noise();
    void relocateToOrigin();
    CCSpriteBatchNode* getFragRoot(){return m_fragRoot;}
    int getFragMatRowCount()const{return m_fragMat.size();}
    int getFragMatColCount()const{return (getFragMatRowCount()==0?0:(int)m_fragMat[0].size());}
    int getFragMatElementCount()const{return getFragMatRowCount()*getFragMatColCount();}
    CfragSprite* getFragByIndex(int i,int j);
    vector<CfragSprite*> getShellFrags();
    vector<CfragSprite*> getFragsInRect(const CCRect&rectInParentSpace);
    CCPoint getFragPosition(CfragSprite*frag);
    CCPoint getFragPosition(int i,int j);
    CCRect getFragRect(CfragSprite*frag);
    CCRect getMinRectForFrags(const vector<CfragSprite*>&fragList);
    CCRect getFragRect(int i,int j);
    bool getIsHasFrag(int i,int j);
    bool getIsHasFrag(CfragSprite*frag);
    vector<Cij> getFragsIndexInRect(const CCRect&rectInParentSpace);
    vector<Cij> getFragsIndexInRectGeneral(const CCRect&rectInParentSpace);//poor performance!
    CCRect getBoundingBoxInLocalSpace()const {return m_boundingBoxInLocalSpace;}
    CCRect getBoundingBox();
    CCPoint downDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth=0);
    CCPoint upDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth=0);
    CCPoint leftDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth=0);
    CCPoint rightDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth=0);
    void draw();
    void killFrag(int i,int j);
    void killFrag(CfragSprite*frag);
protected:
    vector<vector<CfragSprite*> > m_fragMat;
    CCSize m_perPixelSize;
    CCSpriteBatchNode* m_fragRoot;
    CCRect m_boundingBoxInLocalSpace;
};
namespace_pixtk_end
#endif /* defined(__HelloCpp__pixtkDynamicPixelSprite__) */

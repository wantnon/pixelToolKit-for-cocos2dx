//
//  pixtkDynamicPixelSprite.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-4.
//
//

#include "pixtkDynamicPixelSprite.h"
namespace_pixtk_begin
vector<vector<ccColor4B> > getColorMatFromImage(CCImage *image)
//ref to CCTexture2D::initPremultipliedATextureWithImage
{
    bool hasAlpha = image->hasAlpha();
    unsigned int width = image->getWidth();
    unsigned int height = image->getHeight();
    //color mat
    vector<vector<ccColor4B> > colorMat;
    colorMat.resize(height);
    for(int i=0;i<height;i++){
        colorMat[i].resize(width);
    }
    //fill colorMat using image data
    if(hasAlpha){
        unsigned int* inPixel32 = (unsigned int*)image->getData();

        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                colorMat[i][j].r=(int)((*inPixel32 >> 0) & 0xFF);
                colorMat[i][j].g=(int)((*inPixel32 >> 8) & 0xFF);
                colorMat[i][j].b=(int)((*inPixel32 >> 16) & 0xFF);
                colorMat[i][j].a=(int)((*inPixel32 >> 24) & 0xFF);
                inPixel32++;
            }
        }
    }else{
        //see CCImage implemention we know that CCImage data must be rgba8888
        //so image->hasAlpha() always return true
        //so will not enter this branch
        assert(false);
    
    }

    return colorMat;
}
bool isZeroRect(const CCRect&rect){
    if(rect.getMinX()==0&&rect.getMaxX()==0&&rect.getMinY()==0&&rect.getMaxY()==0)return true;
    else return false;
}
//-------------------------------------
bool CfragSprite::init(const string&texFileName){
    CCSprite::initWithFile(texFileName.c_str());
    return true;
}
bool CfragSprite::init(CCTexture2D*texture){
    CCSprite::initWithTexture(texture);
    return true;
}

//-------------------------------------
vector<CfragSprite*> CdynamicPixelSprite::getShellFrags(){
    vector<CfragSprite*> shellFragList;
    int nRow=getFragMatRowCount();
    int nCol=getFragMatColCount();
    for(int i=0;i<nRow;i++){
        for(int j=0;j<nCol;j++){
            if(m_fragMat[i][j]==NULL)continue;
            bool upFragIsNULL;
            if(i-1>=0){
                upFragIsNULL=(m_fragMat[i-1][j]==NULL);
            }else{
                upFragIsNULL=true;
            }
            bool dnFragIsNULL;
            if(i+1<nRow){
                dnFragIsNULL=(m_fragMat[i+1][j]==NULL);
            }else{
                dnFragIsNULL=true;
            }
            bool leftFragIsNULL;
            if(j-1>=0){
                leftFragIsNULL=(m_fragMat[i][j-1]==NULL);
            }else{
                leftFragIsNULL=true;
            }
            bool rightFragIsNULL;
            if(j+1<nCol){
                rightFragIsNULL=(m_fragMat[i][j+1]==NULL);
            }else{
                rightFragIsNULL=true;
            }
            if(upFragIsNULL==false&&dnFragIsNULL==false&&leftFragIsNULL==false&&rightFragIsNULL==false){
                //is not shell fragSprite
            }else{
                //is shell fragSprite
                shellFragList.push_back(m_fragMat[i][j]);
            }
        }
    }
    return shellFragList;
}

void CdynamicPixelSprite::noise(){
    int nRow=getFragMatRowCount();
    int nCol=getFragMatColCount();

    for(int i=0;i<nRow;i++){
        for(int j=0;j<nCol;j++){
            CfragSprite*fragSprite=m_fragMat[i][j];
            if(fragSprite==NULL)continue;
            float offsetx=(rand()%200-100)*0.01*4;
            float offsety=(rand()%200-100)*0.01*4;
           // float rot=rand()%360;
           // fragSprite->setRotation(rot);
            fragSprite->setPosition(fragSprite->getPosition()+CCPoint(offsetx,offsety));
  
        }
    }
}
void CdynamicPixelSprite::relocateToOrigin(){
    int nRow=getFragMatRowCount();
    int nCol=getFragMatColCount();
    
    for(int i=0;i<nRow;i++){
        for(int j=0;j<nCol;j++){
            CfragSprite*fragSprite=m_fragMat[i][j];
            if(fragSprite==NULL)continue;
            fragSprite->setPosition(fragSprite->getInitPosition());
            
        }
    }

}

bool CdynamicPixelSprite::init(const string&texFileName,CCSize perPixelSize){
    CCNode::init();
    m_perPixelSize=perPixelSize;
    //create pixel texture
    CCTexture2D*pixelTex=scaleTexUsingRTT(CCTextureCache::sharedTextureCache()->addImage("whitePixel.png"),m_perPixelSize.width,m_perPixelSize.height);
    //create frageRoot
    m_fragRoot=CCSpriteBatchNode::createWithTexture(pixelTex);
    addChild(m_fragRoot);
    //create colorMat
    CCImage*image=new CCImage();
    image->autorelease();
    bool succ=image->initWithImageFile(texFileName.c_str());
    assert(succ);
    vector<vector<ccColor4B> > pixelMat=getColorMatFromImage(image);
    //create fragSpriteMat
    int nRow=(int)pixelMat.size();
    int nCol=(nRow==0?0:(int)pixelMat[0].size());
    m_fragMat.resize(nRow);
    for(int i=0;i<nRow;i++){
        m_fragMat[i].resize(nCol);
    }
    float height=nRow*m_perPixelSize.height;
    float width=nCol*m_perPixelSize.width;
    for(int i=0;i<nRow;i++){
        for(int j=0;j<nCol;j++){
            ccColor4B color=pixelMat[i][j];
            if(color.a==0)continue;//skip totally transparent pixel (not create sprite for it)
            CfragSprite*fragSprite=new CfragSprite();
            fragSprite->autorelease();
            fragSprite->init(pixelTex);
            fragSprite->setColor(ccc3(color.r, color.g, color.b));
            fragSprite->setOpacity(color.a);
            fragSprite->setIJ(i,j);
            m_fragRoot->addChild(fragSprite);
            //m_fragRoot's position is in the center of fragSpriteMat
            float x=j*m_perPixelSize.width+m_perPixelSize.width/2-width/2;
            float y=height-(i*m_perPixelSize.height+m_perPixelSize.height/2)-height/2;
            fragSprite->setPosition(ccp(x,y));
            fragSprite->storeInitPosition(ccp(x,y));
            m_fragMat[i][j]=fragSprite;
            
            
        }
    }
    //boundingBox in local space
    m_boundingBoxInLocalSpace=CCRect(-width/2,-height/2,width,height);
    //---debug print
    if(g_isPrintFragMat){
        for(int i=0;i<nRow;i++){
            for(int j=0;j<nCol;j++){
                ccColor4B color=pixelMat[i][j];
                cout<<(color.a>0)<<" ";
            }
            cout<<endl;
        }
    }

    return true;
}
CdynamicPixelSprite::CdynamicPixelSprite(){
    m_fragRoot=NULL;
}


CfragSprite* CdynamicPixelSprite::getFragByIndex(int i,int j){
    assert(i>=0&&i<getFragMatRowCount());
    assert(j>=0&&j<getFragMatColCount());
    return m_fragMat[i][j];
}
vector<CfragSprite*> CdynamicPixelSprite::getFragsInRect(const CCRect&rectInParentSpace){
    vector<Cij> fragSpriteIndexList=getFragsIndexInRect(rectInParentSpace);
    vector<CfragSprite*> fragSpriteList;
    int nIJ=(int)fragSpriteIndexList.size();
    for(int i=0;i<nIJ;i++){
        const Cij&IJ=fragSpriteIndexList[i];
        fragSpriteList.push_back(m_fragMat[IJ.getI()][IJ.getJ()]);
    }
    return fragSpriteList;
}
CCPoint CdynamicPixelSprite::getFragPosition(CfragSprite*frag){
    assert(getIsHasFrag(frag));
    return CCPointApplyAffineTransform(frag->getPosition(),nodeToParentTransform());
}
CCPoint CdynamicPixelSprite::getFragPosition(int i,int j){
    CfragSprite*frag=getFragByIndex(i, j);
    assert(frag);
    return CCPointApplyAffineTransform(frag->getPosition(),nodeToParentTransform());
}
CCRect CdynamicPixelSprite::getFragRect(CfragSprite*frag){
    assert(getIsHasFrag(frag));
    return CCRectApplyAffineTransform(frag->boundingBox(),nodeToParentTransform());
}
CCRect CdynamicPixelSprite::getMinRectForFrags(const vector<CfragSprite*>&fragList){
    int nFrag=(int)fragList.size();
    if(nFrag==0)return CCRect(0,0,0,0);
    for(int i=0;i<nFrag;i++){
        CfragSprite*frag=fragList[i];
        assert(getIsHasFrag(frag));
    }
    float xmin=pixINF;
    float xmax=-pixINF;
    float ymin=pixINF;
    float ymax=-pixINF;
    for(int i=0;i<nFrag;i++){
        CfragSprite*frag=fragList[i];
        const CCRect&rect=getFragRect(frag);
        if(rect.getMinX()<xmin)xmin=rect.getMinX();
        if(rect.getMaxX()>xmax)xmax=rect.getMaxX();
        if(rect.getMinY()<ymin)ymin=rect.getMinY();
        if(rect.getMaxY()>ymax)ymax=rect.getMaxY();
    }//got xmin,xmax,ymin,ymax
    return CCRect(xmin,ymin,xmax-xmin,ymax-ymin);
}
CCRect CdynamicPixelSprite::getFragRect(int i,int j){
    CfragSprite*frag=getFragByIndex(i, j);
    assert(frag);
    return CCRectApplyAffineTransform(frag->boundingBox(),nodeToParentTransform());
}
bool CdynamicPixelSprite::getIsHasFrag(int i,int j){
    int nRow=getFragMatRowCount();
    int nCol=getFragMatColCount();
    if(i<0)return false;
    if(j<0)return false;
    if(i>=nRow)return false;
    if(j>=nCol)return false;
    //(i,j) is in range of [0,nRow)x[0,nCol)
    if(m_fragMat[i][j]==NULL)return false;
    return true;
}
bool CdynamicPixelSprite::getIsHasFrag(CfragSprite*frag){
    assert(frag);
    int I=frag->getI();
    int J=frag->getJ();
    if(getIsHasFrag(I,J)==false)return false;
    if(m_fragMat[I][J]!=frag)return false;
    return true;
}
vector<Cij> CdynamicPixelSprite::getFragsIndexInRect(const CCRect&rectInParentSpace){
    vector<Cij> fragIndexList;
    //transform rect to node space
    CCAffineTransform parentToNodeTransform=this->parentToNodeTransform();
    CCRect rectInNodeSpace=CCRectApplyAffineTransform(rectInParentSpace, parentToNodeTransform);
    //rect range (local space)
    float xmin_rt=rectInNodeSpace.getMinX();
    float xmax_rt=rectInNodeSpace.getMaxX();
    float ymin_rt=rectInNodeSpace.getMinY();
    float ymax_rt=rectInNodeSpace.getMaxY();
    //sprite range (local space)
    float xmin_sp=m_boundingBoxInLocalSpace.getMinX();
    float xmax_sp=m_boundingBoxInLocalSpace.getMaxX();
    float ymin_sp=m_boundingBoxInLocalSpace.getMinY();
    float ymax_sp=m_boundingBoxInLocalSpace.getMaxY();
    //calculate index range from position range
    //note: min is floor(x), max is ceil ceil(x)-1
    int imin,imax,jmin,jmax;
    imin=MAX(0,floorf((ymax_sp-ymax_rt)/m_perPixelSize.height));
    imax=MIN(getFragMatRowCount()-1,ceilf((ymax_sp-ymin_rt)/m_perPixelSize.height)-1);
    jmin=MAX(0,floorf((xmin_rt-xmin_sp)/m_perPixelSize.width));
    jmax=MIN(getFragMatColCount()-1,ceilf((xmax_rt-xmin_sp)/m_perPixelSize.width)-1);
    //gather all fragSprites in range [imin,imax]x[jmin,jmax]
    for(int i=imin;i<=imax;i++){
        for(int j=jmin;j<=jmax;j++){
            if(m_fragMat[i][j]){
                fragIndexList.push_back(Cij(i,j));
            }
        }
    }
    return fragIndexList;
}
vector<Cij> CdynamicPixelSprite::getFragsIndexInRectGeneral(const CCRect&rectInParentSpace){
    vector<Cij> fragIndexList;
    //transform rect to node space
    CCAffineTransform parentToNodeTransform=this->parentToNodeTransform();
    CCRect rectInNodeSpace=CCRectApplyAffineTransform(rectInParentSpace, parentToNodeTransform);
    int nRow=getFragMatRowCount();
    int nCol=getFragMatColCount();
    for(int i=0;i<nRow;i++){
        for(int j=0;j<nCol;j++){
            if(getIsHasFrag(i, j)==false)continue;
            CfragSprite*frag=m_fragMat[i][j];
            CCRect fragRect=frag->boundingBox();
            //fragRect collision with rectInNodeSpace
            if(rectInNodeSpace.intersectsRect(fragRect)){
                fragIndexList.push_back(Cij(i,j));
            }
        }
    }
    
    return fragIndexList;
}
CCRect CdynamicPixelSprite::getBoundingBox() {
    return CCRectApplyAffineTransform(m_boundingBoxInLocalSpace, nodeToParentTransform());
}
CCPoint CdynamicPixelSprite::downDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth)
//rayStartPoint, rayLen and rayWidth are in parent space
//return the intersect point
//if intersect point is out of rayLen, then return (rayStartPoint.x,-INFINITY)
{
    CCRect rect_parentSpace=CCRect(rayStartPoint.x-rayWidth/2,rayStartPoint.y-rayLen,rayWidth,rayLen);
    vector<CfragSprite*> collisionFrags=this->getFragsInRect(rect_parentSpace);
    float maxY=-pixINF;
    float x=0;
    int nCollisionFrag=(int)collisionFrags.size();
    for(int i=0;i<nCollisionFrag;i++){
        CCRect frageRect_parentSpace=CCRectApplyAffineTransform(collisionFrags[i]->boundingBox(),nodeToParentTransform());//transform to this's parent space
        if(frageRect_parentSpace.getMaxY()>maxY){
            maxY=frageRect_parentSpace.getMaxY();
            x=(frageRect_parentSpace.getMinX()+frageRect_parentSpace.getMaxX())/2;
        }
    }//got maxY
    CCPoint intersectPoint=CCPoint(x,maxY);
    return intersectPoint;
}
CCPoint CdynamicPixelSprite::upDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth)
//rayStartPoint, rayLen and rayWidth are in parent space
//return the intersect point
//if intersect point is out of rayLen, then return (rayStartPoint.x,INFINITY)
{
    CCRect rect_parentSpace=CCRect(rayStartPoint.x-rayWidth/2,rayStartPoint.y,rayWidth,rayLen);
    vector<CfragSprite*> collisionFrags=this->getFragsInRect(rect_parentSpace);
    float minY=pixINF;
    float x=0;
    int nCollisionFrag=(int)collisionFrags.size();
    for(int i=0;i<nCollisionFrag;i++){
        CCRect frageRect_parentSpace=CCRectApplyAffineTransform(collisionFrags[i]->boundingBox(),nodeToParentTransform());//transform to this's parent space
        if(frageRect_parentSpace.getMaxY()<minY){
            minY=frageRect_parentSpace.getMaxY();
            x=(frageRect_parentSpace.getMinX()+frageRect_parentSpace.getMaxX())/2;
        }
    }//got minH
    CCPoint intersectPoint=CCPoint(x,minY);
    return intersectPoint;
}
CCPoint CdynamicPixelSprite::leftDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth)
//rayStartPoint, rayLen and rayWidth are in parent space
//return the intersect point
//if intersect point is out of rayLen, then return (-INFINITY,rayStartPoint.y)
{
    CCRect rect_parentSpace=CCRect(rayStartPoint.x-rayLen,rayStartPoint.y-rayWidth/2,rayLen,rayWidth);
    vector<CfragSprite*> collisionFrags=this->getFragsInRect(rect_parentSpace);
    float maxX=-pixINF;
    float y=0;
    int nCollisionFrag=(int)collisionFrags.size();
    for(int i=0;i<nCollisionFrag;i++){
        CCRect frageRect_parentSpace=CCRectApplyAffineTransform(collisionFrags[i]->boundingBox(),nodeToParentTransform());//transform to this's parent space
        if(frageRect_parentSpace.getMaxX()<maxX){
            maxX=frageRect_parentSpace.getMaxX();
            y=(frageRect_parentSpace.getMinY()+frageRect_parentSpace.getMaxY())/2;
        }
    }//got maxX
    CCPoint intersectPoint=CCPoint(maxX,y);
    return intersectPoint;
}
CCPoint CdynamicPixelSprite::rightDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth)
//rayStartPoint, rayLen and rayWidth are in parent space
//return the intersect point
//if intersect point is out of rayLen, then return (INFINITY,rayStartPoint.y)
{
    CCRect rect_parentSpace=CCRect(rayStartPoint.x,rayStartPoint.y-rayWidth/2,rayLen,rayWidth);
    vector<CfragSprite*> collisionFrags=this->getFragsInRect(rect_parentSpace);
    float minX=pixINF;
    float y=0;
    int nCollisionFrag=(int)collisionFrags.size();
    for(int i=0;i<nCollisionFrag;i++){
        CCRect frageRect_parentSpace=CCRectApplyAffineTransform(collisionFrags[i]->boundingBox(),nodeToParentTransform());//transform to this's parent space
        if(frageRect_parentSpace.getMaxX()<minX){
            minX=frageRect_parentSpace.getMaxX();
            y=(frageRect_parentSpace.getMinY()+frageRect_parentSpace.getMaxY())/2;
        }
    }//got minX
    CCPoint intersectPoint=CCPoint(minX,y);
    return intersectPoint;
}


void CdynamicPixelSprite::draw(){
    if(g_isShowBoundingBox){
        CCRect rect=m_boundingBoxInLocalSpace;
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
void CdynamicPixelSprite::killFrag(int i,int j){
    assert(getIsHasFrag(i, j));
    m_fragMat[i][j]->removeFromParentAndCleanup(true);
    m_fragMat[i][j]=NULL;
}
void CdynamicPixelSprite::killFrag(CfragSprite*frag){
    assert(getIsHasFrag(frag));
    Cij IJ=frag->getIJ();
    m_fragMat[IJ.getI()][IJ.getJ()]->removeFromParentAndCleanup(true);
    m_fragMat[IJ.getI()][IJ.getJ()]=NULL;
}


namespace_pixtk_end

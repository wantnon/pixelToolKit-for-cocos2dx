//
//  pixtkSimpleClass.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-5-8.
//
//

#ifndef __HelloCpp__pixtkSimpleClass__
#define __HelloCpp__pixtkSimpleClass__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "pixtkDefine.h"
namespace_pixtk_begin

class Cij{
public:
    Cij(){
        initMemebers();
    }
    virtual~Cij(){}
    Cij(int i,int j){
        initMemebers();
        m_i=i;
        m_j=j;
    }
    void setI(int i){m_i=i;}
    void setJ(int j){m_j=j;}
    void setIJ(int i,int j){
        m_i=i;
        m_j=j;
    }
    int getI()const{return m_i;}
    int getJ()const{return m_j;}
protected:
    void initMemebers(){
        m_i=m_j=0;
    }
protected:
    int m_i,m_j;
};

class CcollisionRect
{
public:
    CcollisionRect(){};
    virtual~CcollisionRect(){};
    CcollisionRect(const CCRect&rect,const string&name){
        init(rect, name);
    }
    bool init(const CCRect&rect,const string&name){
        setRect(rect);
        setName(name);
        return true;
    }
    void setRect(const CCRect&rect){m_rect=rect;}
    void setName(const string&name){m_name=name;}
    CCRect getRect()const{return m_rect;}
    string getName()const{return m_name;}
protected:
    CCRect m_rect;
    string m_name;
};

namespace_pixtk_end
#endif /* defined(__HelloCpp__pixtkSimpleClass__) */

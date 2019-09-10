/*!
* \file Polygons.h
* \brief 多边形形状类
* \author 王澳
* \date 2019.09
*/

#ifndef POLYGONS_H
#define POLYGONS_H

#include"Shape2D.h"

/// \brief 代表多边形形状，继承Shape2D类
class My::Polygons:public My::Shape2D{
public:
    
    Polygons():Shape2D(Shape2D::Polygons){}

    /// \brief 绘制形状函数
    virtual void draw(QWidget* w);

    /// \brief 判断是否在形状内函数
    virtual bool isInShape(QPointF p,QWidget* w);

    /// \brief 偏移形状函数
    virtual void offset(float xOffset,float yOffset);
};
#endif // POLYGONS_H

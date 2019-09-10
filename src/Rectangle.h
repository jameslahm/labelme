/*!
* \file Rectangle.h
* \brief 矩形形状类
* \author 王澳
* \date 2019.09
*/

#ifndef RECTANGLE_H
#define RECTANGLE_H
#include"Shape2D.h"

/// \brief 代表矩形形状，继承Shape2D类
class My::Rectangle:public My::Shape2D{
public:
    
    /// \brief 存储矩形的宽
    float width;

    /// \brief 存储矩形的高
    float height;

    Rectangle():Shape2D(Shape2D::Rectangle){}

    /// \brief 绘制形状函数
    virtual void draw(QWidget* w);

    /// \brief 判断是否在形状内函数
    virtual bool isInShape(QPointF p,QWidget* w);

    /// \brief 偏移形状函数
    virtual void offset(float xOffset,float yOffset);
};

#endif // RECTANGLE_H

/*!
* \file Circle.h
* \brief 圆形形状类
* \author 王澳
* \date 2019.09
*/

#ifndef CIRCLE_H
#define CIRCLE_H
#include"Shape2D.h"

/// \brief 代表圆形状，继承Shape2D类
class My::Circle:public My::Shape2D{
public:
    
    /// \brief 代表圆的半径大小
    float radius;
    Circle():Shape2D(Shape2D::Circle){}

    /// \brief 绘制形状函数
    virtual void draw(QWidget* w);

    /// \brief 判断是否在形状内函数
    virtual bool isInShape(QPointF p,QWidget* w);

    /// \brief 偏移形状函数
    virtual void offset(float xOffset,float yOffset);
};

#endif // CIRCLE_H

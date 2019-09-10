/*!
* \file Brush.h
* \brief 画刷形状类，用于分割标注
* \author 王澳
* \date 2019.09
*/

#ifndef BRUSH_H
#define BRUSH_H
#include"Shape2D.h"

/// \brief 代表画刷形状，即分割标注所用形状，继承Shape2D类
class My::Brush:public My::Shape2D{
public:
    
    /// \brief 存储标注像素点的一系列绘制大小
    QVector<float> radius;

    Brush():Shape2D(Shape2D::Brush){}

    /// \brief 绘制形状函数
    virtual void draw(QWidget* w);

    /// \brief 判断是否在形状内函数
    virtual bool isInShape(QPointF p,QWidget* w);

    /// \brief 偏移形状函数
    virtual void offset(float xOffset,float yOffset);
};
#endif // BRUSH_H

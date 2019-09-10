/*!
* \file Curve.h
* \brief 平滑曲线形状类
* \author 王澳
* \date 2019.09
*/

#ifndef CURVE_H
#define CURVE_H
#include"Shape2D.h"

/// \brief 代表平滑曲线形状，继承Shape2D类
class My::Curve:public My::Shape2D{
public:
    
    Curve():Shape2D(Shape2D::Curve){}

    /// \brief 绘制形状函数
    virtual void draw(QWidget* w);

    /// \brief 判断是否在形状内函数
    virtual bool isInShape(QPointF p,QWidget* w);

    /// \brief 偏移形状函数
    virtual void offset(float xOffset,float yOffset);
};
#endif // CURVE_H

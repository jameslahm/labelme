/*!
* \file Shape2D.h
* \brief 2d形状纯虚基类
* \author 王澳
* \date 2019.09
*/

#ifndef SHAPE2D_H
#define SHAPE2D_H
#include<QVector>
#include<QPoint>
#include<QLabel>
#include<opencv2/opencv.hpp>
#include"Shape.h"

/// \brief 2d标注形状的基类
class My::Shape2D:public My::Shape{
public:

    /// \brief 存储标注形状的像素点位
    QVector<QPointF> points;
    Shape2D(My::Shape::Type t):Shape(t){}

    /// \brief 绘制标注形状虚函数
    virtual void draw(QWidget* w);

    /// \brief 判断鼠标是否在标注形状内部虚函数
    virtual bool isInShape(QPointF p,QWidget* w);

    /// \brief 偏移标注形状虚函数
    virtual void offset(float xOffset,float yOffset);
    virtual ~Shape2D()=0;
};

#endif // SHAPE_H



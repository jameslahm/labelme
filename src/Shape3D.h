/*!
* \file Shape3D.h
* \brief 3d形状纯虚基类
* \author 王澳
* \date 2019.09
*/

#ifndef SHAPE3D_H
#define SHAPE3D_H
#include<QVector>
#include<opencv2/opencv.hpp>
#include"Shape.h"

/// \brief 3d标注形状的基类
class My::Shape3D:public My::Shape{
public:

    /// \brief 存储像素点位
    QVector<cv::Point3f> points;
public:
    Shape3D(Type t);
    virtual ~Shape3D()=0;
};

#endif // SHAPE3D_H

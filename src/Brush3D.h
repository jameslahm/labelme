/*!
* \file Brush3D.h
* \brief 3d画刷形状类，用于3d状态分割标注
* \author 王澳
* \date 2019.09
*/

#ifndef BRUSH3D_H
#define BRUSH3D_H

#include"Shape3D.h"

/// \brief 代表画刷形状，用于3d分割标注，继承Shape3D
class My::Brush3D:public My::Shape3D{
public:

    /// \brief 存储一系列像素点的大小
    QVector<float> radius;
public:
    Brush3D():Shape3D(My::Shape3D::Brush3D){}
    ~Brush3D();
};
#endif // BRUSH3D_H

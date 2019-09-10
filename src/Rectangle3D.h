/*!
* \file Rectangle3D.h
* \brief 3d长方体形状类，用于3d状态检测标注
* \author 王澳
* \date 2019.09
*/

#ifndef RECTANGLE3D_H
#define RECTANGLE3D_H

#include"Shape3D.h"

/// \brief 代表长方体形状，用于3d分割标注，继承Shape3D
class My::Rectangle3D:public My::Shape3D{
public:

    /// \brief 存储长方体的宽
    float width=0;

    /// \brief 存储长方体的高
    float height=0;

    /// \brief 存储长方体的z方向跨度
    int zspan=0;

public:
    Rectangle3D():Shape3D(My::Shape3D::Rectangle3D){}
    ~Rectangle3D();
};

#endif // RECTANGLE3D_H

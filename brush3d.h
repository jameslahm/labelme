#ifndef BRUSH3D_H
#define BRUSH3D_H

#include"shape3d.h"

class My::Brush3D:public My::Shape3D{
public:
    QVector<float> radius;
public:
    Brush3D():Shape3D(My::Shape3D::Brush3D){}

};
#endif // BRUSH3D_H

#ifndef RECTANGLE3D_H
#define RECTANGLE3D_H

#include"shape3d.h"

class My::Rectangle3D:public My::Shape3D{
public:
    float width=0;
    float height=0;
    int zspan=0;
public:
    Rectangle3D():Shape3D(My::Shape3D::Rectangle3D){}

};

#endif // RECTANGLE3D_H

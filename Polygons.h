#ifndef POLYGONS_H
#define POLYGONS_H

#include"Shape.h"

class My::Polygons:public My::Shape{
public:

    Polygons():Shape(Shape::Polygons){}
    void draw(QWidget* w);
//    virtual void draw(QPixmap* w);
    bool isInShape(QPointF p,QWidget* w);
    void offset(float xOffset,float yOffset);
};
#endif // POLYGONS_H

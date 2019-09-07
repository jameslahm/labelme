#ifndef CURVE_H
#define CURVE_H
#include"Shape.h"

class My::Curve:public My::Shape{
public:

    Curve():Shape(Shape::Curve){}

    void draw(QWidget* w);
//    virtual void draw(QPixmap* w);
    bool isInShape(QPointF p,QWidget* w);
    void offset(float xOffset,float yOffset);
};
#endif // CURVE_H

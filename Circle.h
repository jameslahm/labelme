#ifndef CIRCLE_H
#define CIRCLE_H
#include"Shape.h"

class My::Circle:public My::Shape{
public:
    float radius;
    Circle():Shape(Shape::Circle){}
    void draw(QWidget* w);
//    virtual void draw(QPixmap* w);
    bool isInShape(QPointF p,QWidget* w);
    void offset(float xOffset,float yOffset);
};

#endif // CIRCLE_H

#ifndef RECTANGLE_H
#define RECTANGLE_H
#include"Shape.h"
class My::Rectangle:public My::Shape{
public:
    float width;
    float height;

    Rectangle():Shape(Shape::Rectangle){}
    void draw(QWidget* w);
//    virtual void draw(QPixmap* w);
    bool isInShape(QPointF p,QWidget* w);
    void offset(float xOffset,float yOffset);
};

#endif // RECTANGLE_H

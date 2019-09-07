#ifndef BRUSH_H
#define BRUSH_H
#include"Shape.h"
class My::Brush:public My::Shape{
public:
    QVector<float> radius;

    Brush():Shape(Shape::Brush){}

    void draw(QWidget* w);
//    virtual void draw(QPixmap* w);
    bool isInShape(QPointF p,QWidget* w);
    void offset(float xOffset,float yOffset);
};
#endif // BRUSH_H

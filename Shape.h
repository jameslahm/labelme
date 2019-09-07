#ifndef SHAPE_H
#define SHAPE_H
#include"Namespace.h"
#include<QString>
#include<QColor>
#include<QPoint>
#include<QList>
#include<QLabel>
#include<opencv2/opencv.hpp>
#include<QPen>
class My::Shape{
public:
    enum Type{Brush,Rectangle,Polygons,Circle,Curve};
    const Type type;
    QString label;
    QColor color=QColor(100,255,0,100);
    bool isFill=false;
    bool isHide=false;
    bool isHover=false;
//    bool isHover=false;
    QVector<QPointF> points;
    Shape(Type t):type(t){}
    virtual void draw(QWidget* w);
//    virtual void draw(QPixmap* w);
    virtual bool isInShape(QPointF p,QWidget* w);
    virtual void offset(float xOffset,float yOffset);
    virtual ~Shape()=0;
};

#endif // SHAPE_H



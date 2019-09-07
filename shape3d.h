#ifndef SHAPE3D_H
#define SHAPE3D_H
#include"Namespace.h"
#include<QVector>
#include<opencv2/opencv.hpp>
#include<QColor>
#include<vector>

class My::Shape3D{
public:
    enum Type {Rectangle3D,Brush3D};
    Type type;
    QString label;
    QColor color;
    QVector<cv::Point3f> points;
    bool isHover=false;
    bool isFill=false;
    bool isHide=false;
public:
    Shape3D(Type t):type(t){}
    virtual ~Shape3D()=0;
};

#endif // SHAPE3D_H

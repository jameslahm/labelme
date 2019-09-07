#ifndef PARSE_H
#define PARSE_H
#include<QList>
#include<QFileInfo>
#include<QFile>
#include<QString>
#include<QJsonDocument>
#include<QJsonParseError>
#include"Shape.h"
#include<QDir>
#include<QDebug>
#include"Polygons.h"
#include"Circle.h"
#include"Rectangle.h"
#include"Brush.h"
#include"Curve.h"
#include<QJsonArray>
#include<QJsonObject>
#include"shape3d.h"
#include<opencv2/opencv.hpp>
#include"brush3d.h"
#include"rectangle3d.h"
#include<QFileInfo>
#include<QDir>
#include<QDirIterator>
#include"Namespace.h"

namespace My {
    class Parse;
}

class My::Parse{
public:
    static void parse(const QString& fileName,QList<My::Shape*>& shapes,const QString& labelfileName);
    static void write(const QString& fileName,QList<My::Shape*>& shapes);
    static void parse3D(const QString& fileName,QList<cv::Mat>& list,QList<My::Shape3D*>& shapes,const QString& labelfileName);
    static void parseImage(int dimension,QList<cv::Mat>& list,QList<My::Shape3D*>& shapes,QList<Label3D*>& labels);
    static void write3D(const QString& fileName,QList<My::Shape3D*>& shapes);
};

#endif // PARSE_H

/*!
* \file Parse.h
* \brief 2d、3d图片及标注解析类
* \author 王澳
* \date 2019.09
*/

#ifndef PARSE_H
#define PARSE_H
#include<QDebug>
#include<QList>
#include<QFileInfo>
#include<QFile>
#include<QDir>
#include<QString>
#include<QJsonDocument>
#include<QJsonParseError>
#include<QJsonArray>
#include<QJsonObject>
#include<QDirIterator>
#include"Namespace.h"
#include<opencv2/opencv.hpp>



/// \brief 负责解析标注文件以及图片
///
/// 2d、3d图片及标注文件的解析和保存均由Parse类负责
class My::Parse{
public:

    /// \brief 2d图片及标注的解析
    static void parse(const QString& fileName,QList<My::Shape2D*>& shapes,const QString& labelfileName);

    /// \brief 2d图片及标注信息的保存
    static void write(const QString& fileName,QList<My::Shape2D*>& shapes);

    /// \brief 3d图片及标注的解析
    static void parse3D(const QString& fileName,QList<cv::Mat>& list,QList<My::Shape3D*>& shapes,const QString& labelfileName);

    /// \brief 3d图片及标注某个视角的解析
    static void parseImage(int dimension,QList<cv::Mat>& list,QList<My::Shape3D*>& shapes,QList<Label3D*>& labels);

    /// \brief 3d图片及标注信息的保存
    static void write3D(const QString& fileName,QList<My::Shape3D*>& shapes);
};

#endif // PARSE_H

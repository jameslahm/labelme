/*!
* \file parse.cpp
* \brief 2d、3d图片及标注解析类
* \author 王澳
* \date 2019.09
*/

#include"Parse.h"
#include"Shape2D.h"
#include"Shape3D.h"
#include"Polygons.h"
#include"Circle.h"
#include"Rectangle.h"
#include"Brush.h"
#include"Curve.h"
#include"Brush3D.h"
#include"Rectangle3D.h"
#include"Label.h"
#include"Label3D.h"


/// \brief 解析2d图片以及标注信息
void My::Parse::parse(const QString& fileName,QList<My::Shape2D*>& shapes,const QString& labelfileName){

    //先清空标注形状列表
    for(int i=0;i<shapes.length();i++){
        delete shapes[i];
        shapes[i]=nullptr;
    }
    shapes.clear();

    //若无指明的标注文件，则默认为同名加json后缀
    QFileInfo fileInfo(fileName);
    QString labelFileName;
    if(labelfileName.isEmpty())labelFileName=fileInfo.absoluteDir().absolutePath()+"/"+fileInfo.baseName()+".json";
    else labelFileName=labelfileName;
    QFile labelfile(labelFileName);
    if(!labelfile.exists()){return;}
    if(!labelfile.open(QIODevice::ReadOnly)){
        qDebug()<<"some error when read the file"<<endl;
        return;
    }
    QByteArray allData=labelfile.readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData,&jsonError));

    if(jsonError.error!=QJsonParseError::NoError){
        qDebug()<<"json parse error"<<endl;
        return;
    }
    if(jsonDoc.isNull()||jsonDoc.isEmpty()){return;}
    QVariantList dataList=jsonDoc.toVariant().toList();

    //解析json文件
    foreach(QVariant data,dataList){
        QVariantMap dataMap=data.toMap();
        QString type=dataMap.value("type").toString();
        QString label=dataMap.value("label").toString();
        QVariantList colorList=dataMap.value("color").toList();
        QVariantList pointsList=dataMap.value("points").toList();
        if(type.isNull()||label.isNull()){
            qDebug()<<"No Type or Label"<<endl;
        }
        int r=colorList[0].toInt();
        int g=colorList[1].toInt();
        int b=colorList[2].toInt();
        int a=colorList.value(3,100).toInt();
        QColor color(r,g,b,a);

        My::Shape2D* shape;
        if(type=="Brush"){
            My::Brush* brushShape=new My::Brush();
            foreach(QVariant r,dataMap.value("radius").toList()){
                brushShape->radius.append(r.toFloat());
            }
            shape=brushShape;
            shape->label=label;
            shape->color=color;
        }
        else if(type=="Rectangle"){
            My::Rectangle* rectangleShape=new My::Rectangle();
            rectangleShape->width=dataMap.value("width").toFloat();
            rectangleShape->height=dataMap.value("height").toFloat();
            shape=rectangleShape;
            shape->label=label;
            shape->color=color;
        }
        else if(type=="Polygons"){
            shape=new My::Polygons();
            shape->label=label;
            shape->color=color;
        }
        else if(type=="Circle"){
            My::Circle* circleShape=new My::Circle();
            circleShape->radius=dataMap.value("radius").toFloat();
            shape=circleShape;
            shape->label=label;
            shape->color=color;
        }
        else if(type=="Curve"){
            shape=new My::Curve();
            shape->label=label;
            shape->color=color;
        }
        else{
            qDebug()<<"Not Support Shape"<<endl;
            return;
        }
        foreach(QVariant points,pointsList){
            float point1=points.toList()[0].toFloat();
            float point2=points.toList()[1].toFloat();
            QPointF point=QPointF(qreal(point1),qreal(point2));
            shape->points.append(point);
        }

        shapes.append(shape);
    }
}


/// \brief 2d图片以及标注信息的保存写入
void My::Parse::write(const QString& fileName,QList<My::Shape2D*>& shapes){

    //形成json格式
    QFileInfo fileInfo(fileName);
    QString labelFileName=fileInfo.absoluteDir().absolutePath()+"/"+fileInfo.baseName()+".json";
    QFile labelfile(labelFileName);
    QJsonArray jsonArray;
    foreach(My::Shape2D* shape,shapes){
        QJsonObject obj;
        QJsonArray colors;
        QJsonArray points;
        colors.append(shape->color.red());
        colors.append(shape->color.green());
        colors.append(shape->color.blue());
        colors.append(shape->color.alpha());
        foreach(QPointF p,shape->points){
            QJsonArray a;
            a.append(p.x());
            a.append(p.y());
            points.append(a);
        }
        obj.insert("color",colors);
        obj.insert("points",points);
        obj.insert("label",shape->label);
        if(shape->type==My::Shape::Brush){
            My::Brush* brush=dynamic_cast<My::Brush*>(shape);
            QJsonArray radius;
            obj.insert("type","Brush");
            obj.insert("shape","circle");
            for(int i=0;i<brush->radius.length();i++){
                    radius.append(brush->radius[i]);
            }

            obj.insert("radius",radius);
        }
        if(shape->type==My::Shape::Rectangle){
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(shape);
            obj.insert("type","Rectangle");
            obj.insert("width",rectangle->width);
            obj.insert("height",rectangle->height);
        }
        if(shape->type==My::Shape::Polygons){
            obj.insert("type","Polygons");
        }
        if(shape->type==My::Shape::Circle){
            My::Circle* circle=dynamic_cast<My::Circle*>(shape);
            obj.insert("type","Circle");
            obj.insert("radius",circle->radius);
        }
        if(shape->type==My::Shape::Curve){
            obj.insert("type","Curve");
        }
        jsonArray.append(obj);
    }

    //写入文件
    QJsonDocument doc(jsonArray);
    QByteArray ba=doc.toJson(QJsonDocument::Indented);
    if(!labelfile.open(QIODevice::WriteOnly)){
        qDebug()<<"file write error"<<endl;
    }
    labelfile.write(ba);
    labelfile.close();
}


/// \brief 解析3d图片以及标注信息
void My::Parse::parse3D(const QString& fileName,QList<cv::Mat>& list,QList<My::Shape3D*>& shapes,const QString& labelfileName){

    //先清空标注形状列表
    for(int i=0;i<shapes.length();i++){
        delete shapes[i];
        shapes[i]=nullptr;
    }
    shapes.clear();

    //清空图片列表
    list.clear();

    //发现一系列文件并存储
    QFileInfo fileInfo(fileName);
    QString dirName=fileInfo.absoluteDir().absolutePath();

    QStringList filters;
    QStringList fileNames;
    filters<<"*.jpg"<<"*.bmp"<<"*.jpeg"<<"*.png"<<"*.gif";
    QDirIterator iter(dirName,filters,QDir::Files,QDirIterator::Subdirectories);
    while(iter.hasNext()){
        iter.next();
        fileNames.append(iter.fileInfo().absoluteFilePath());
    }
    for(int i=0;i<fileNames.length();i++){
        cv::Mat m=cv::imread(fileNames[i].toStdString(),cv::IMREAD_GRAYSCALE);
        list.append(m);
    }

    //解析标注文件
    QString labelFileName;
    if(labelfileName.isEmpty())labelFileName=fileInfo.absoluteDir().absolutePath()+"/"+fileInfo.baseName()+"_3D.json";
    else labelFileName=labelfileName;

    QFile labelfile(labelFileName);
    if(!labelfile.exists()){return;}
    if(!labelfile.open(QIODevice::ReadOnly)){
        qDebug()<<"some error when read the file"<<endl;
        return;
    }
    QByteArray allData=labelfile.readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData,&jsonError));

    if(jsonError.error!=QJsonParseError::NoError){
        qDebug()<<"json parse error"<<endl;
        return;
    }
    if(jsonDoc.isNull()||jsonDoc.isEmpty()){return;}
    QVariantList dataList=jsonDoc.toVariant().toList();


    //解析json文件
    foreach(QVariant data,dataList){
        QVariantMap dataMap=data.toMap();
        QString type=dataMap.value("type").toString();
        QString label=dataMap.value("label").toString();
        QVariantList colorList=dataMap.value("color").toList();
        QVariantList pointsList=dataMap.value("points").toList();
        if(type.isNull()||label.isNull()){
            qDebug()<<"No Type or Label"<<endl;
        }
        int r=colorList[0].toInt();
        int g=colorList[1].toInt();
        int b=colorList[2].toInt();
        int a=colorList.value(3,100).toInt();
        QColor color(r,g,b,a);

        My::Shape3D* shape3D;
        if(type=="Brush3D"){
            My::Brush3D* brushShape3D=new My::Brush3D();
            foreach(QVariant r,dataMap.value("radius").toList()){
                brushShape3D->radius.append(r.toFloat());
            }
            shape3D=brushShape3D;
            shape3D->label=label;
            shape3D->color=color;
        }
        else if(type=="Rectangle3D"){
            My::Rectangle3D* rectangleShape3D=new My::Rectangle3D();
            rectangleShape3D->width=dataMap.value("width").toFloat();
            rectangleShape3D->height=dataMap.value("height").toFloat();
            rectangleShape3D->zspan=dataMap.value("zspan").toInt();
            shape3D=rectangleShape3D;
            shape3D->label=label;
            shape3D->color=color;
        }
        else{
            qDebug()<<"Not Support Shape"<<endl;
            return;
        }
        foreach(QVariant points,pointsList){
            float point1=points.toList()[0].toFloat();
            float point2=points.toList()[1].toFloat();
            int point3=points.toList()[2].toInt();
            cv::Point3f p(point1,point2,point3);
            shape3D->points.append(p);
        }

        shapes.append(shape3D);
    }


}


/// \brief 解析3d图片某视角的图片以及标注信息
void My::Parse::parseImage(int dimension,QList<cv::Mat>& list,QList<My::Shape3D*>& shapes,QList<Label3D*>& labels){

    //先清空标注形状列表
    for(int j=0;j<labels[dimension]->shapes.length();j++){
        delete labels[dimension]->shapes[j];
    }
    labels[dimension]->shapes.clear();

    //分视角讨论
    if(dimension==0){

        //先形成图片
        cv::Mat mat1=list[labels[0]->zindex];
        QImage image1(mat1.data,mat1.cols,mat1.rows,static_cast<int>(mat1.step),QImage::Format_Grayscale8);
        *(labels[0]->pixmap)=QPixmap::fromImage(image1);


        //给对应视角的图片形成标注形状
        for(int i=0;i<shapes.length();i++){
            if(shapes[i]->type==My::Shape3D::Rectangle3D){
                My::Rectangle3D* rectangle3d=dynamic_cast<My::Rectangle3D*>(shapes[i]);
                My::Rectangle* rectangle=new My::Rectangle();
                rectangle->isFill=rectangle3d->isFill;
                rectangle->isHover=rectangle3d->isHover;
                rectangle->isHide=rectangle3d->isHide;
                if((labels[0]->zindex>=rectangle3d->points[0].z) && (labels[0]->zindex<=(rectangle3d->points[0].z+rectangle3d->zspan))){
                    rectangle->points.append(QPointF(rectangle3d->points[0].x,rectangle3d->points[0].y));
                    rectangle->width=rectangle3d->width;
                    rectangle->height=rectangle3d->height;
                    rectangle->color=rectangle3d->color;
                    labels[0]->shapes.append(rectangle);

                }
                else{
                    labels[0]->shapes.append(rectangle);

                }
            }
            if(shapes[i]->type==My::Shape3D::Brush3D){
                My::Brush3D* brush3d=dynamic_cast<My::Brush3D*>(shapes[i]);
                My::Brush* brush=new My::Brush();
                brush->isFill=brush3d->isFill;
                brush->isHover=brush3d->isHover;
                brush->isHide=brush3d->isHide;
                for(int i=0;i<brush3d->points.length();i++){
                    cv::Point3f point3d=brush3d->points[i];
                    if(int(point3d.z)==labels[0]->zindex){
                        QPointF p(point3d.x,point3d.y);
                        brush->points.append(p);
                        brush->radius.append(brush3d->radius[i]);
                    }
                }
                if(brush->points.length()>0){
                    brush->color=brush3d->color;
                }
                labels[0]->shapes.append(brush);
            }
        }
    }
    if(dimension==1){
        cv::Mat mat2=cv::Mat(list.length(),list[0].cols,CV_8UC1);
        for(int i=0;i<mat2.rows;i++){
            for(int j=0;j<mat2.cols;j++){
                mat2.at<uchar>(i,j)=list[i].at<uchar>(labels[1]->zindex,j);
            }
        }
        QImage image2(mat2.data,mat2.cols,mat2.rows,static_cast<int>(mat2.step),QImage::Format_Grayscale8);
        *(labels[1]->pixmap)=QPixmap::fromImage(image2);

        for(int i=0;i<shapes.length();i++){
            if(shapes[i]->type==My::Shape3D::Rectangle3D){
                My::Rectangle3D* rectangle3d=dynamic_cast<My::Rectangle3D*>(shapes[i]);
                My::Rectangle* rectangle=new My::Rectangle();
                rectangle->isFill=rectangle3d->isFill;
                rectangle->isHover=rectangle3d->isHover;
                rectangle->isHide=rectangle3d->isHide;
                if(labels[1]->zindex>=(rectangle3d->points[0].y*list[0].rows)&&(labels[1]->zindex<=((rectangle3d->points[0].y+rectangle3d->height)*list[0].rows))){
                    rectangle->width=rectangle3d->width;
                    rectangle->height=float(rectangle3d->zspan)/float(list.length());
                    rectangle->points.append(QPointF(rectangle3d->points[0].x,rectangle3d->points[0].z/list.length()));
                    rectangle->color=rectangle3d->color;
                    labels[1]->shapes.append(rectangle);
                }
                else{
                    labels[1]->shapes.append(rectangle);
                }
             }
            if(shapes[i]->type==My::Shape3D::Brush3D){
                My::Brush3D* brush3d=dynamic_cast<My::Brush3D*>(shapes[i]);
                My::Brush* brush=new My::Brush();
                brush->isFill=brush3d->isFill;
                brush->isHover=brush3d->isHover;
                brush->isHide=brush3d->isHide;
                for(int i=0;i<brush3d->points.length();i++){
                    cv::Point3f point3d=brush3d->points[i];
                    if(int(point3d.y*list[0].rows)==labels[1]->zindex){
                        QPointF p(point3d.x,point3d.z/list.length());
                        brush->points.append(p);
                        brush->radius.append(brush3d->radius[i]);
                    }
                }
                if(brush->points.length()>0){
                    brush->color=brush3d->color;
                }
                labels[1]->shapes.append(brush);
            }
        }

    }
    if(dimension==2){
        cv::Mat mat3=cv::Mat(list.length(),list[0].rows,CV_8UC1);
        for(int i=0;i<mat3.rows;i++){
            for(int j=0;j<mat3.cols;j++){
                mat3.at<uchar>(i,j)=list[i].at<uchar>(list[0].rows-j,labels[2]->zindex);
            }
        }
        QImage image3(mat3.data,mat3.cols,mat3.rows,static_cast<int>(mat3.step),QImage::Format_Grayscale8);
        *(labels[2]->pixmap)=QPixmap::fromImage(image3);

        for(int i=0;i<shapes.length();i++){
            if(shapes[i]->type==My::Shape3D::Rectangle3D){
                My::Rectangle3D* rectangle3d=dynamic_cast<My::Rectangle3D*>(shapes[i]);
                My::Rectangle* rectangle=new My::Rectangle();
                rectangle->isFill=rectangle3d->isFill;
                rectangle->isHover=rectangle3d->isHover;
                rectangle->isHide=rectangle3d->isHide;
                if(labels[2]->zindex>=(rectangle3d->points[0].x*list[0].cols) && (labels[2]->zindex<=(rectangle3d->points[0].x+rectangle3d->width)*list[0].cols)){
                    rectangle->width=-rectangle3d->height;
                    rectangle->height=float(rectangle3d->zspan)/float(list.length());
                    rectangle->points.append(QPointF(1-(rectangle3d->points[0].y),rectangle3d->points[0].z/list.length()));
                    rectangle->color=rectangle3d->color;
                    labels[2]->shapes.append(rectangle);
                }
                else{
                    labels[2]->shapes.append(rectangle);
                }

            }
            if(shapes[i]->type==My::Shape3D::Brush3D){
                My::Brush3D* brush3d=dynamic_cast<My::Brush3D*>(shapes[i]);
                My::Brush* brush=new My::Brush();
                brush->isFill=brush3d->isFill;
                brush->isHover=brush3d->isHover;
                brush->isHide=brush3d->isHide;
                for(int i=0;i<brush3d->points.length();i++){
                    cv::Point3f point3d=brush3d->points[i];
                    if(int(point3d.x*list[0].cols)==labels[2]->zindex){
                        QPointF p(1-point3d.y,point3d.z/list.length());
                        brush->points.append(p);
                        brush->radius.append(brush3d->radius[i]);
                    }
                }
                if(brush->points.length()>0){
                    brush->color=brush3d->color;

                }
                labels[2]->shapes.append(brush);
            }
        }
    }
}


/// \brief 3d图片以及标注信息的保存写入
void My::Parse::write3D(const QString& fileName,QList<My::Shape3D*>& shapes){
    QFileInfo fileInfo(fileName);
    QString labelFileName=fileInfo.absoluteDir().absolutePath()+"/"+fileInfo.baseName()+"_3D.json";
    QFile labelfile(labelFileName);
    QJsonArray jsonArray;

    //形成json格式
    foreach(My::Shape3D* shape,shapes){
        QJsonObject obj;
        QJsonArray colors;
        QJsonArray points;
        colors.append(shape->color.red());
        colors.append(shape->color.green());
        colors.append(shape->color.blue());
        colors.append(shape->color.alpha());
        foreach(cv::Point3f p,shape->points){
            QJsonArray a;
            a.append(p.x);
            a.append(p.y);
            a.append(p.z);
            points.append(a);
        }
        obj.insert("color",colors);
        obj.insert("points",points);
        obj.insert("label",shape->label);
        if(shape->type==My::Shape3D::Brush3D){
            My::Brush3D* brush3d=dynamic_cast<My::Brush3D*>(shape);
            QJsonArray radius;
            obj.insert("type","Brush3D");
            for(int i=0;i<brush3d->radius.length();i++){
                    radius.append(brush3d->radius[i]);
            }

            obj.insert("radius",radius);
        }
        if(shape->type==My::Shape3D::Rectangle3D){
            My::Rectangle3D* rectangle3d=dynamic_cast<My::Rectangle3D*>(shape);
            obj.insert("type","Rectangle3D");
            obj.insert("width",rectangle3d->width);
            obj.insert("height",rectangle3d->height);
            obj.insert("zspan",rectangle3d->zspan);
        }
        jsonArray.append(obj);
    }

    //写入文件
    QJsonDocument doc(jsonArray);
    QByteArray ba=doc.toJson(QJsonDocument::Indented);
    if(!labelfile.open(QIODevice::WriteOnly)){
        qDebug()<<"file write error"<<endl;
    }
    labelfile.write(ba);
    labelfile.close();
}

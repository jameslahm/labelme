#include<QPainter>
#include"Rectangle.h"
#include<QDebug>
#include<math.h>

void My::Rectangle::draw(QWidget *w){
    if(isHide)return;
    QPainter painter(w);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(4);
    painter.setPen(pen);
    if(isFill || isHover)painter.setBrush(color);
    else painter.setBrush(Qt::NoBrush);
    if(points.length()==0)return;
    if(width>=0&&height>=0){
        painter.drawRect(int(points[0].x()*w->width()),int(points[0].y()*w->height()),int(width*w->width()),int(height*w->height()));
    }
    if(width<0&&height>=0){
        painter.drawRect(int(points[0].x()*w->width()+width* w->width()),int(points[0].y()*w->height()),int(abs(width*w->width())),int(height*w->height()));
    }
    if(width<0&&height<0){
        painter.drawRect(int(points[0].x()*w->width()+width* w->width()),int(points[0].y()*w->height()+height*w->height()),int(abs(width*w->width())),int(abs(height*w->height())));
    }
    if(width>=0&&height<0){
        painter.drawRect(int(points[0].x()*w->width()),int(points[0].y()*w->height()+height*w->height()),int(width*w->width()),int(abs(height*w->height())));
    }

}

bool My::Rectangle::isInShape(QPointF p,QWidget* w){

    if(points.length()==0)return false;
    std::vector<cv::Point2f> vec;

    vec.push_back(cv::Point2f(float(points[0].x())*w->width(),float(points[0].y())*w->height()));
    vec.push_back(cv::Point2f((float(points[0].x())+width)*w->width(),float(points[0].y())*w->height()));
    vec.push_back(cv::Point2f((float(points[0].x())+width)*w->width(),(float(points[0].y())+height)*w->height()));
    vec.push_back(cv::Point2f(float(points[0].x())*w->width(),(float(points[0].y())+height)*w->height()));


//    for(int i=0;i<4;i++){
//        qDebug()<<vec[i].x<<" "<<vec[i].y<<endl;
//    }

//    qDebug()<<p.x()<<" "<<w->width();
//    qDebug()<<p.y()<<" "<<w->height();

    cv::Point2f cvp(float(p.x())*w->width(),float(p.y())*w->height());

//    qDebug()<<cvp.x<<" "<<cvp.y;

    double res=cv::pointPolygonTest(vec,cvp,false);
    if(res<0)return false;
    else return true;
}

void My::Rectangle::offset(float xOffset,float yOffset){
    for(int i=0;i<points.length();i++){
        points[i].rx()+=double(xOffset);
        points[i].ry()+=double(yOffset);
    }
}

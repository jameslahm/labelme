/*!
* \file rectangle.cpp
* \brief 矩形形状类
* \author 王澳
* \date 2019.09
*/

#include<QPainter>
#include"Rectangle.h"
#include<QDebug>
#include<math.h>


/// \brief 绘制矩形
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


/// \brief 判断是否在矩形内部
bool My::Rectangle::isInShape(QPointF p,QWidget* w){

    if(points.length()==0)return false;
    std::vector<cv::Point2f> vec;

    vec.push_back(cv::Point2f(float(points[0].x())*w->width(),float(points[0].y())*w->height()));
    vec.push_back(cv::Point2f((float(points[0].x())+width)*w->width(),float(points[0].y())*w->height()));
    vec.push_back(cv::Point2f((float(points[0].x())+width)*w->width(),(float(points[0].y())+height)*w->height()));
    vec.push_back(cv::Point2f(float(points[0].x())*w->width(),(float(points[0].y())+height)*w->height()));

    cv::Point2f cvp(float(p.x())*w->width(),float(p.y())*w->height());

    double res=cv::pointPolygonTest(vec,cvp,false);
    if(res<0)return false;
    else return true;
}


/// \brief 偏移标注
void My::Rectangle::offset(float xOffset,float yOffset){
    for(int i=0;i<points.length();i++){
        points[i].rx()+=double(xOffset);
        points[i].ry()+=double(yOffset);
    }
}

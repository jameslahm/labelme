/*!
* \file brush.cpp
* \brief 画刷形状类，用于分割标注
* \author 王澳
* \date 2019.09
*/

#include"Brush.h"
#include<QPainter>
#include<vector>
#include<iostream>
#include<QDebug>


/// \brief 绘制画刷形状（分割标注）
void My::Brush::draw(QWidget* w){
    if(isHide)return;
    QPainter painter(w);
    painter.setPen(color);
    painter.setBrush(color);
    for(int i=0;i<points.length();i++){
        painter.drawEllipse(int(points[i].x()*w->width()),int(points[i].y()*w->height()),int(radius[i]*w->width()),int(radius[i]*w->width()));
    }
}


/// \brief 判断鼠标是否在分割标注内部
bool My::Brush::isInShape(QPointF p,QWidget* w){
    if(points.length()==0)return false;

    //发现能包围分割标注的最小矩形
    std::vector<cv::Point> vec;
    for(int i=0;i<points.length();i++){
        vec.push_back(cv::Point(int(points[i].x()*w->width()),int(points[i].y()*w->height())));
    }
    cv::RotatedRect rectPoint=cv::minAreaRect(vec);
    cv::Point2f fourPoints[4];
    rectPoint.points(fourPoints);
    cv::Point2f cvp(float(p.x())*w->width(),float(p.y())*w->height());

    //判断是否在内部
    std::vector<cv::Point2f> vec2;
    for(int i=0;i<4;i++){
        vec2.push_back(fourPoints[i]);
    }
    double res=cv::pointPolygonTest(vec2,cvp,true);
    if(res<0)return false;
    else return true;
}

/// \brief 标注形状偏移
void My::Brush::offset(float xOffset,float yOffset){
    for(int i=0;i<points.length();i++){
        points[i].rx()+=double(xOffset);
        points[i].ry()+=double(yOffset);
    }
}

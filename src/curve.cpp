/*!
* \file curve.cpp
* \brief 平滑曲线形状类
* \author 王澳
* \date 2019.09
*/

#include"Curve.h"
#include<QPainter>

/// \brief 绘制平滑曲线
void My::Curve::draw(QWidget* w){
    if(isHide)return;
    QPainter painter(w);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(4);
    painter.setPen(pen);
    QVector<QPoint> ps;
    for(int i=0;i<points.length();i++){
        ps.push_back(QPoint(int(points[i].x()*w->width()),int(points[i].y()*w->height())));
    }
    if(isHover || isFill){
        painter.setBrush(color);
        painter.drawPolygon(ps);
    }
    else{
        painter.drawPolyline(ps);
    }
}


/// \brief 判断是否在平滑曲线内部
bool My::Curve::isInShape(QPointF p, QWidget *w){
    std::vector<cv::Point> vec;
    for(int i=0;i<points.length();i++){
        vec.push_back(cv::Point(int(points[i].x()*w->width()),int(points[i].y()*w->height())));
    }
    cv::Point2f cvp(float(p.x())*w->width(),float(p.y())*w->height());
    double res=cv::pointPolygonTest(vec,cvp,true);
    if(res<0)return false;
    else return true;
}

/// \brief 偏移标注
void My::Curve::offset(float xOffset, float yOffset){
    for(int i=0;i<points.length();i++){
        points[i].rx()+=double(xOffset);
        points[i].ry()+=double(yOffset);
    }
}

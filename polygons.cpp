#include"Polygons.h"
#include<QPainter>
#include<QVector>

void My::Polygons::draw(QWidget *w){
    if(isHide)return;
    QPainter painter(w);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(4);
    painter.setPen(pen);
    if(isFill || isHover)painter.setBrush(color);
    else painter.setBrush(Qt::NoBrush);
    QVector<QPoint> ps;
    for(int i=0;i<points.length();i++){
        ps.push_back(QPoint(int(points[i].x()*w->width()),int(points[i].y()*w->height())));
    }
    painter.drawPolygon(ps);
}

bool My::Polygons::isInShape(QPointF p,QWidget* w){
    std::vector<cv::Point> vec;
    for(int i=0;i<points.length();i++){
        vec.push_back(cv::Point(int(points[i].x()*w->width()),int(points[i].y()*w->height())));
    }
    cv::Point2f cvp(float(p.x())*w->width(),float(p.y())*w->height());
    double res=cv::pointPolygonTest(vec,cvp,true);
    if(res<0)return false;
    else return true;
}

void My::Polygons::offset(float xOffset,float yOffset){
    for(int i=0;i<points.length();i++){
        points[i].rx()+=double(xOffset);
        points[i].ry()+=double(yOffset);
    }
}

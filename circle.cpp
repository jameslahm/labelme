#include"Circle.h"
#include<QPainter>
#include<math.h>


void My::Circle::draw(QWidget* w){
    if(isHide)return;
    QPainter painter(w);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(4);
    painter.setPen(pen);
    if(isFill || isHover)painter.setBrush(color);
    else painter.setBrush(Qt::NoBrush);
    foreach(QPointF point,points){
        painter.drawEllipse(int(point.x()*w->width()-radius*w->width()),int(point.y()*w->height()-radius*w->width()),2*int(radius*w->width()),2*int(radius*w->width()));
    }
}

bool My::Circle::isInShape(QPointF p,QWidget* w){
    int distance=int(sqrt(pow(points[0].x()*w->width()-p.x()*w->width(),2)+pow(points[0].y()*w->height()-p.y()*w->height(),2)));
    if(distance<radius*w->width())return true;
    else  return false;
}

void My::Circle::offset(float xOffset,float yOffset){
    for(int i=0;i<points.length();i++){
        points[i].rx()+=double(xOffset);
        points[i].ry()+=double(yOffset);
    }
}

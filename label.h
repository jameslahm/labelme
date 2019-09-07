#ifndef LABEL_H
#define LABEL_H
#include"Namespace.h"
#include<QLabel>
#include<QWidget>
#include"CentralWinit.h"


class My::Label:public QLabel{
    Q_OBJECT


    int flag;//标志位，用来撤销以及恢复动作

    float cursorX=0;
    float cursorY=0;
    QPixmap magnifierArea;


public:
    QColor color;
    int current=-1;
    QPixmap* pixmap;
    float size=float(0.01);
    float zoomLevel=1.0;
    bool MagniFier=false;
    CentralWInit* manager;
public:
signals:

public:
    float XOffsetSum=0;
    float YOffsetSum=0;
    QList<My::Shape*> shapes;
    My::shapeStatus status=My::shapeStatus::NoShape;
    Label(QWidget* parent):QLabel(parent){
        color.setRgb(100,255,0,100);
        setMouseTracking(true);
        pixmap=new QPixmap();
        setAlignment(Qt::AlignCenter);
        magnifierArea.load("C:/Users/13664/Desktop/2.jpg");

    }
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    ~Label(){
        delete pixmap;
    }

};

#endif // LABEL_H

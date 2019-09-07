#ifndef LABEL3D_H
#define LABEL3D_H
#include "Namespace.h"
#include<QLabel>
#include"centralwinit3d.h"


class My::Label3D:public QLabel{
    Q_OBJECT

    QPixmap magnifierArea;
public:
    float xOffset=0;
    float yOffset=0;

    float XOffsetSum=0;
    float YOffsetSum=0;

    float cursorX=0;
    float cursorY=0;

    int realLength;

    QPixmap* pixmap;
    int viewAngle;
    QList<My::Shape*> shapes;
    float zoomLevel=1.0;
    int zindex=0;
//    int current=-1;
    bool MagniFier=false;
    CentralWInit3D* manager;

    bool isFinished=false;
//    My::shapeStatus status=My::shapeStatus::NoShape;

    Label3D(QWidget* parent,int v):QLabel(parent),viewAngle(v){
        setMouseTracking(true);
        pixmap=new QPixmap();
        setAlignment(Qt::AlignCenter);
        magnifierArea.load("C:/Users/13664/Desktop/2.jpg");
    }

    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    ~Label3D();
};

#endif // LABEL3D_H

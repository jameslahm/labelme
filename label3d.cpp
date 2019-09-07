#include"Label3D.h"
#include<QPainter>
#include"Shape.h"
#include"Rectangle.h"
#include"Brush.h"
#include"brush3d.h"
#include"rectangle3d.h"
#include"shape3d.h"
#include"mainwindow.h"
#include<QDebug>
#include"command3d.h"

void My::Label3D::paintEvent(QPaintEvent* event){
    if(pixmap->isNull()){return;}
    QPixmap tempPixMap(*pixmap);

    if(viewAngle==1 || viewAngle==2){
        realLength=tempPixMap.height()*3;
        tempPixMap=tempPixMap.scaled(tempPixMap.width(),realLength);
    }

    tempPixMap=tempPixMap.scaled(tempPixMap.width()*zoomLevel,tempPixMap.height()*zoomLevel,Qt::KeepAspectRatio);

    this->resize(tempPixMap.width(),tempPixMap.height());
    //    this->resize(tempPixMap.width(),tempPixMap.height());
    QPainter painter(this);
    painter.drawPixmap(0,0,tempPixMap.width(),tempPixMap.height(),tempPixMap);
    foreach(My::Shape* shape,shapes){
        if(shape!=nullptr) shape->draw(this);
    }
    if(MagniFier){
        QPainter painter(this);
        painter.drawPixmap(this->width()-100,this->height()-100,100,100,magnifierArea);
    }
}

void My::Label3D::mousePressEvent(QMouseEvent* event){
//    manager->Focus=viewAngle;
    float x=event->x();
    float y=event->y();
    cursorX=x;
    cursorY=y;
    QPointF p(qreal(x/this->width()),qreal(y/this->height()));
    if(event->buttons()&Qt::RightButton){manager->Focus=viewAngle;return;}
    if(manager->status==My::NoShape){
        if(manager->current==-1)manager->Focus=viewAngle;
        else{
//            manager->selectedChanged(manager->current,false);
            manager->sync(viewAngle,My::UnSelected);
            manager->current=-1;
        }
        manager->Focus=viewAngle;
        return;
    }
    if(manager->status==My::InShape){
        int index=shapes.length();
        for(int i=0;i<shapes.length();i++){
            if(shapes[i]->isInShape(p,this)){
                index=i;
                break;
            }
        }
        if(index<shapes.length()){
            manager->current=index;
            manager->sync(viewAngle,My::Selected);
        }
        setCursor(Qt::ClosedHandCursor);
        manager->Focus=viewAngle;
        return;
    }
    if(manager->status==My::BrushShape){
        if(manager->current==-1){
            My::Brush* brush=new My::Brush();
            brush->radius.append(manager->size);
            brush->color=manager->color;
            brush->points.append(p);
            shapes.append(brush);
            manager->current=shapes.length()-1;
            manager->sync(viewAngle,My::AddShape);
        }
        else{
            My::Brush* brush=dynamic_cast<My::Brush*>(shapes[manager->current]);
            brush->radius.append(manager->size);
            brush->points.append(p);
            manager->sync(viewAngle,My::AddPoint);
        }
        manager->Focus=viewAngle;
        return;
    }
    if(manager->status==My::RectangleShape){
        if(manager->current==-1){
            My::Rectangle* rectangle=new My::Rectangle();
            rectangle->points.append(p);
            rectangle->color=manager->color;
            shapes.append(rectangle);
            manager->current=shapes.length()-1;
            manager->sync(viewAngle,My::AddShape);
        }
        else{
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(shapes[manager->current]);
            if(rectangle->points.length()>0){
                if(isFinished)return;
                rectangle->color=manager->color;
                rectangle->width=(x-rectangle->points[0].x()*this->width())/this->width();
                rectangle->height=(y-rectangle->points[0].y()*this->height())/this->height();
                manager->sync(viewAngle,My::ChangePoint);
                isFinished=true;
             }
        }
        manager->Focus=viewAngle;
        return;
    }
//    qDebug()<<"Focus "<<manager->Focus<<endl;
}
void My::Label3D::mouseMoveEvent(QMouseEvent* event){
    float x=event->x();
    float y=event->y();
    int index=shapes.length();
    QPointF p(qreal(x/this->width()),qreal(y/this->height()));
    manager->sync(viewAngle,My::ShowMessage);

    if(MagniFier){
        QPixmap tempPixMap;
        tempPixMap=tempPixMap.grabWidget(this,int(x),int(y),50,50);
        magnifierArea=tempPixMap.scaled(100,100,Qt::KeepAspectRatio);
        qDebug()<<"success"<<endl;
        update();
    }

    if(manager->status==My::NoShape){
        qDebug()<<"NoShape"<<endl;
        qDebug()<<manager->current<<endl;
        cursorX=x;
        cursorY=y;
        for(int i=0;i<shapes.length();i++){
            if(shapes[i]->isInShape(p,this)){
                index=i;
                break;
            }
        }
        if(index<shapes.length()){
            setCursor(Qt::OpenHandCursor);
            manager->status=My::InShape;
            manager->current=index;
            manager->sync(viewAngle,My::Hovered);
            return;
        }
        setCursor(Qt::ArrowCursor);
        return;
    }
    if(manager->status==My::InShape){
        qDebug()<<"InShape"<<endl;
        qDebug()<<manager->current<<endl;
        if(event->buttons()&Qt::LeftButton){
            xOffset=(x-cursorX)/this->width();
            yOffset=(y-cursorY)/this->height();

            XOffsetSum+=xOffset;
            YOffsetSum+=yOffset;

            cursorX=x;
            cursorY=y;

            manager->sync(viewAngle,My::Offset);
            return;
        }
        for(int i=0;i<shapes.length();i++){
            if(shapes[i]->isInShape(p,this)){
                index=i;
                break;
            }
        }

        if(index<shapes.length()){
            manager->current=index;
            manager->sync(viewAngle,My::Hovered);
        }

        if(index>=shapes.length()){
            manager->status=My::NoShape;
            setCursor(Qt::ArrowCursor);
//            if(current!=-1)shapes[current]->isHover=false;
            manager->sync(viewAngle,My::UnHovered);
            return;
        }
        cursorX=x;
        cursorY=y;
        return;
    }
    if(manager->status==My::BrushShape){
        qDebug()<<"BrushShape"<<endl;
        qDebug()<<manager->current<<endl;
        cursorX=x;
        cursorY=y;
        if(event->buttons()&Qt::LeftButton){
            My::Brush* brush=dynamic_cast<My::Brush*>(shapes[manager->current]);
            brush->color=manager->color;
            brush->radius.append(manager->size);
            brush->points.append(p);
            manager->sync(viewAngle,My::AddPoint);
        }
        return;
    }
    if(manager->status==My::RectangleShape){
        qDebug()<<"RectangleShape"<<endl;
        qDebug()<<manager->current<<endl;
        cursorX=x;
        cursorY=y;
        if(isFinished==true) return;
        if(manager->current!=-1){
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(shapes[manager->current]);
            if(rectangle->points.length()==0)return;
            rectangle->color=manager->color;
            rectangle->width=(x-rectangle->points[0].x()*this->width())/this->width();
            rectangle->height=(y-rectangle->points[0].y()*this->height())/this->height();
            manager->sync(viewAngle,My::ChangePoint);
        }
        return;
    }

}
void My::Label3D::mouseReleaseEvent(QMouseEvent* event){
    if(manager->status==My::NoShape){
        return;
    }
    if(manager->status==My::InShape){
        setCursor(Qt::OpenHandCursor);
        if(event->button()==Qt::LeftButton){
            manager->command3d->logMove(manager->current,viewAngle,XOffsetSum,YOffsetSum);
            XOffsetSum=0;
            YOffsetSum=0;
        }
        return;
    }
    if(manager->status==My::BrushShape){
        return;
    }
    if(manager->status==My::RectangleShape){
        return;
    }
    if(manager->status==My::PolygonsShape){
        return;
    }
    if(manager->status==My::CircleShape){
        return;
    }
    if(manager->status==My::CurveShape){
        return;
    }
}

My::Label3D::~Label3D(){
    delete pixmap;
    for(int i=0;i<shapes.length();i++){
        delete shapes[i];
    }
    shapes.clear();
}

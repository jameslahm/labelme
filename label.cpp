#include"label.h"
#include"Shape.h"
#include"Rectangle.h"
#include"Brush.h"
#include"Circle.h"
#include<Polygons.h>
#include"Curve.h"
#include"Shape.h"
#include<QPainter>
#include<QInputDialog>
#include"mainwindow.h"
#include"command2d.h"



void My::Label::paintEvent(QPaintEvent *event){
    if(pixmap->isNull()){return;}
    QPixmap tempPixMap(*pixmap);
    tempPixMap=tempPixMap.scaled(tempPixMap.width()*zoomLevel,tempPixMap.height()*zoomLevel,Qt::KeepAspectRatio);
    this->resize(tempPixMap.width(),tempPixMap.height());
    QPainter painter(this);
    painter.drawPixmap(0,0,tempPixMap.width(),tempPixMap.height(),tempPixMap);
//    painter.drawRect(100,100,100,0);
//    if(MagniFier){
//        QPixmap magnifierArea=QPixmap::grabWidget(this->parent(),QCursor::pos().x(),QCursor::pos().y(),10,10);
//        magnifierArea.scaled(100,100,Qt::KeepAspectRatio);
//        painter.drawPixmap(this->width()-100,this->height()-100,100,100,magnifierArea);
//    }
    foreach(My::Shape* shape,shapes){
        shape->draw(this);
    }
    if(MagniFier){
        QPainter painter(this);
        painter.drawPixmap(this->width()-100,this->height()-100,100,100,magnifierArea);
    }
//    QPainter painter(this);
//    foreach(My::Shape* shape,shapes){
//        shape->draw(&tempPixMap);
//    }
}

void My::Label::mousePressEvent(QMouseEvent *event){
    float x=event->x();
    float y=event->y();
    cursorX=x;
    cursorY=y;
    QPointF p(qreal(x/this->width()),qreal(y/this->height()));
    if(event->buttons()&Qt::RightButton)return;
    if(status==My::NoShape){
        if(current==-1)return;
        else{
            emit(manager->selectedChanged(current,false));
            current=-1;
        }
        return;
    }
    if(status==My::InShape){
        int index=shapes.length();
        for(int i=0;i<shapes.length();i++){
            if(shapes[i]->isInShape(p,this)){
                index=i;
                break;
            }
        }
        if(index<shapes.length()){
            current=index;
        }
        setCursor(Qt::ClosedHandCursor);
        emit(manager->selectedChanged(current,true));
    }
    if(status==My::BrushShape){
        if(current==-1){
            My::Brush* brush=new My::Brush();
            brush->radius.append(size);
            brush->color=color;
            brush->points.append(p);
            shapes.append(brush);
            current=shapes.length()-1;
            update();
        }
        else{
            My::Brush* brush=dynamic_cast<My::Brush*>(shapes[current]);
            brush->radius.append(size);
            brush->points.append(p);
            update();
        }
        return;
    }
    if(status==My::RectangleShape){
        if(current==-1){
            My::Rectangle* rectangle=new My::Rectangle();
            rectangle->points.append(p);
            rectangle->color=color;
            shapes.append(rectangle);
            current=shapes.length()-1;
        }
        else{
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(shapes[current]);
            rectangle->width=(x-this->width()*rectangle->points[0].x())/this->width();
            rectangle->height=(y-this->height()*rectangle->points[0].y())/this->height();
            update();
            bool isOk;
            QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,"",&isOk);
            if(isOk){
                rectangle->label=text;
                manager->command->logAdd(current);

                emit(manager->labelAdded(rectangle,current));
                current=-1;
            }
        }
        return;
    }
    if(status==My::PolygonsShape){
        if(current==-1){
            My::Polygons* polygons=new My::Polygons();
            polygons->points.append(p);
            polygons->points.append(p);
            polygons->color=color;
            shapes.append(polygons);
            current=shapes.length()-1;
        }
        else{
            My::Polygons* polygons=dynamic_cast<My::Polygons*>(shapes[current]);
            polygons->points.append(p);
            polygons->points.append(p);
            update();
        }
        return;
    }
    if(status==My::CircleShape){
        if(current==-1){
            My::Circle* circle=new My::Circle();
            circle->color=color;
            circle->points.append(p);
            shapes.append(circle);
            current=shapes.length()-1;
        }
        else{
            My::Circle* circle=dynamic_cast<My::Circle*>(shapes[current]);
            circle->radius=sqrt(pow(x-circle->points[0].x()*this->width(),2)+pow(y-circle->points[0].y()*this->height(),2))/this->width();
            update();
            bool isOk;
            QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,"",&isOk);
            if(isOk){
                circle->label=text;
                manager->command->logAdd(current);
                emit(manager->labelAdded(circle,current));
                current=-1;
            }
        }
        return;
    }
    if(status==My::CurveShape){
        if(current==-1){
            My::Curve* curve=new My::Curve();
            curve->color=color;
            curve->points.append(p);
            shapes.append(curve);
            current=shapes.length()-1;
        }
        else{
            My::Curve* curve=dynamic_cast<My::Curve*>(shapes[current]);
            curve->points.append(p);
            update();
            bool isOk;
            QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,"",&isOk);
            if(isOk){
                curve->label=text;
                manager->command->logAdd(current);

                emit(manager->labelAdded(curve,current));
                current=-1;
            }
        }
        return;
    }


}

void My::Label::mouseMoveEvent(QMouseEvent *event){

    MainWindow* w=qobject_cast<MainWindow*>(manager->parent());


    float x=event->x();
    float y=event->y();
    int index=shapes.length();
    QPointF p(qreal(x/this->width()),qreal(y/this->height()));

    /*MainWindow* */w=qobject_cast<MainWindow*>(manager->parent());
    w->statusBar()->showMessage(QString("Pos: X %1 Y %2").arg(x).arg(y),1000);

    if(MagniFier){
        QPixmap tempPixMap;
        tempPixMap=tempPixMap.grabWidget(this,int(x),int(y),50,50);
        magnifierArea=tempPixMap.scaled(100,100,Qt::KeepAspectRatio);
        qDebug()<<"success"<<endl;
        update();
    }

    if(status==My::NoShape){
        qDebug()<<"NoShape"<<endl;
        qDebug()<<current<<endl;
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
            status=My::InShape;
            current=index;
            shapes[current]->isHover=true;
            update();
            return;
        }
        setCursor(Qt::ArrowCursor);
        return;
    }
    if(status==My::InShape){
        qDebug()<<"InShape"<<endl;
        qDebug()<<current<<endl;
        if(event->buttons()&Qt::LeftButton){
            float xOffset=(x-cursorX)/this->width();
            float yOffset=(y-cursorY)/this->height();

            XOffsetSum+=xOffset;
            YOffsetSum+=yOffset;

            cursorX=x;
            cursorY=y;
            shapes[current]->offset(xOffset,yOffset);
            update();
            return;
        }
        for(int i=0;i<shapes.length();i++){
            if(shapes[i]->isInShape(p,this)){
                index=i;
                break;
            }
        }

        if(index<shapes.length()){
            current=index;
            shapes[current]->isHover=true;
            update();
        }

        if(index>=shapes.length()){
            status=My::NoShape;
            setCursor(Qt::ArrowCursor);
            if(current!=-1)shapes[current]->isHover=false;
            update();
            return;
        }
        cursorX=x;
        cursorY=y;
        return;

    }
    if(status==My::BrushShape){
        qDebug()<<"BrushShape"<<endl;
        qDebug()<<current<<endl;
        cursorX=x;
        cursorY=y;
        if(event->buttons()&Qt::LeftButton){
            My::Brush* brush=dynamic_cast<My::Brush*>(shapes[current]);
            brush->color=color;
            brush->radius.append(size);
            brush->points.append(p);
            update();
        }
        return;
    }
    if(status==My::RectangleShape){
        qDebug()<<"RectangleShape"<<endl;
        qDebug()<<current<<endl;
        cursorX=x;
        cursorY=y;
        if(current!=-1){
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(shapes[current]);
            rectangle->color=color;
            rectangle->width=(x-rectangle->points[0].x()*this->width())/this->width();
            rectangle->height=(y-rectangle->points[0].y()*this->height())/this->height();
            update();
        }
        return;
    }
    if(status==My::PolygonsShape){
        qDebug()<<"PolygonsShape"<<endl;
        qDebug()<<current<<endl;
        cursorX=x;
        cursorY=y;
        if(current!=-1){
            shapes[current]->points.pop_back();
            shapes[current]->points.push_back(p);
            update();
        }
        return;
    }
    if(status==My::CircleShape){
        qDebug()<<"CircleShape"<<endl;
        qDebug()<<current<<endl;
        cursorX=x;
        cursorY=y;
        if(current!=-1){
            My::Circle* circle=dynamic_cast<My::Circle*>(shapes[current]);
            circle->radius=sqrt(pow(x-circle->points[0].x()*this->width(),2)+pow(y-circle->points[0].y()*this->height(),2))/this->width();
            update();
        }
        return;
    }
    if(status==My::CurveShape){
        qDebug()<<"CurveShape"<<endl;
        qDebug()<<current<<endl;
        cursorX=x;
        cursorY=y;
        if(current!=-1){
           My::Curve* curve=dynamic_cast<My::Curve*>(shapes[current]);
           curve->points.append(p);
           update();
        }
        return;
    }
}

void My::Label::mouseReleaseEvent(QMouseEvent *event){
    if(status==My::NoShape){
        return;
    }
    if(status==My::InShape){
        setCursor(Qt::OpenHandCursor);
        if(event->button()==Qt::LeftButton){
            manager->command->logMove(current,XOffsetSum,YOffsetSum);
            XOffsetSum=0;
            YOffsetSum=0;
        }
        return;
    }
    if(status==My::BrushShape){
        return;
    }
    if(status==My::RectangleShape){
        return;
    }
    if(status==My::PolygonsShape){
        return;
    }
    if(status==My::CircleShape){
        return;
    }
    if(status==My::CurveShape){
        return;
    }
}




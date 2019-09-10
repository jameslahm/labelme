/*!
* \file label.cpp
* \brief 2d绘制图片及标注类
* \author 王澳
* \date 2019.09
*/

#include<QDebug>
#include<QPainter>
#include<QInputDialog>
#include"mainwindow.h"
#include"Label.h"
#include"Shape2D.h"
#include"Rectangle.h"
#include"Brush.h"
#include"Circle.h"
#include"Polygons.h"
#include"Curve.h"
#include"Command2D.h"


/// \brief 重写绘制事件的处理
void My::Label::paintEvent(QPaintEvent *event){

    //空图片则返回
    if(pixmap->isNull()){return;}

    //根据放大倍数进行放大
    QPixmap tempPixMap(*pixmap);
    tempPixMap=tempPixMap.scaled(tempPixMap.width()*zoomLevel,tempPixMap.height()*zoomLevel,Qt::KeepAspectRatio);

    //label进行响应的大小调整
    this->resize(tempPixMap.width(),tempPixMap.height());

    //绘制图片
    QPainter painter(this);
    painter.drawPixmap(0,0,tempPixMap.width(),tempPixMap.height(),tempPixMap);

    //绘制形状
    foreach(My::Shape2D* shape,shapes){
        shape->draw(this);
    }

    //根据是否开启放大镜，进行绘制
    if(MagniFier){
        QPainter painter(this);
        painter.drawPixmap(this->width()-100,this->height()-100,100,100,magnifierArea);
    }
}

/// \brief 重写鼠标移动事件的处理
void My::Label::mousePressEvent(QMouseEvent *event){

    //更新当前鼠标所在位置
    float x=event->x();
    float y=event->y();
    cursorX=x;
    cursorY=y;

    //更新鼠标点位，该点位存储横纵坐标比例
    QPointF p(qreal(x/this->width()),qreal(y/this->height()));

    //右键则直接返回，右键会弹出菜单，所以此处不进行处理
    if(event->buttons()&Qt::RightButton)return;

    //若为noshape状态
    if(status==My::NoShape){
        if(current==-1)return;
        else{

            //当前current不为-1，则发送信号
            emit(manager->selectedChanged(current,false));
            current=-1;
        }
        return;
    }

    //若为inshape状态，即鼠标在标注形状内部
    if(status==My::InShape){

        //判断是否仍在标注形状内部，若在则更新current
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

        //设置鼠标样式
        setCursor(Qt::ClosedHandCursor);
        emit(manager->selectedChanged(current,true));
    }

    //若为创建画刷形状状态（分割标注）
    if(status==My::BrushShape){

        //若还未添加形状则添加
        if(current==-1){
            My::Brush* brush=new My::Brush();
            brush->radius.append(size);
            brush->color=color;
            brush->points.append(p);
            shapes.append(brush);
            current=shapes.length()-1;
            update();
        }

        //若已添加，则直接添加像素点
        else{
            My::Brush* brush=dynamic_cast<My::Brush*>(shapes[current]);
            brush->radius.append(size);
            brush->points.append(p);
            update();
        }
        return;
    }

    //若为创建矩形状态
    if(status==My::RectangleShape){

        if(current==-1){
            My::Rectangle* rectangle=new My::Rectangle();
            rectangle->points.append(p);
            rectangle->color=color;
            shapes.append(rectangle);
            current=shapes.length()-1;
        }

        //若已添加形状，则根据鼠标所在位置更新矩形位置，且询问是否添加
        else{
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(shapes[current]);
            rectangle->width=(x-this->width()*rectangle->points[0].x())/this->width();
            rectangle->height=(y-this->height()*rectangle->points[0].y())/this->height();
            update();

            //询问是否添加
            bool isOk;
            QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,"",&isOk);
            if(isOk){
                rectangle->label=text;

                //命令栈记录
                manager->command->logAdd(current);

                //发送信号
                emit(manager->labelAdded(rectangle,current));
                current=-1;
            }
        }
        return;
    }

    //若为创建多边形
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

    //若为创建圆形
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

    //若为创建平滑曲线
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

/// \brief 重写鼠标移动事件的处理
void My::Label::mouseMoveEvent(QMouseEvent *event){

    //状态栏显示坐标
    MainWindow* w=qobject_cast<MainWindow*>(manager->parent());

    //获取更新鼠标点位
    float x=event->x();
    float y=event->y();
    int index=shapes.length();
    QPointF p(qreal(x/this->width()),qreal(y/this->height()));

    /*MainWindow* */w=qobject_cast<MainWindow*>(manager->parent());
    w->statusBar()->showMessage(QString("Pos: X %1 Y %2").arg(x).arg(y),1000);

    //是否有放大镜，若有则会实时获取鼠标当前的图片区域
    if(MagniFier){
        QPixmap tempPixMap;
        tempPixMap=tempPixMap.grabWidget(this,int(x),int(y),20,20);
        magnifierArea=tempPixMap.scaled(100,100,Qt::KeepAspectRatio);
        qDebug()<<"success"<<endl;
        update();
    }

    //noshape状态
    if(status==My::NoShape){

        //更新鼠标位置
        cursorX=x;
        cursorY=y;

        //判断是否鼠标在标注形状内，并更新
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

    //inshape状态
    if(status==My::InShape){

        //若为左键，则为移动标注形状的位置
        if(event->buttons()&Qt::LeftButton){
            float xOffset=(x-cursorX)/this->width();
            float yOffset=(y-cursorY)/this->height();

            //更新总偏移量
            XOffsetSum+=xOffset;
            YOffsetSum+=yOffset;

            cursorX=x;
            cursorY=y;

            //标注形状进行偏移
            shapes[current]->offset(xOffset,yOffset);
            update();
            return;
        }

        //判断是否鼠标是否在标注内并更新
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

    //创建画刷状态
    if(status==My::BrushShape){

        cursorX=x;
        cursorY=y;

        //左键则添加像素点
        if(event->buttons()&Qt::LeftButton){
            My::Brush* brush=dynamic_cast<My::Brush*>(shapes[current]);
            brush->color=color;
            brush->radius.append(size);
            brush->points.append(p);
            update();
        }
        return;
    }

    //创建矩形状态
    if(status==My::RectangleShape){

        cursorX=x;
        cursorY=y;

        //同步更新矩形的位置
        if(current!=-1){
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(shapes[current]);
            rectangle->color=color;
            rectangle->width=(x-rectangle->points[0].x()*this->width())/this->width();
            rectangle->height=(y-rectangle->points[0].y()*this->height())/this->height();
            update();
        }
        return;
    }

    //创建多边形状态
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

    //创建圆
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

    //创建平滑曲线状态
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

/// \brief 重写释放鼠标事件的处理
void My::Label::mouseReleaseEvent(QMouseEvent *event){
    if(status==My::NoShape){
        return;
    }
    if(status==My::InShape){
        setCursor(Qt::OpenHandCursor);

        //命令栈记录总偏移量
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




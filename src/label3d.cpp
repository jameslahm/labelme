/*!
* \file label3d.cpp
* \brief 3d绘制图片及标注类
* \author 王澳
* \date 2019.09
*/

#include<QPainter>
#include<QDebug>
#include"Shape2D.h"

#include"Label3D.h"
#include"Rectangle.h"
#include"Brush.h"
#include"Brush3D.h"
#include"Rectangle3D.h"
#include"Shape3D.h"
#include"mainwindow.h"
#include"Command3D.h"

/// \brief 重写绘制事件的处理
void My::Label3D::paintEvent(QPaintEvent* event){

    //空图片返回
    if(pixmap->isNull()){return;}
    QPixmap tempPixMap(*pixmap);

    //视角为1和2，这里为了图片显示效果，默认高度放大三倍
    if(viewAngle==1 || viewAngle==2){
        realLength=tempPixMap.height()*3;
        tempPixMap=tempPixMap.scaled(tempPixMap.width(),realLength);
    }

    //根据放大倍数进行放大
    tempPixMap=tempPixMap.scaled(tempPixMap.width()*zoomLevel,tempPixMap.height()*zoomLevel,Qt::KeepAspectRatio);

    //调整label大小
    this->resize(tempPixMap.width(),tempPixMap.height());

    //绘制标注形状
    QPainter painter(this);
    painter.drawPixmap(0,0,tempPixMap.width(),tempPixMap.height(),tempPixMap);

    //绘制标注
    foreach(My::Shape2D* shape,shapes){
        shape->draw(this);
    }

    //放大镜绘制
    if(MagniFier){
        QPainter painter(this);
        painter.drawPixmap(this->width()-100,this->height()-100,100,100,magnifierArea);
    }
}

/// \brief 重写鼠标移动事件的处理
void My::Label3D::mousePressEvent(QMouseEvent* event){

    //更新鼠标位置
    float x=event->x();
    float y=event->y();
    cursorX=x;
    cursorY=y;
    QPointF p(qreal(x/this->width()),qreal(y/this->height()));

    //右键返回
    if(event->buttons()&Qt::RightButton){manager->Focus=viewAngle;return;}

    //noshape状态
    if(manager->status==My::NoShape){
        if(manager->current==-1)manager->Focus=viewAngle;
        else{

            //同步
            manager->sync(viewAngle,My::UnSelected);
            manager->current=-1;
        }
        manager->Focus=viewAngle;
        return;
    }

    //inshape状态
    if(manager->status==My::InShape){
        int index=shapes.length();

        //判断是否鼠标在标注形状内，并同步
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

    //创建画刷
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

            //同步
            manager->sync(viewAngle,My::AddPoint);
        }
        manager->Focus=viewAngle;
        return;
    }

    //创建矩形
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

            //矩形这里比较特殊，因为要保证在三个视角能分别绘制或者调整矩形，isFinished变量用来标记当前视角是否形状已经创建完毕或者调整完毕，
            //若完毕，则在该视角上不会进行矩形相应的更改
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(shapes[manager->current]);
            if(rectangle->points.length()>0){
                if(isFinished)return;
                rectangle->color=manager->color;
                rectangle->width=(x-rectangle->points[0].x()*this->width())/this->width();
                rectangle->height=(y-rectangle->points[0].y()*this->height())/this->height();

                //同步更改
                manager->sync(viewAngle,My::ChangePoint);
                isFinished=true;
             }
        }
        manager->Focus=viewAngle;
        return;
    }

}

/// \brief 重写鼠标移动事件的处理
void My::Label3D::mouseMoveEvent(QMouseEvent* event){

    //更新点位
    float x=event->x();
    float y=event->y();
    int index=shapes.length();
    QPointF p(qreal(x/this->width()),qreal(y/this->height()));

    //状态栏显示坐标
    manager->sync(viewAngle,My::ShowMessage);

    //放大镜操作
    if(MagniFier){
        QPixmap tempPixMap;
        tempPixMap=tempPixMap.grabWidget(this,int(x),int(y),20,20);
        magnifierArea=tempPixMap.scaled(100,100,Qt::KeepAspectRatio);
        update();
    }

    //noshape状态
    if(manager->status==My::NoShape){

        cursorX=x;
        cursorY=y;

        //判断鼠标是否在当前标注形状内并更新
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

            //同步
            manager->sync(viewAngle,My::Hovered);
            return;
        }
        setCursor(Qt::ArrowCursor);
        return;
    }

    //inshape状态
    if(manager->status==My::InShape){
        if(event->buttons()&Qt::LeftButton){
            xOffset=(x-cursorX)/this->width();
            yOffset=(y-cursorY)/this->height();

            //更新总偏移量
            XOffsetSum+=xOffset;
            YOffsetSum+=yOffset;

            cursorX=x;
            cursorY=y;

            //偏移
            manager->sync(viewAngle,My::Offset);
            return;
        }

        //判断鼠标是否在标注形状内并更新
        for(int i=0;i<shapes.length();i++){
            if(shapes[i]->isInShape(p,this)){
                index=i;
                break;
            }
        }

        if(index<shapes.length()){
            manager->current=index;

            //同步
            manager->sync(viewAngle,My::Hovered);
        }

        if(index>=shapes.length()){
            manager->status=My::NoShape;
            setCursor(Qt::ArrowCursor);

            //同步
            manager->sync(viewAngle,My::UnHovered);
            return;
        }
        cursorX=x;
        cursorY=y;
        return;
    }

    //创建画刷形状状态
    if(manager->status==My::BrushShape){

        cursorX=x;
        cursorY=y;

        //添加像素点
        if(event->buttons()&Qt::LeftButton){
            My::Brush* brush=dynamic_cast<My::Brush*>(shapes[manager->current]);
            brush->color=manager->color;
            brush->radius.append(manager->size);
            brush->points.append(p);

            manager->sync(viewAngle,My::AddPoint);
        }
        return;
    }

    //创建矩形状态
    if(manager->status==My::RectangleShape){

        cursorX=x;
        cursorY=y;

        //若完毕，则直接返回
        if(isFinished==true) return;

        //更新矩形形状
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

/// \brief 重写释放鼠标事件的处理
void My::Label3D::mouseReleaseEvent(QMouseEvent* event){
    if(manager->status==My::NoShape){
        return;
    }
    if(manager->status==My::InShape){
        setCursor(Qt::OpenHandCursor);

        //命令栈记录总偏移量
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

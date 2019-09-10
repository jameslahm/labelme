/*!
* \file Label.h
* \brief 2d绘制图片及标注类
* \author 王澳
* \date 2019.09
*/

#ifndef LABEL_H
#define LABEL_H

#include<QLabel>
#include<QWidget>
#include"CentralWinit2D.h"

/// \brief 2d绘制图片及标注类，继承QLabel
///
/// 内部具体的形状的绘制均交由Shape类进行处理，
/// Shape类为形状基类，
/// 具体的绘制则会调用对应形状类的虚函数。

class My::Label:public QLabel{
    Q_OBJECT

private:

    /// \brief 存储当前鼠标的坐标位置
    float cursorX=0;

    /// \brief 存储当前鼠标的坐标位置
    float cursorY=0;

    /// \brief 右下角放大区域图片
    QPixmap magnifierArea;


public:
    /// \brief 默认标注形状颜色
    QColor color;

    /// \brief 默认-1，current表示当前操作的标注形状的索引
    int current=-1;

    /// \brief 中心图片
    QPixmap* pixmap;

    /// \brief 默认标注形状大小
    float size=float(0.01);

    /// \brief 默认放大倍数
    float zoomLevel=1.0;

    /// \brief 是否开启放大镜
    bool MagniFier=false;

    /// \brief 存放2d中心组件
    CentralWInit2D* manager;
    
    /// \brief 每次移动标注形状的总偏移量
    float XOffsetSum=0;

    /// \brief 每次移动标注形状的总偏移量
    float YOffsetSum=0;

public:
    /// \brief 标注形状列表
    QList<My::Shape2D*> shapes;

    /// \brief 标注状态，默认为noshape
    My::shapeStatus status=My::shapeStatus::NoShape;

    /// \brief 构造函数
    Label(QWidget* parent):QLabel(parent){
        color.setRgb(100,255,0,100);
        setMouseTracking(true);
        pixmap=new QPixmap();
        setAlignment(Qt::AlignCenter);
        magnifierArea.load(":/icons/icons/temp.jpg");
    }

    /// \brief 重写
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    ~Label(){
        delete pixmap;
    }

};

#endif // LABEL_H

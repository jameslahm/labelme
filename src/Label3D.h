/*!
* \file Label3D.h
* \brief 3d绘制图片及标注类
* \author 王澳
* \date 2019.09
*/

#ifndef LABEL3D_H
#define LABEL3D_H

#include<QLabel>
#include"CentralWInit3D.h"

/// \brief 3d绘制图片及标注类，继承QLabel
///
/// 内部具体的形状的绘制均交由Shape类进行处理，
/// Shape类为形状基类，
/// 具体的绘制则会调用对应形状类的虚函数。
class My::Label3D:public QLabel{
    Q_OBJECT

private:

    /// \brief 放大区域
    QPixmap magnifierArea;
    
    /// \brief 每次移动标注形状的总偏移量，命令栈撤销、重做用
    float XOffsetSum=0;

    /// \brief 每次移动标注形状的总偏移量，命令栈撤销、重做用
    float YOffsetSum=0;

    
    /// \brief 当前label所在视角1
    int viewAngle;
    
public:

    /// \brief 中心图片
    QPixmap* pixmap;

    /// \brief 3d中心组件
    CentralWInit3D* manager;

    /// \brief 当前鼠标位置
    float cursorX=0;

    /// \brief 当前鼠标位置
    float cursorY=0;

    /// \brief 每次移动标注形状的微小偏移量
    float xOffset=0;
    float yOffset=0;
    
    /// \brief 图片真实高度，在3d图片重建过程中，侧面的图片高度过小，因此进行了默认拉伸，默认高度放大倍数为3倍
    int realLength;
    
    /// \brief 存储标注
    QList<My::Shape2D*> shapes;

    /// \brief 默认放大倍数
    float zoomLevel=1.0;

    /// \brief 当前图片在该视角下的顺序
    int zindex=0;

    /// \brief 是否开启放大镜
    bool MagniFier=false;

    
    /// \brief 当前视角的形状是否创建完毕
    bool isFinished=false;

    /// \brief 构造函数
    Label3D(QWidget* parent,int v):QLabel(parent),viewAngle(v){
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

    ~Label3D();
};

#endif // LABEL3D_H

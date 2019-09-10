/*!
* \file CentralWInit3D.h
* \brief 3d中心组件类
* \author 王澳
* \date 2019.09
*/

#ifndef CENTRALWINIT3D_H
#define CENTRALWINIT3D_H

#include<opencv2/core.hpp>
#include<QScrollArea>
#include<QList>
#include<QListWidgetItem>
#include<QGridLayout>
#include"CentralWidget.h"

/// \brief 3d中心组件类，继承CentralWidget
///
/// 内部的绘画以及标注相关的动作交由Label3D类进行处理
/// 命令的撤销和重做交由Command3D类进行处理
/// 3d图片及标注文件的解析和存储交由Parse类进行处理
class CentralWInit3D:public My::CentralWidget{
    Q_OBJECT

private:

    /// \brief 存储2d的一系列图片
    QList<cv::Mat> list;

    /// \brief 主窗口中心滚动区域
    QScrollArea* sarea;

    /// \brief 栅格布局
    QGridLayout* gridlayout;

    /// \brief 展示3个视角图片的滚动区域
    QList<QScrollArea*> sareas;

    /// \brief 展示3个视角图片的中心labeel组件
    QList<My::Label3D*> labels;

public:

    /// \brief 标注默认大小
    float size=0.01;

    /// \brief 标注形状默认颜色
    QColor color=QColor(100,255,0,100);

    /// \brief 当前编辑状态，默认为NoShape
    My::shapeStatus status=My::NoShape;

    /// \brief 3d中心组件命令栈所用到的标注形状列表
    QList<My::Shape3D*> trashshapes;

    /// \brief 3d中心组件的命令栈组件，用来实现撤销和重做功能
    My::Command3D* command3d;  

    /// \brief 初始化当前标注，默认为-1
    int current=-1;  

    /// \brief 存储3d的标注形状
    QList<My::Shape3D*> shapes;

public:

    /// \brief 视角变量
    ///
    /// 当前焦点所在视角 0，1，2的顺序为从左往右，从上往下
    int Focus=0;
    CentralWInit3D(QWidget* parent);

    /// \brief 切换图片函数
    ///
    /// 3d状态下，下一张图片功能经由mainwindow调用该函数处理
    void nextImage();

    /// \brief 切换图片函数
    ///
    /// 3d状态下，上一张图片功能经由mainwindow调用该函数处理
    void prevImage();

    /// \brief 3d组件label3d绘制与该组件同步的统一接口函数
    ///
    /// 所有标注的变更都会由label3d调用该函数用来进行同步，
    /// 而该组件会根据同步状态分别调用不同的函数进行处理
    void sync(int v,My::syncStatus s);

    /// \brief 添加标注形状的处理函数
    void addShape(int v);

    /// \brief 添加像素点的处理函数
    void addPoint(int v);

    /// \brief 更改选中的处理函数
    void selected(int v);

    /// \brief 更改未选中的处理函数
    void unSelected(int v);

    /// \brief 移除标注形状的处理函数
    void remove(int v);

    /// \brief 绘制矩形时，改变绘画框的处理函数
    void changePoint(int v);

    /// \brief 在状态栏显示坐标的处理函数
    void showMessage(int v);

    /// \brief 更改鼠标在标注形状内的处理函数
    void hovered(int v);

    /// \brief 更改鼠标不在标注形状内的处理函数
    void unHovered(int v);

    /// \brief 移动标注形状的处理函数
    void offset(int v);

    /// \brief 解析三个视角图片，并更新标注信息
    void parseImage();


    /// \brief 用来初始化中心组件
    virtual void setUp(QMainWindow* w);

    /// \brief 解析文件（显示图片，解析标注形状）
    virtual void parse(const QString& fileName,const QString& labelfileName);

    /// \brief 改变标注的状态，槽函数
    virtual void changeShapeLabel(QListWidgetItem* item);

    /// \brief 改变标注的选中状态、槽函数
    virtual void changeSelected();

    /// \brief 创建标注形状
    virtual void createShape(My::shapeStatus s);

    /// \brief 处理按键事件
    virtual void keyPressHandle(int k);

    /// \brief 编辑标注
    virtual void editLabel();

    /// \brief 删除标注
    virtual void deleteLabel();

    /// \brief 编辑画刷大小，画刷是用来进行分割标注
    virtual void editBrush();

    /// \brief 编辑标注形状的颜色
    virtual void editColor();

    /// \brief 放大功能
    virtual void zoomIn();

    /// \brief 缩小功能
    virtual void zoomOut();

    /// \brief 使中心组件宽度适应主窗口
    virtual void fitWindow();

    /// \brief 恢复图片原始大小
    virtual void originalSize();

    /// \brief 放大镜功能
    virtual void magnifier(bool b);

    /// \brief 保存文件功能
    virtual void save(const QString& fileName);

    /// \brief 隐藏标注
    virtual void hidePolygons();

    /// \brief 显示标注
    virtual void showPolygons();

    /// \brief 关闭文件
    void closeFile();

    ~CentralWInit3D();
};

#endif // CENTRALWINIT3D_H

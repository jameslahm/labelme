/*!
* \file CentralWidget.h
* \brief 中心组件基类
* \author 王澳
* \date 2019.09
*/

#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include<QMainWindow>
#include<QWidget>
#include<QListWidgetItem>
#include"Namespace.h"

/// \brief 中心组件基类
///
/// CentralWidget类负责主窗口中心组件的界面初始化以及后续中心组件的管理。
/// 其两个派生类分别负责2D、3D中心组件，
/// 2d和3d的模式可以进行切换。
/// 内部的绘画以及标注相关的动作交由Label类（或Label3D类）进行处理。
/// 命令的撤销和重做交由Command2D类（或Command3D）进行处理。
/// 标注文件的解析和存储，以及3D图片系列文件的解析均交由Parse类进行处理。
class My::CentralWidget:public QWidget{
    Q_OBJECT

public:
signals:

    /// \brief 标注选中状态改变信号
    void selectedChanged(int index,bool b);

    /// \brief 标注删除信号
    void labelDeleted(int index);

    /// \brief 标注解析完毕信号
    void labelsFinished(QList<My::Shape*>& shapes);

    /// \brief 标注添加信号
    void labelAdded(My::Shape* shape,int index);

    /// \brief 标注改变信号
    void labelChanged(int index,My::Shape* shape);

public:

    CentralWidget(QWidget* parent):QWidget(parent){}


    /// \brief 用来初始化中心组件
    virtual void setUp(QMainWindow* w);

    /// \brief 解析文件（显示图片，解析标注形状）
    virtual void parse(const QString& fileName,const QString& labelfileName);//解析标注文件

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

    /// \brief 关闭文件功能
    virtual void closeFile();
    virtual ~CentralWidget()=0;
};

#endif // CENTRALWIDGET_H

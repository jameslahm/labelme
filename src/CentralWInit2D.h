/*!
* \file CentralWInit2D.h
* \brief 2d中心组件类
* \author 王澳
* \date 2019.09
*/

#ifndef CENTRALWINIT2D_H
#define CENTRALWINIT2D_H

#include<QScrollArea>
#include<QListWidgetItem>
#include"Shape2D.h"
#include"CentralWidget.h"

/// \brief 2d中心组件，继承CentralWidget
///
/// 内部的绘画以及标注相关的动作交由Label类
/// 命令的撤销和重做交由Command2D类
/// 标注文件的解析和存储交由Parse类进行处理
class CentralWInit2D:public My::CentralWidget{
    Q_OBJECT

private:

    /// \brief 2d中心组件滚动区域
    QScrollArea* sarea;

public:
    
    /// \brief 命令栈用来存放所用到的所有标注形状
    QList<My::Shape2D*> trashshapes;
    
    /// \brief 中心绘制组件，用来展示图片、绘制标注
    My::Label* label;

    /// \brief 命令栈组件，用来实现撤销、重做命令
    My::Command2D* command;

    /// \brief 构造函数
    CentralWInit2D(QWidget* parent);

    /// \brief 用来初始化中心组件
    virtual void setUp(QMainWindow* w);

    /// \brief 解析文件（显示图片，解析标注形状）
    virtual void parse(const QString& fileName,const QString& labelfileName);/// \brief 解析标注文件

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

    /// \brief 关闭当前文件
    void closeFile();
    ~CentralWInit2D();
};

#endif // CENTRALWINIT_H

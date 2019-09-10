/*!
* \file ActionsInit.h
* \brief 负责所有QAction管理的组件
* \author 王澳
* \date 2019.09
*/

#ifndef ACTIONSINIT_H
#define ACTIONSINIT_H

#include<QWidget>
#include<QAction>
#include<QMap>
#include<QString>
#include<QStyle>
#include<QApplication>
#include<QMenuBar>
#include<QStatusBar>
#include<QToolBar>
#include"Namespace.h"

/// \brief 负责所有QAction的管理，以及主窗口各组件QAction的初始化和状态管理
class ActionsInit:public QWidget{
    Q_OBJECT

private:

    /// \brief 存储所有QAction，键为QAction的名字，值为对应的指针
    QMap<QString,QAction*>* map;

public:
    ActionsInit(QWidget* parent):QWidget(parent){map=new QMap<QString,QAction*>();}

    /// \brief 主窗口所有QAction的初始化
    void setUp(QMainWindow* w);

    /// \brief 初始化菜单栏上的所有动作
    void setUp(QMenuBar* mbar);

    /// \brief 初始化状态栏上的所有动作
    void setUp(QStatusBar* sbar);

    /// \brief 初始化工具栏上的所有动作
    void setUp(QToolBar* tbar);

    /// \brief 负责状态初始化，当主窗口需要状态初始化时，调用该函数
    void initStatus(bool b);

    /// \brief 对外的接口，控制QAction的enable
    void enableAction(QString s);

    /// \brief 对外的接口，控制QAction的disable
    void disableAction(QString s);

    /// \brief 根据主窗口的状态，同步更改QAction的状态
    void setStatus(My::Status status);

    /// \brief 切换动作的checkstate
    void toggleAction(QString s,bool b);

    /// \brief 中心组件右键菜单
    QMenu* setUpCentralWidget();

    /// \brief 标签列表组件右键菜单
    QMenu* setUpLabelsDWInit();

    /// \brief 创建标注时的动作状态管理
    void createShape(My::shapeStatus s);
};

#endif // ACTIONSINIT_H

/*!
* \file MenubarInit.h
* \brief 菜单栏管理组件
* \author 王澳
* \date 2019.09
*/

#ifndef MENUBAR_H
#define MENUBAR_H

#include<QWidget>
#include<QMainWindow>
#include<QMenuBar>
#include<QMenu>

/// \brief 负责菜单栏组件的初始化及状态管理
class MenubarInit:public QWidget{
    Q_OBJECT

private:

    /// \brief 菜单栏组件
    QMenuBar* mbar;

public:

    MenubarInit(QWidget* parent):QWidget(parent){}

    /// \brief 菜单栏初始化
    void setUp(QMainWindow* w){
        mbar=w->menuBar();
        QMenu* menu1=mbar->addMenu("File");
        menu1->setObjectName("File");
        QMenu* menu2=mbar->addMenu("Edit");
        menu2->setObjectName("Edit");
        QMenu* menu3=mbar->addMenu("View");
        menu3->setObjectName("View");
        QMenu* menu4=mbar->addMenu("Help");
        menu4->setObjectName("Help");
    }
};

#endif // MENUBAR_H

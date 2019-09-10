/*!
* \file StatusbarInit.h
* \brief 状态栏管理组件
* \author 王澳
* \date 2019.09
*/


#ifndef STATUSBAR_H
#define STATUSBAR_H

#include<QStatusBar>
#include<QMainWindow>
#include<QWidget>


/// \brief 负责状态栏组件的初始化及状态管理
class StatusbarInit:public QWidget{
    Q_OBJECT

private:

    /// \brief 状态栏组件
    QStatusBar* sbar;

public:
    StatusbarInit(QWidget* parent):QWidget(parent){}

    /// \brief 状态栏初始化
    void setUp(QMainWindow* w){
        sbar=w->statusBar();
    }
};


#endif

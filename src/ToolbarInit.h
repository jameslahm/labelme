/*!
* \file ToolbarInit.h
* \brief 工具栏管理组件
* \author 王澳
* \date 2019.09
*/

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include<QToolBar>
#include<QWidget>
#include<QMainWindow>


/// \brief 负责工具栏组件的初始化及状态管理
class ToolbarInit:public QWidget{
    Q_OBJECT

private:

    /// \brief 工具栏组件
    QToolBar* tbar;

public:

    ToolbarInit(QWidget* parent):QWidget(parent){}

    /// \brief 返回工具栏
    QToolBar* toolBar() const{
        return tbar;
    }

    /// \brief 工具栏初始化
    void setUp(QMainWindow* w){
        tbar=new QToolBar("toolbar",w);
        tbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        w->addToolBar(Qt::LeftToolBarArea,tbar);
    }
};


#endif // TOOLBAR_H

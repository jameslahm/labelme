#ifndef TOOLBAR_H
#define TOOLBAR_H

#include<QToolBar>
#include<QWidget>
#include<QMainWindow>

class ToolbarInit:public QWidget{
    Q_OBJECT
    QToolBar* tbar;
public:
    ToolbarInit(QWidget* parent):QWidget(parent){}
    QToolBar* toolbar() const{
        return tbar;
    }
    void setUp(QMainWindow* w){
        tbar=new QToolBar("toolbar",w);
        tbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        w->addToolBar(Qt::LeftToolBarArea,tbar);

    }
};


#endif // TOOLBAR_H

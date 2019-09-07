#ifndef MENUBAR_H
#define MENUBAR_H

#include<QWidget>
#include<QMainWindow>
#include<QMenuBar>
#include<QMenu>


class MenubarInit:public QWidget{
    Q_OBJECT
    QMenuBar* mbar;
public:
    MenubarInit(QWidget* parent):QWidget(parent){}
    QMenuBar* menubar() const{
        return mbar;
    }
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

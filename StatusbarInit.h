#ifndef STATUSBAR_H
#define STATUSBAR_H

#include<QStatusBar>
#include<QMainWindow>
#include<QWidget>

class StatusbarInit:public QWidget{
    Q_OBJECT
    QStatusBar* sbar;
public:
    StatusbarInit(QWidget* parent):QWidget(parent){}
    QStatusBar* statusbar() const{
        return sbar;
    }
    void setUp(QMainWindow* w){
        sbar=w->statusBar();
    }
};


#endif

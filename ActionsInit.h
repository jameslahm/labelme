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

class MainWindow;

class ActionsInit:public QWidget{
    Q_OBJECT
    QMap<QString,QAction*>* map;
public:
    ActionsInit(QWidget* parent):QWidget(parent){map=new QMap<QString,QAction*>();}
    void setUp(MainWindow* w);
    void setUp(QMenuBar* mbar);
    void setUp(QStatusBar* sbar);
    void setUp(QToolBar* tbar);
    void initStatus(bool b);
    void enableAction(QString s);
    void disableAction(QString s);
    void setStatus(My::Status status);
    void toggleAction(QString s,bool b);
    QMenu* setUpCentralWidget();
    QMenu* setUpLabelsDWInit();
};

#endif // ACTIONSINIT_H

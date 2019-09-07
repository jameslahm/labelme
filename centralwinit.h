#ifndef CENTRALWINIT_H
#define CENTRALWINIT_H

#include<QStatusBar>
#include<QMainWindow>
#include<QWidget>
#include<QLabel>
#include<QGridLayout>
#include<QPixmap>
#include<QDebug>
#include<QScrollArea>
#include"Shape.h"
#include"Namespace.h"
#include<QListWidgetItem>
#include<QKeyEvent>


class CentralWInit:public QWidget{
    Q_OBJECT
    friend class MainWindow;
    QScrollArea* sarea;


//    QGridLayout* glayout;

public:
signals:
    void labelsFinished(QList<My::Shape*>& shapes);
    void labelAdded(My::Shape* shape,int index);
    void selectedChanged(int index,bool b);
    void labelChanged(int index,My::Shape* shape);
    void labelDeleted(int index);
public:
    QList<My::Shape*> trashshapes;

    My::Command2D* command;

    My::Label* label;
    CentralWInit(QWidget* parent):QWidget(parent){}
    void setUp2D(QMainWindow* w);
//    void changePixmap(const QString& s);
    void parse(const QString& fileName,const QString& labelfileName);//解析标注文件

    void changeShapeLabel(QListWidgetItem* item);
    void changeSelected();
    void createShape(My::shapeStatus s);

    void keyPressHandle(int k);
    void changeLabel();
    void deleteLabel();
    void editBrush();
    void editColor();
    void zoomIn();
    void zoomOut();
    void fitWindow();
    void originalSize();
    void magnifier(bool b);
    void save(const QString& fileName);
    void detach();
};

#endif // CENTRALWINIT_H

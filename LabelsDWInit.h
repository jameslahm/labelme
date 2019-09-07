#ifndef LABELS_H
#define LABELS_H

#include <QDockWidget>
#include <QMainWindow>
#include <QWidget>
#include<QScrollArea>
#include"Shape.h"
#include<QList>
#include<QGroupBox>
#include<QListWidget>


class LabelsDWInit : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
//    QScrollArea* sarea;
    QDockWidget* dwLabels;
    QListWidget* lw;
public:

    LabelsDWInit(QWidget* parent):QWidget(parent){}
    QDockWidget* dockwidget() const{
        return dwLabels;
    }
    void setUp(QMainWindow* w);
    void showLabels(QList<My::Shape*>& shapes);
    void changeSelected(int index,bool b);
    void addLabel(My::Shape* shape,int index);
    void changeLabel(int index,My::Shape* shape);
    void deleteLabel(int index);
    void showLabels3D(QList<My::Shape3D*>& shapes);
    void changeLabel3D(int index,My::Shape3D* shape);
    void addLabel3D(My::Shape3D* shape,int index);
};

#endif // LABELS_H

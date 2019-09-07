#ifndef CENTRALWINIT3D_H
#define CENTRALWINIT3D_H
#include<QWidget>
#include<QScrollArea>
#include<QMainWindow>
#include "Namespace.h"
#include<QGridLayout>
#include<opencv2/core.hpp>
#include<QList>
#include<QListWidget>
#include"shape3d.h"

class MainWindow;

class CentralWInit3D:public QWidget{
    Q_OBJECT

    friend MainWindow;

public:
    My::Command3D* command3d;
    QList<My::Shape3D*> trashshapes;
    float size=0.01;
    QColor color=QColor(100,255,0,100);
    int current=-1;

    My::shapeStatus status=My::NoShape;

    QList<cv::Mat> list;

    QList<My::Shape3D*> shapes;

    QList<QScrollArea*> sareas;
    QList<My::Label3D*> labels;

    QScrollArea* sarea;
    QGridLayout* gridlayout;


public:
signals:
    void labelsFinished(QList<My::Shape3D*>& shapes);
    void selectedChanged(int index,bool b);
    void labelChanged(int index,My::Shape3D* shape);
    void labelDeleted(int index);
    void labelAdded(My::Shape3D* shape,int index);

public:
    int Focus=0;
    CentralWInit3D(QWidget* parent):QWidget(parent){}
    void setUp3D(MainWindow* w);
    void detach();
    void parse(const QString& fileName,const QString& labelfileName);
    void changeShapeLabel(QListWidgetItem* item);
    void changeSelected();
    void nextImage();
    void prevImage();
    void sync(int v,My::syncStatus s);
    void addShape(int v);
    void addPoint(int v);
    void selected(int v);
    void unSelected(int v);
    void remove(int v);
    void changePoint(int v);
    void showMessage(int v);
    void hovered(int v);
    void unHovered(int v);
    void offset(int v);
    void keyPressHandle(int key);
    void createShape(My::shapeStatus s);
    void changeLabel();
    void deleteLable();
    void editBrush();
    void editColor();
    void magnifier(bool b);
    void parseImage();
    void save(QString fileName);
    void zoomIn();
    void zoomOut();
    void originalSize();
    void fitWindow();
};

#endif // CENTRALWINIT3D_H

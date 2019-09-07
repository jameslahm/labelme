#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"MenubarInit.h"
#include"ToolbarInit.h"
#include"ActionsInit.h"
//#include"FlagDWInit.h"
//#include"LabellistDWInit.h"
#include"LabelsDWInit.h"
#include"FilesDWInit.h"
#include"StatusbarInit.h"
#include <QMainWindow>
#include"CentralWInit.h"
#include<QFile>
#include<QFileInfo>
#include<QKeyEvent>
#include<QDir>
#include"centralwinit3d.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    //控件初始化部分
    ActionsInit* actionsinit;
    MenubarInit* mbarinit;
    ToolbarInit* tbarinit;
//    FlagDWInit* flaginit;
//    LabellistDWInit* labellistinit;
    LabelsDWInit* labelsinit;
    FilesDWInit* filesinit;
    StatusbarInit* sbarinit;

    CentralWInit* centralwidget;
    CentralWInit3D* centralwidget3d;

    //文件部分变量
    QString fileName;
    QString dirName;

    //编辑状态变量



    //
public:
    signals:
    void fileNameChanged(QString fileName,QString labelfileName);
    void statusInit(bool b);
public:
    void changeState(int index,bool b);
public:
    My::Status status=My::Status::Edit;
    My::Mode mode=My::TwoD;
    void changefile(QListWidgetItem* current,QListWidgetItem* prvious);
    void openfile();
    void nextimage();
    void previmage();
    void opendir();
    void openrecent();
    void save();
    void saveas();
    void saveautomatically();
    void changeoutputdir();
    void closefile();
    void deletefile();
    void quit();
    void createbrush();
    void createrectangle();
    void createcircle();
    void createcurve();
    void createline();
    void createpoint();
    void createlinestrip();
    void editpolygons();
    void editlabel();
    void duplicatepolygons();
    void deletepolygons();
    void undo();
    void undolastpoint();
    void editbrush();
    void polygonlinecolor();
    void polygonfillcolor();
    void polygonlabels(bool b);
    void filelist(bool b);
    void hidepolygons();
    void showpolygons();
    void zoomin();
    void zoomout();
    void originalsize();
    void fitwindow();
    void createpolygons();
    void editcolor();
    void redo();
    void magnifier(bool b);
    void import();
    void toggle3d(bool b);
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_H

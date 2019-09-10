/*!
* \file actionsinit.cpp
* \brief 负责所有QAction管理的组件
* \author 王澳
* \date 2019.09
*/

#include"ActionsInit.h"
#include"mainwindow.h"
#include<QDebug>


/// \brief 主窗口所有动作的初始化生成
void ActionsInit::setUp(QMainWindow* w1){

    MainWindow* w=qobject_cast<MainWindow*>(w1);

    //打开文件
    QStyle* style=QApplication::style();
    QAction* a1=new QAction(w);
    a1->setText("Open");
    a1->setStatusTip("Open");
    a1->setShortcut(QKeySequence::Open);
    a1->setIcon(QIcon(QPixmap(":/icons/icons/open.png")));
    connect(a1,&QAction::triggered,w,&MainWindow::openfile);
    map->insert("Open",a1);

    //下一张图片
    QAction* a2=new QAction(w);
    a2->setText("Next\nImage");
    a2->setEnabled(false);
    a2->setStatusTip("Next\nImage");
    a2->setShortcut(QKeySequence(Qt::Key_D));
    a2->setIcon(style->standardIcon(QStyle::SP_ArrowRight));
    connect(a2,&QAction::triggered,w,&MainWindow::nextimage);
    map->insert("Next Image",a2);


    //上一张图片
    QAction* a3=new QAction(w);
    a3->setText("Prev\nImage");
    a3->setEnabled(false);
    a3->setStatusTip("Prev Image");
    a3->setShortcut(QKeySequence(Qt::Key_A));
    a3->setIcon(style->standardIcon(QStyle::SP_ArrowLeft));
    connect(a3,&QAction::triggered,w,&MainWindow::previmage);
    map->insert("Prev Image",a3);

    //打开目录
    QAction* a4=new QAction(w);
    a4->setText("Open\nDir");
    a4->setStatusTip("Open Dir");
    a4->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_U));
    a4->setIcon(QIcon(QPixmap(":/icons/icons/open.png")));
    connect(a4,&QAction::triggered,w,&MainWindow::opendir);
    map->insert("Open Dir",a4);

    //打开最近
    QAction* a5=new QAction(w);
    a5->setText("Open Recent");
    a5->setStatusTip("Open Recent");
    map->insert("Open Recent",a5);

    //保存文件
    QAction* a6=new QAction(w);
    a6->setText("Save");
    a6->setEnabled(false);
    a6->setStatusTip("Save");
    a6->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_S));
    a6->setIcon(QIcon(QPixmap(":/icons/icons/save.png")));
    connect(a6,&QAction::triggered,w,&MainWindow::save);
    map->insert("Save",a6);

    //另存文件
    QAction* a7=new QAction(w);
    a7->setText("Save As");
    a7->setEnabled(false);
    a7->setStatusTip("Save As");
    a7->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_S));
    a7->setIcon(QIcon(QPixmap(":/icons/icons/save-as.png")));
    connect(a7,&QAction::triggered,w,&MainWindow::saveas);
    map->insert("Save As",a7);

    //自动保存
    QAction* a8=new QAction(w);
    a8->setText("Save Automatically");
    a8->setCheckable(true);
    a8->setChecked(false);
    a8->setStatusTip("Save Automatically");
    a8->setIcon(QIcon(QPixmap(":/icons/icons/save.png")));
    connect(a8,&QAction::triggered,w,&MainWindow::saveautomatically);
    map->insert("Save Automatically",a8);

    //改变输出目录
    QAction* a9=new QAction(w);
    a9->setText("Change Output Dir");
    a9->setStatusTip("Change Output Dir");
    a9->setIcon(style->standardIcon(QStyle::SP_DirOpenIcon));
    connect(a9,&QAction::triggered,w,&MainWindow::changeoutputdir);
    map->insert("Change Output Dir",a9);

    //关闭文件
    QAction* a10=new QAction(w);
    a10->setText("Close");
    a10->setEnabled(false);
    a10->setStatusTip("Close");
    a10->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_W));
    a10->setIcon(QIcon(QPixmap(":/icons/icons/close.png")));
    connect(a10,&QAction::triggered,w,&MainWindow::closefile);
    map->insert("Close",a10);

    //删除文件
    QAction* a11=new QAction(w);
    a11->setText("Delete\nFile");
    a11->setEnabled(false);
    a11->setStatusTip("Delete File");
    a11->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Delete));
    a11->setIcon(QIcon(QPixmap(":/icons/icons/delete.png")));
    connect(a11,&QAction::triggered,w,&MainWindow::deletefile);
    map->insert("Delete File",a11);

    //退出程序
    QAction* a12=new QAction(w);
    a12->setText("Quit");
    a12->setStatusTip("Quit");
    a12->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
    a12->setIcon(QIcon(QPixmap(":/icons/icons/quit.png")));
    connect(a12,&QAction::triggered,w,&MainWindow::quit);
    map->insert("Quit",a12);

    //创建笔刷
    QAction* a13=new QAction(w);
    a13->setText("Create\nBrush");
    a13->setEnabled(false);
    a13->setStatusTip("Create Brush");
    a13->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    a13->setIcon(QIcon(QPixmap(":/icons/icons/objects.png")));
    connect(a13,&QAction::triggered,w,&MainWindow::createbrush);
    map->insert("Create Brush",a13);

    //创建多边形
    QAction* a38=new QAction(w);
    a38->setText("Create\nPolygons");
    a38->setEnabled(false);
    a38->setStatusTip("Create Polygons");
    a38->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
    a38->setIcon(QIcon(QPixmap(":/icons/icons/objects.png")));
    connect(a38,&QAction::triggered,w,&MainWindow::createpolygons);
    map->insert("Create Polygons",a38);

    //创建矩形
    QAction* a14=new QAction(w);
    a14->setText("Create\nRectangle");
    a14->setEnabled(false);
    a14->setStatusTip("Create Rectangle");
    a14->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_R));
    a14->setIcon(QIcon(QPixmap(":/icons/icons/objects.png")));
    connect(a14,&QAction::triggered,w,&MainWindow::createrectangle);
    map->insert("Create Rectangle",a14);

    //创建圆形
    QAction* a15=new QAction(w);
    a15->setText("Create Circle");
    a15->setEnabled(false);
    a15->setStatusTip("Create Circle");
    a15->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
    a15->setIcon(QIcon(QPixmap(":/icons/icons/objects.png")));
    connect(a15,&QAction::triggered,w,&MainWindow::createcircle);
    map->insert("Create Circle",a15);

    //创建线条
    QAction* a16=new QAction(w);
    a16->setText("Create Line");
    a16->setEnabled(false);
    a16->setStatusTip("Create Line");
    a16->setIcon(QIcon(QPixmap(":/icons/icons/objects.png")));
    map->insert("Create Line",a16);

    //创建点
    QAction* a17=new QAction(w);
    a17->setText("Create Point");
    a17->setEnabled(false);
    a17->setStatusTip("Create Point");
    a17->setIcon(QIcon(QPixmap(":/icons/icons/objects.png")));
    map->insert("Create Point",a17);

    //创建折线
    QAction* a18=new QAction(w);
    a18->setText("Create LineStrip");
    a18->setEnabled(false);
    a18->setStatusTip("Create LineStrip");
    a18->setIcon(QIcon(QPixmap(":/icons/icons/objects.png")));
    map->insert("Create LineStrip",a18);


    //另存文件
    QAction* a19=new QAction(w);
    a19->setText("Edit\nPolygons");
    a19->setEnabled(false);
    a19->setStatusTip("Edit Polygons");
    a19->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_J));
    a19->setIcon(QIcon(QPixmap(":/icons/icons/edit.png")));
    connect(a19,&QAction::triggered,w,&MainWindow::editpolygons);
    map->insert("Edit Polygons",a19);

    //编辑label
    QAction* a20=new QAction(w);
    a20->setText("Edit\nLabel");
    a20->setEnabled(false);
    a20->setStatusTip("Edit Label");
    a20->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_E));
    a20->setIcon(QIcon(QPixmap(":/icons/icons/edit")));
    connect(a20,&QAction::triggered,w,&MainWindow::editlabel);
    map->insert("Edit Label",a20);

    //复制多边形
    QAction* a21=new QAction(w);
    a21->setText("Duplicate Polygons");
    a21->setEnabled(false);
    a21->setStatusTip("Duplicate Polygons");
    a21->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_D));
    a21->setIcon(QIcon(QPixmap(":/icons/icons/copy.png")));
    map->insert("Duplicate Polygons",a21);

    //删除多边形
    QAction* a22=new QAction(w);
    a22->setText("Delete\nPolygons");
    a22->setEnabled(false);
    a22->setStatusTip("Delete Polygons");
    a22->setShortcut(QKeySequence(Qt::Key_Delete));
    a22->setIcon(QIcon(QPixmap(":/icons/icons/cancel.png")));
    connect(a22,&QAction::triggered,w,&MainWindow::deletepolygons);
    map->insert("Delete Polygons",a22);

    //撤销操作
    QAction* a23=new QAction(w);
    a23->setText("Undo");
    a23->setEnabled(false);
    a23->setStatusTip("Undo");
    a23->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Z));
    a23->setIcon(QIcon(QPixmap(":/icons/icons/undo.png")));
    connect(a23,&QAction::triggered,w,&MainWindow::undo);
    map->insert("Undo",a23);

    //撤销当前操作
    QAction* a24=new QAction(w);
    a24->setText("Undo last point");
    a24->setEnabled(false);
    a24->setStatusTip("Undo last point");
    a24->setIcon(QIcon(QPixmap(":/icons/icons/undo.png")));
    connect(a24,&QAction::triggered,w,&MainWindow::undolastpoint);
    map->insert("Undo last point",a24);

    //编辑笔刷
    QAction* a25=new QAction(w);
    a25->setText("Edit\nBrush");
    a25->setEnabled(false);
    a25->setStatusTip("Edit Brush");
    a25->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_B));
    a25->setIcon(QIcon(QPixmap(":/icons/icons/edit.png")));
    connect(a25,&QAction::triggered,w,&MainWindow::editbrush);
    map->insert("Edit Brush",a25);

    //改变绘制多边形颜色
    QAction* a26=new QAction(w);
    a26->setText("Polygon Line Color");
    a26->setEnabled(false);
    a26->setStatusTip("Polygon Line Color");
    a26->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_L));
    map->insert("Polygon Line Color",a26);

    //改变多边形填充颜色
    QAction* a27=new QAction(w);
    a27->setText("Polygon Fill Color");
    a27->setEnabled(false);
    a27->setStatusTip("Polygon Fill Color");
    a27->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_L));
    a27->setIcon(QIcon(QPixmap(":/icons/icons/color.png")));
    map->insert("Polygon Fill Color",a27);

    //显示多边形的labels
    QAction* a30=new QAction(w);
    a30->setText("Polygon Labels");
    a30->setCheckable(true);
    a30->setChecked(true);
    a30->setStatusTip("Polygon Labels");
    connect(a30,&QAction::triggered,w,&MainWindow::polygonlabels);
    map->insert("Polygon Labels",a30);

    //显示文件列表
    QAction* a31=new QAction(w);
    a31->setText("File List");
    a31->setCheckable(true);
    a31->setChecked(true);
    a31->setStatusTip("File List");
    connect(a31,&QAction::triggered,w,&MainWindow::filelist);
    map->insert("File List",a31);

    //隐藏多边形
    QAction* a32=new QAction(w);
    a32->setText("Hide Polygons");
    a32->setStatusTip("Hide Polygons");
    a32->setIcon(QIcon(QPixmap(":/icons/icons/eye.png")));
    connect(a32,&QAction::triggered,w,&MainWindow::hidepolygons);
    map->insert("Hide Polygons",a32);

    //显示多边形
    QAction* a33=new QAction(w);
    a33->setText("Show Polygons");
    a33->setStatusTip("Show Polygons");
    a33->setIcon(QIcon(QPixmap(":/icons/icons/eye.png")));
    connect(a33,&QAction::triggered,w,&MainWindow::showpolygons);
    map->insert("Show Polygons",a33);

    //放大页面
    QAction* a34=new QAction(w);
    a34->setText("Zoom\nIn");
    a34->setStatusTip("Zoom In");
    a34->setEnabled(false);
    a34->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Plus));
    a34->setIcon(QIcon(QPixmap(":/icons/icons/zoom-in.png")));
    connect(a34,&QAction::triggered,w,&MainWindow::zoomin);
    map->insert("Zoom In",a34);

    //缩小页面
    QAction* a35=new QAction(w);
    a35->setText("Zoom\nOut");
    a35->setEnabled(false);
    a35->setStatusTip("Zoom Out");
    a35->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Minus));
    a35->setIcon(QIcon(QPixmap(":/icons/icons/zoom-out.png")));
    connect(a35,&QAction::triggered,w,&MainWindow::zoomout);
    map->insert("Zoom Out",a35);

    //显示原始大小
    QAction* a36=new QAction(w);
    a36->setText("Original size");
    a36->setEnabled(false);
    a36->setStatusTip("Original size");
    a36->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_0));
    a36->setIcon(QIcon(QPixmap(":/icons/icons/zoom.png")));
    connect(a36,&QAction::triggered,w,&MainWindow::originalsize);
    map->insert("Original size",a36);

    //恢复正常显示界面
    QAction* a37=new QAction(w);
    a37->setText("Fit\nWindow");
    a37->setEnabled(false);
    a37->setStatusTip("Fit Window");
    a37->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F));
    a37->setIcon(QIcon(QPixmap(":/icons/icons/fit-window.png")));
    connect(a37,&QAction::triggered,w,&MainWindow::fitwindow);
    map->insert("Fit Window",a37);

    //恢复正常显示界面
    QAction* a39=new QAction(w);
    a39->setText("Edit Color");
    a39->setEnabled(false);
    a39->setStatusTip("Edit Color");
    a39->setIcon(QIcon(QPixmap(":/icons/icons/color.png")));
    connect(a39,&QAction::triggered,w,&MainWindow::editcolor);
    map->insert("Edit Color",a39);

    //重做
    QAction* a40=new QAction(w);
    a40->setText("Redo");
    a40->setEnabled(false);
    a40->setStatusTip("Redo");
    a40->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_Z));
    a40->setIcon(QIcon(QPixmap(":/icons/icons/undo.png")));
    connect(a40,&QAction::triggered,w,&MainWindow::redo);
    map->insert("Redo",a40);

    //放大镜
    QAction* a41=new QAction(w);
    a41->setText("Magnifier");
    a41->setEnabled(false);
    a41->setCheckable(true);
    a41->setChecked(false);
    a41->setStatusTip("Magnifier");
    a41->setIcon(QIcon(QPixmap(":/icons/icons/zoom-in.png")));
    connect(a41,&QAction::triggered,w,&MainWindow::magnifier);
    map->insert("Magnifier",a41);

    //导入标注文件
    QAction* a42=new QAction(w);
    a42->setText("Import");
    a42->setEnabled(false);
    a42->setStatusTip("Import");
    connect(a42,&QAction::triggered,w,&MainWindow::import);
    map->insert("Import",a42);

    //删除标签
    QAction* a43=new QAction(w);
    a43->setText("Delete Label");
    a43->setEnabled(false);
    a43->setStatusTip("Delete Label");
    connect(a43,&QAction::triggered,w,&MainWindow::deletepolygons);
    map->insert("Delete Label",a43);

    //创建平滑曲线
    QAction* a44=new QAction(w);
    a44->setText("Create Curve");
    a44->setEnabled(false);
    a44->setStatusTip("Create Curve");
    a44->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_L));
    a44->setIcon(QIcon(QPixmap(":/icons/icons/objects.png")));
    connect(a44,&QAction::triggered,w,&MainWindow::createcurve);
    map->insert("Create Curve",a44);

    //切换3D模式
    QAction* a45=new QAction(w);
    a45->setText("3D");
    a45->setEnabled(true);
    a45->setCheckable(true);
    a45->setChecked(false);
    a45->setStatusTip("3D");
    connect(a45,&QAction::triggered,w,&MainWindow::toggle3d);
    map->insert("3D",a45);
}


/// \brief 菜单栏所有动作的初始化
void ActionsInit::setUp(QMenuBar* mbar){

    //文件菜单
    QMenu* menu1=mbar->findChild<QMenu*>("File",Qt::FindDirectChildrenOnly);

    //编辑菜单
    QMenu* menu2=mbar->findChild<QMenu*>("Edit",Qt::FindDirectChildrenOnly);

    //视图菜单
    QMenu* menu3=mbar->findChild<QMenu*>("View",Qt::FindDirectChildrenOnly);

    menu1->addAction(map->value("Open"));
    menu1->addAction(map->value("Next Image"));
    menu1->addAction(map->value("Prev Image"));
    menu1->addAction(map->value("Open Dir"));
    menu1->addAction(map->value("Import"));
    menu1->addSeparator();
    menu1->addAction(map->value("Save"));
    menu1->addAction(map->value("Save As"));
    menu1->addSeparator();
    menu1->addAction(map->value("Close"));
    menu1->addAction(map->value("Delete File"));
    menu1->addAction(map->value("Quit"));


    menu2->addAction(map->value("Create Brush"));
    menu2->addAction(map->value("Edit Brush"));
    menu2->addSeparator();
    menu2->addAction(map->value("Create Rectangle"));
    menu2->addAction(map->value("Create Circle"));
    menu2->addAction(map->value("Create Polygons"));
    menu2->addAction(map->value("Create Curve"));
    menu2->addSeparator();
    menu2->addAction(map->value("Edit Color"));
    menu2->addSeparator();
    menu2->addAction(map->value("Edit Polygons"));
    menu2->addAction(map->value("Delete Polygons"));
    menu2->addAction(map->value("Edit Label"));
    menu2->addSeparator();
    menu2->addAction(map->value("Undo"));
    menu2->addAction(map->value("Redo"));
    menu2->addAction(map->value("Undo last point"));

    menu3->addAction(map->value("Polygon Labels"));
    menu3->addAction(map->value("File List"));
    menu3->addSeparator();
    menu3->addAction(map->value("3D"));
    menu3->addSeparator();
    menu3->addAction(map->value("Hide Polygons"));
    menu3->addAction(map->value("Show Polygons"));
    menu3->addSeparator();
    menu3->addAction(map->value("Magnifier"));
    menu3->addAction(map->value("Zoom In"));
    menu3->addAction(map->value("Zoom Out"));
    menu3->addAction(map->value("Original size"));
    menu3->addAction(map->value("Fit Window"));
}


/// \brief 状态栏所有动作的初始化
void ActionsInit::setUp(QStatusBar* sbar){
    sbar->addAction(map->value("Open"));
    sbar->addAction(map->value("Next Image"));
    sbar->addAction(map->value("Prev Image"));
    sbar->addAction(map->value("Open Dir"));
    sbar->addAction(map->value("Save"));
    sbar->addAction(map->value("Save As"));
    sbar->addAction(map->value("Close"));
    sbar->addAction(map->value("Delete File"));
    sbar->addAction(map->value("Quit"));
    sbar->addAction(map->value("Create Brush"));
    sbar->addAction(map->value("Create Rectangle"));
    sbar->addAction(map->value("Create Circle"));
    sbar->addAction(map->value("Create Polygons"));
    sbar->addAction(map->value("Edit Brush"));
    sbar->addAction(map->value("Edit Polygons"));
    sbar->addAction(map->value("Edit Label"));
    sbar->addAction(map->value("Duplicate Polygons"));
    sbar->addAction(map->value("Delete Polygons"));
    sbar->addAction(map->value("Undo"));
    sbar->addAction(map->value("Redo"));
    sbar->addAction(map->value("Undo last point"));
    sbar->addAction(map->value("Polygon Line Color"));
    sbar->addAction(map->value("Polygon Fill Color"));
    sbar->addAction(map->value("Polygon Labels"));
    sbar->addAction(map->value("File List"));
    sbar->addAction(map->value("Hide Polygons"));
    sbar->addAction(map->value("Show Polygons"));
    sbar->addAction(map->value("Zoom In"));
    sbar->addAction(map->value("Zoom Out"));
    sbar->addAction(map->value("Original size"));
    sbar->addAction(map->value("Fit Window"));
    sbar->addAction(map->value("Edit Color"));
}


/// \brief 工具栏所有动作的初始化
void ActionsInit::setUp(QToolBar* tbar){
    tbar->addAction(map->value("Open"));
    tbar->addAction(map->value("Open Dir"));
    tbar->addAction(map->value("Next Image"));
    tbar->addAction(map->value("Prev Image"));
    tbar->addAction(map->value("Save"));
    tbar->addAction(map->value("Create Brush"));
    tbar->addAction(map->value("Edit Brush"));
    tbar->addAction(map->value("Create Polygons"));
    tbar->addAction(map->value("Create Rectangle"));
    tbar->addAction(map->value("Edit Color"));
    tbar->addAction(map->value("Edit Polygons"));
    tbar->addAction(map->value("Delete Polygons"));
    tbar->addAction(map->value("Edit Label"));
    tbar->addAction(map->value("Magnifier"));
    tbar->addAction(map->value("Undo"));
    tbar->addAction(map->value("Redo"));
    tbar->addAction(map->value("Zoom In"));
    tbar->addAction(map->value("Zoom Out"));
    tbar->addAction(map->value("Fit Window"));
}


/// \brief 初始化状态
void ActionsInit::initStatus(bool b){
    MainWindow* w=qobject_cast<MainWindow*>(this->parent());
    map->value("Magnifier")->setEnabled(true);
    map->value("Next Image")->setEnabled(b);
    map->value("Prev Image")->setEnabled(b);
    map->value("Save")->setEnabled(b);
    map->value("Close")->setEnabled(b);
    map->value("Delete File")->setEnabled(b);
    map->value("Create Brush")->setEnabled(b);
    map->value("Create Polygons")->setEnabled(b);
    map->value("Create Rectangle")->setEnabled(b);
    map->value("Create Circle")->setEnabled(b);
    map->value("Create Curve")->setEnabled(b);
    map->value("Undo")->setEnabled(b);
    map->value("Zoom In")->setEnabled(b);
    map->value("Zoom Out")->setEnabled(b);
    map->value("Fit Window")->setEnabled(b);
    map->value("Original size")->setEnabled(b);
    map->value("Save As")->setEnabled(b);
    map->value("Edit Color")->setEnabled(b);
    map->value("Import")->setEnabled(b);
    map->value("Redo")->setEnabled(b);
    map->value("Magnifier")->setEnabled(b);
}


/// \brief 对外接口，使动作enable
void ActionsInit::enableAction(QString s){
    map->value(s)->setEnabled(true);
}


/// \brief 对外接口，使动作disable
void ActionsInit::disableAction(QString s){
    map->value(s)->setEnabled(false);
}


/// \brief 主窗口状态改变时，会调用该函数同步更改动作的状态
void ActionsInit::setStatus(My::Status status){
    if(status==My::Status::Edit){
        map->value("Edit Polygons")->setEnabled(false);
        map->value("Duplicate Polygons")->setEnabled(false);
        map->value("Edit Label")->setEnabled(false);
        map->value("Delete Polygons")->setEnabled(false);
        map->value("Delete Label")->setEnabled(false);
        map->value("Undo")->setEnabled(true);
        map->value("Redo")->setEnabled(true);
        map->value("Create Polygons")->setEnabled(true);
        map->value("Create Brush")->setEnabled(true);
        map->value("Edit Brush")->setEnabled(false);
        map->value("Create Rectangle")->setEnabled(true);
        map->value("Create Curve")->setEnabled(true);
        map->value("Undo last point")->setEnabled(false);
        map->value("Create Circle")->setEnabled(true);
    }
    if(status==My::Status::Create){
        map->value("Edit Polygons")->setEnabled(true);
        map->value("Duplicate Polygons")->setEnabled(false);
        map->value("Edit Label")->setEnabled(false);
        map->value("Delete Polygons")->setEnabled(false);
        map->value("Delete Label")->setEnabled(false);
        map->value("Undo last point")->setEnabled(true);
    }
    if(status==My::Status::Focus){
        map->value("Edit Polygons")->setEnabled(false);
        map->value("Duplicate Polygons")->setEnabled(true);
        map->value("Edit Label")->setEnabled(true);
        map->value("Delete Polygons")->setEnabled(true);
        map->value("Delete Label")->setEnabled(true);
        map->value("Undo last point")->setEnabled(false);
    }
}



/// \brief 中心组件右键菜单动作管理
QMenu* ActionsInit::setUpCentralWidget(){
    QMenu* m=new QMenu(this);
    m->addAction(map->value("Create Brush"));
    m->addAction(map->value("Create Rectangle"));
    m->addAction(map->value("Create Polygons"));
    m->addAction(map->value("Create Circle"));
    m->addAction(map->value("Create Curve"));
    m->addAction(map->value("Edit Polygons"));
    m->addAction(map->value("Delete Polygons"));
    m->addAction(map->value("Edit Label"));
    m->addAction(map->value("Undo last point"));
    m->addAction(map->value("Undo"));
    m->addAction(map->value("Redo"));
    return m;
}


/// \brief 标签列表组件右键菜单动作管理
QMenu* ActionsInit::setUpLabelsDWInit(){
    QMenu* m=new QMenu(this);
    m->addAction(map->value("Edit Label"));
    m->addAction(map->value("Delete Label"));
    return m;
}


/// \brief 对外接口，管理动作的checkstate
void ActionsInit::toggleAction(QString s, bool b){
    map->value(s)->setChecked(b);
}


/// \brief 主窗口创建功能触发时，会调用该函数根据创建的形状进行动作状态的同步
void ActionsInit::createShape(My::shapeStatus s){
    if(s==My::BrushShape){
        disableAction("Create Brush");
        enableAction("Create Rectangle");
        enableAction("Create Polygons");
        enableAction("Create Circle");
        enableAction("Create Curve");
        enableAction("Edit Brush");
        disableAction("Undo");
        disableAction("Redo");
    }
    if(s==My::RectangleShape){
        disableAction("Create Rectangle");
        enableAction("Create Brush");
        enableAction("Create Polygons");
        enableAction("Create Circle");
        enableAction("Create Curve");
        disableAction("Edit Brush");
        disableAction("Undo");
        disableAction("Redo");
    }
    if(s==My::CircleShape){
        disableAction("Create Circle");
        enableAction("Create Rectangle");
        enableAction("Create Brush");
        enableAction("Create Polygons");
        enableAction("Create Curve");
        disableAction("Edit Brush");
        disableAction("Undo");
        disableAction("Redo");
    }
    if(s==My::PolygonsShape){
        disableAction("Create Polygons");
        enableAction("Create Rectangle");
        enableAction("Create Brush");
        enableAction("Create Circle");
        enableAction("Create Curve");
        disableAction("Edit Brush");
        disableAction("Undo");
        disableAction("Redo");
    }
    if(s==My::CurveShape){
        disableAction("Create Curve");
        enableAction("Create Rectangle");
        enableAction("Create Brush");
        enableAction("Create Circle");
        enableAction("Create Polygons");
        disableAction("Edit Brush");
        disableAction("Undo");
        disableAction("Redo");
    }
}

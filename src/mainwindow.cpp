/*!
* \file mainwindow.cpp
* \brief 主窗口
* \author 王澳
* \date 2019.09
*/

#include<QMenuBar>
#include<QMenu>
#include<QDockWidget>
#include<QToolBar>
#include<QStatusBar>
#include<QDebug>
#include<QFileDialog>
#include<QMessageBox>
#include<QDebug>
#include<QEvent>
#include "mainwindow.h"
#include"Command2D.h"
#include"Command3D.h"
#include"Label.h"
#include"Label3D.h"


/// \brief 构造函数，各个组件的初始化以及信号槽的连接
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //各组件初始化
    actionsinit=new ActionsInit(this);
    mbarinit=new MenubarInit(this);
    tbarinit=new ToolbarInit(this);
    labelsinit=new LabelsDWInit(this);
    filesinit=new FilesDWInit(this);
    sbarinit=new StatusbarInit(this);

    //中心组件初始化，默认2d组件
    centralwidget=new CentralWInit2D(this);

    //初始化
    mbarinit->setUp(this);
    tbarinit->setUp(this);
    labelsinit->setUp(this);
    filesinit->setUp(this);
    sbarinit->setUp(this);

    //中心组件初始化
    centralwidget->setUp(this);
    centralwidget2d=qobject_cast<CentralWInit2D*>(centralwidget);

    //各组件动作初始化
    actionsinit->setUp(this);
    actionsinit->setUp(menuBar());
    actionsinit->setUp(tbarinit->toolBar());
    actionsinit->setUp(statusBar());

    //中心组件右键菜单
    QMenu* m1=actionsinit->setUpCentralWidget();
    centralwidget2d->label->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(centralwidget2d->label,&QLabel::customContextMenuRequested,[=](){m1->exec(QCursor::pos());});

    //标签列表组件右键菜单
    QMenu* m2=actionsinit->setUpLabelsDWInit();
    labelsinit->dwLabels->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(labelsinit->dwLabels,&QDockWidget::customContextMenuRequested,[=](){m2->exec(QCursor::pos());});


    //当中心组件内的标注发生改变时，调用标签列表的槽函数进行同步
    connect(centralwidget,&My::CentralWidget::labelChanged,labelsinit,&LabelsDWInit::changeLabel);

    //当中心组件内的标注发生添加时，调用标签列表的槽函数进行同步
    connect(centralwidget,&My::CentralWidget::labelAdded,labelsinit,&LabelsDWInit::addLabel);

    //当中心组件解析完毕时，调用标签列表组件的槽函数同步显示标签列表
    connect(centralwidget,&My::CentralWidget::labelsFinished,labelsinit,&LabelsDWInit::showLabels);

    //当中心组件内的标注发生删除时，调用标签列表的槽函数进行同步
    connect(centralwidget,&My::CentralWidget::labelDeleted,labelsinit,&LabelsDWInit::deleteLabel);

    //当中心组件内的标注选中状态发生改变时，调用标签列表的槽函数进行同步
    connect(centralwidget,&My::CentralWidget::selectedChanged,this,&MainWindow::changeState);

    //当中心组件内的标注选中状态发生改变时，主窗口同步状态
    connect(centralwidget,&My::CentralWidget::selectedChanged,labelsinit,&LabelsDWInit::changeSelected);


    //当主窗口状态初始化时，调用负责所有QAction的组件进行初始化
    connect(this,&MainWindow::statusInit,actionsinit,&ActionsInit::initStatus);



    //当文件切换时，调用中心组件的解析函数进行相应的文件同步
    connect(this,&MainWindow::fileNameChanged,centralwidget,&My::CentralWidget::parse);

    //当文件列表组件选中状态发生改变时，主窗口同时更改当前文件
    connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);


    //当标签列表中的标注状态发生改变时，中心组件进行同步，主要同步标签列表中item的check状态，进行是否显示状态的更新
    connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);

    //当标签列表中的标注选中状态发生改变时，中心组件同步
    connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);

    //调整窗口大小
    resize(1200,800);

    //设置窗口图标
    this->setWindowIcon(QIcon(":/icons/icons/start.png"));
}

/// \brief 打开文件命令
void MainWindow::openfile(){

    disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);

    //断开标签列表组件的信号连接，便于清空标签列表的item
    disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
    disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);

    //打开文件交由文件列表组件处理
    bool i=filesinit->openFile(fileName,dirName);

    connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);

    //失败则返回
    if(!i)return;

    //发送文件打开信号
    emit(fileNameChanged(fileName,""));

    //状态初始化
    emit(statusInit(true));

    //重新连接信号
    connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
    connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
}


/// \brief 改变文件
///
/// 2d模式下在文件列表里切换文件时会调用该函数
void MainWindow::changefile(QListWidgetItem* current,QListWidgetItem* previous){

    //3d模式下直接返回
    if(mode==My::ThreeD) return;

    //断开信号槽
    disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
    disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);

    //更改文件列表的check状态
    filesinit->changeState(previous);

    //更改文件
    fileName=current->text();

    //发送信号
    emit(fileNameChanged(fileName,""));
    emit(statusInit(true));

    //重新连接
    connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
    connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
}

/// \brief 下一张图片
///
/// 2d模式下由文件列表组件进行切换
/// 3d模式下由中心组件进行切换
void MainWindow::nextimage(){

    //2d模式下由文件列表组件进行切换
    if(mode==My::TwoD){
        filesinit->nextImage(fileName);
        emit(fileNameChanged(fileName,""));
    }

    //3d模式下由中心组件进行切换
    if(mode==My::ThreeD){
        centralwidget3d->nextImage();
    }
}

/// \brief 上一张图片
/// ///
/// 2d模式下由文件列表组件进行切换
/// 3d模式下由中心组件进行切换
void MainWindow::previmage(){
    if(mode==My::TwoD){
        filesinit->prevImage(fileName);
        emit(fileNameChanged(fileName,""));
    }
    if(mode==My::ThreeD){
        centralwidget3d->prevImage();
    }
}

/// \brief 打开文件夹
void MainWindow::opendir(){
    //断开连接，便于清空文件列表
    disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);

    //断开标签列表组件的信号连接，便于清空标签列表的item
    disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
    disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);

    //打开文件交由文件列表组件处理
    bool i=filesinit->openDir(fileName,dirName);

    connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);

    //失败则返回
    if(!i)return;

    //发送文件打开信号
    emit(fileNameChanged(fileName,""));

    //状态初始化
    emit(statusInit(true));

    //重新连接信号
    connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
    connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
}

/// \brief 保存文件
void MainWindow::save(){
    centralwidget->save(fileName);
}

/// \brief 另存文件
void MainWindow::saveas(){}

/// \brief 自动保存
void MainWindow::saveautomatically(){}

/// \brief 切换输出文件夹
void MainWindow::changeoutputdir(){}

/// \brief 关闭文件
void MainWindow::closefile(){
    int res=QMessageBox::warning(this,"Close File","Are you sure to close the file ?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
    if(res==QMessageBox::Ok){

        //断开连接
        disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);

        //状态初始化
        emit(statusInit(false));

        //关闭文件
        centralwidget->closeFile();

        //重新连接
        connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
    }
}

/// \brief 删除文件
void MainWindow::deletefile(){}

/// \brief 退出程序
void MainWindow::quit(){
    int res=QMessageBox::warning(this,"Quit","Are you sure to Quit?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
    if(res==QMessageBox::Ok){
        qApp->quit();
    }
}

/// \brief 创建画刷，分割标注
void MainWindow::createbrush(){

    //创建状态下，标签列表不能交互
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::NoItemFlags);
    }

    //切换状态
    status=My::Create;
    actionsinit->setStatus(My::Create);
    actionsinit->createShape(My::BrushShape);

    //中心组件创建画刷标注形状
    centralwidget->createShape(My::BrushShape);

}

/// \brief 创建矩形
void MainWindow::createrectangle(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::NoItemFlags);
    }

    status=My::Create;
    actionsinit->setStatus(My::Create);
    actionsinit->createShape(My::RectangleShape);

    centralwidget->createShape(My::RectangleShape);

}

/// \brief 创建圆形
void MainWindow::createcircle(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::NoItemFlags);
    }

    status=My::Create;
    actionsinit->setStatus(My::Create);
    actionsinit->createShape(My::CircleShape);

    centralwidget->createShape(My::CircleShape);

}

/// \brief 创建多边形
void MainWindow::createpolygons(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::NoItemFlags);
    }

    status=My::Create;
    actionsinit->setStatus(My::Create);
    actionsinit->createShape(My::PolygonsShape);

    centralwidget->createShape(My::PolygonsShape);
}

/// \brief 创建平滑曲线
void MainWindow::createcurve(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::NoItemFlags);
    }

    status=My::Create;
    actionsinit->setStatus(My::Create);
    actionsinit->createShape(My::CurveShape);

    centralwidget->createShape(My::CurveShape);

}

/// \brief 编辑标注形状
void MainWindow::editpolygons(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
    }

    status=My::Edit;
    actionsinit->setStatus(My::Edit);
    centralwidget->createShape(My::NoShape);
}

/// \brief 编辑标注文字
void MainWindow::editlabel(){
    centralwidget->editLabel();

}


/// \brief 删除标注
void MainWindow::deletepolygons(){ 

    //先断开连接，便于选中状态处理
    disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
    centralwidget->deleteLabel();
    connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
    emit(labelsinit->lw->itemSelectionChanged());
}

/// \brief 撤销动作
void MainWindow::undo(){
    //先断开连接，便于选中状态处理
    disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
    if(mode==My::TwoD) centralwidget2d->command->undo();
    if(mode==My::ThreeD) centralwidget3d->command3d->undo();
    connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
    emit(labelsinit->lw->itemSelectionChanged());
}

/// \brief 重做动作
void MainWindow::redo(){
    //先断开连接，便于选中状态处理
    disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
    if(mode==My::TwoD) centralwidget2d->command->redo();
    if(mode==My::ThreeD) centralwidget3d->command3d->redo();
    connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
    emit(labelsinit->lw->itemSelectionChanged());
}


/// \brief 撤销正在绘制的标注
void MainWindow::undolastpoint(){
    centralwidget->keyPressHandle(Qt::Key_Escape);
}

/// \brief 编辑画刷的大小
void MainWindow::editbrush(){
    centralwidget->editBrush();
}

/// \brief 编辑标注颜色
void MainWindow::editcolor(){
    centralwidget->editColor();
}

/// \brief 是否显示标签列表组件
void MainWindow::polygonlabels(bool b){
    if(b){
        labelsinit->dwLabels->show();
    }
    else{
        labelsinit->dwLabels->hide();
    }
}

/// \brief 是否显示文件列表组件
void MainWindow::filelist(bool b){
    if(b){
        filesinit->dwFiles->show();
    }
    else{
        filesinit->dwFiles->hide();
    }
}

/// \brief 隐藏标注
void MainWindow::hidepolygons(){
    centralwidget->hidePolygons();

}

/// \brief 显示标注
void MainWindow::showpolygons(){
    centralwidget->showPolygons();
}

/// \brief 放大图片
void MainWindow::zoomin(){
    centralwidget->zoomIn();
}

/// \brief 缩小图片
void MainWindow::zoomout(){
    centralwidget->zoomOut();
}

/// \brief 原始大小
void MainWindow::originalsize(){
    centralwidget->originalSize();
}

/// \brief 宽度适应窗口
void MainWindow::fitwindow(){
    centralwidget->fitWindow();
}

/// \brief 放大镜功能
void MainWindow::magnifier(bool b){
    centralwidget->magnifier(b);
}

/// \brief 处理按键事件
void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return || event->key()==Qt::Key_Escape)
        centralwidget->keyPressHandle(event->key());
    else
        QMainWindow::keyPressEvent(event);
}


/// \brief 关闭事件
void MainWindow::closeEvent(QCloseEvent *event){
    int res=QMessageBox::warning(this,"Quit","Are you sure to Quit?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
    if(res==QMessageBox::Ok){
        event->accept();
    }
    else{
        event->ignore();
    }
}

/// \brief 改变主窗口状态
void MainWindow::changeState(int index, bool b){
    if(b){
        if(status==My::Create)return;
        actionsinit->setStatus(My::Focus);
    }
    else{
        if(status==My::Create)return;
        actionsinit->setStatus(My::Edit);
    }
}

/// \brief 导入标注文件
void MainWindow::import(){
    QString labelfileName=QFileDialog::getOpenFileName(this,"Open an Image",QDir::root().absolutePath(),\
                     "text(*.json)");
    if (labelfileName.isEmpty()){
        QMessageBox::warning(this,"File!","Please select an label file");
        return;

    }
    emit fileNameChanged(fileName,labelfileName);
}

/// \brief 切换为3d模式
void MainWindow::toggle3d(bool b){
    if(b){

        emit(statusInit(false));

        //便于清空标签列表
        disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
        centralwidget->closeFile();
        disconnect(centralwidget,&My::CentralWidget::labelsFinished,labelsinit,&LabelsDWInit::showLabels);

        //取消2d组件连接信号
        disconnect(centralwidget,&My::CentralWidget::labelChanged,labelsinit,&LabelsDWInit::changeLabel);
        disconnect(centralwidget,&My::CentralWidget::labelAdded,labelsinit,&LabelsDWInit::addLabel);
        disconnect(centralwidget,&My::CentralWidget::labelDeleted,labelsinit,&LabelsDWInit::deleteLabel);
        disconnect(centralwidget,&My::CentralWidget::selectedChanged,this,&MainWindow::changeState);
        disconnect(centralwidget,&My::CentralWidget::selectedChanged,labelsinit,&LabelsDWInit::changeSelected);
        disconnect(this,&MainWindow::fileNameChanged,centralwidget,&My::CentralWidget::parse);

        //2d组件的右键菜单
        disconnect(centralwidget2d->label,&QLabel::customContextMenuRequested,0,0);


        //取消文件列表组件连接信号
        disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        filesinit->lw->clear();

        mode=My::ThreeD;    

//        delete centralwidget;
        centralwidget=new CentralWInit3D(this);

        centralwidget3d=qobject_cast<CentralWInit3D*>(centralwidget);

        centralwidget->setUp(this);

        connect(centralwidget,&My::CentralWidget::labelChanged,labelsinit,&LabelsDWInit::changeLabel);
        connect(centralwidget,&My::CentralWidget::labelAdded,labelsinit,&LabelsDWInit::addLabel);
        connect(centralwidget,&My::CentralWidget::labelsFinished,labelsinit,&LabelsDWInit::showLabels);

        connect(centralwidget,&My::CentralWidget::selectedChanged,labelsinit,&LabelsDWInit::changeSelected);
        connect(centralwidget,&My::CentralWidget::selectedChanged,this,&MainWindow::changeState);
        connect(centralwidget,&My::CentralWidget::labelDeleted,labelsinit,&LabelsDWInit::deleteLabel);

        connect(this,&MainWindow::fileNameChanged,centralwidget,&My::CentralWidget::parse);
        connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);

        connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);

    }
    else{
        emit(statusInit(false));

        //便于清空标签列表
        disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);
        centralwidget->closeFile();
        disconnect(centralwidget,&My::CentralWidget::labelsFinished,labelsinit,&LabelsDWInit::showLabels);

        //取消3d组件连接信号
        disconnect(centralwidget,&My::CentralWidget::labelChanged,labelsinit,&LabelsDWInit::changeLabel);
        disconnect(centralwidget,&My::CentralWidget::labelAdded,labelsinit,&LabelsDWInit::addLabel);
        disconnect(centralwidget,&My::CentralWidget::labelDeleted,labelsinit,&LabelsDWInit::deleteLabel);
        disconnect(centralwidget,&My::CentralWidget::selectedChanged,this,&MainWindow::changeState);
        disconnect(centralwidget,&My::CentralWidget::selectedChanged,labelsinit,&LabelsDWInit::changeSelected);
        disconnect(this,&MainWindow::fileNameChanged,centralwidget,&My::CentralWidget::parse);

        //取消文件列表组件连接信号
        disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        filesinit->lw->clear();

        mode=My::TwoD;

//        delete centralwidget;
        centralwidget=new CentralWInit2D(this);

        centralwidget2d=qobject_cast<CentralWInit2D*>(centralwidget);

        centralwidget->setUp(this);

        connect(centralwidget,&My::CentralWidget::labelChanged,labelsinit,&LabelsDWInit::changeLabel);
        connect(centralwidget,&My::CentralWidget::labelAdded,labelsinit,&LabelsDWInit::addLabel);
        connect(centralwidget,&My::CentralWidget::labelsFinished,labelsinit,&LabelsDWInit::showLabels);
        connect(centralwidget,&My::CentralWidget::selectedChanged,labelsinit,&LabelsDWInit::changeSelected);
        connect(centralwidget,&My::CentralWidget::selectedChanged,this,&MainWindow::changeState);
        connect(centralwidget,&My::CentralWidget::labelDeleted,labelsinit,&LabelsDWInit::deleteLabel);

        connect(this,&MainWindow::fileNameChanged,centralwidget,&My::CentralWidget::parse);
        connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);

        connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&My::CentralWidget::changeShapeLabel);
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&My::CentralWidget::changeSelected);

        //2d中心组件右键菜单
        QMenu* m1=actionsinit->setUpCentralWidget();
        centralwidget2d->label->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(centralwidget2d->label,&QLabel::customContextMenuRequested,[=](){m1->exec(QCursor::pos());});
   }
}


MainWindow::~MainWindow()
{

}

#include "mainwindow.h"
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
#include"label.h"
#include"shape3d.h"
#include"command2d.h"
#include"command3d.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    actionsinit=new ActionsInit(this);
    mbarinit=new MenubarInit(this);
    tbarinit=new ToolbarInit(this);
//    flaginit=new FlagDWInit(this);
//    labellistinit=new LabellistDWInit(this);
    labelsinit=new LabelsDWInit(this);
    filesinit=new FilesDWInit(this);
    sbarinit=new StatusbarInit(this);
    centralwidget=new CentralWInit(this);


    mbarinit->setUp(this);
    tbarinit->setUp(this);
//    flaginit->setUp(this);
//    labellistinit->setUp(this);
    labelsinit->setUp(this);
    filesinit->setUp(this);
    sbarinit->setUp(this);



    centralwidget->setUp2D(this);


    actionsinit->setUp(this);

    actionsinit->setUp(mbarinit->menubar());

    actionsinit->setUp(tbarinit->toolbar());
    actionsinit->setUp(sbarinit->statusbar());

    QMenu* m1=actionsinit->setUpCentralWidget();
    centralwidget->label->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(centralwidget->label,&QLabel::customContextMenuRequested,[=](){m1->exec(QCursor::pos());});

    QMenu* m2=actionsinit->setUpLabelsDWInit();
    labelsinit->dwLabels->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(labelsinit->dwLabels,&QDockWidget::customContextMenuRequested,[=](){m2->exec(QCursor::pos());});


    connect(centralwidget,&CentralWInit::labelChanged,labelsinit,&LabelsDWInit::changeLabel);
    connect(centralwidget,&CentralWInit::labelDeleted,labelsinit,&LabelsDWInit::deleteLabel);
    connect(centralwidget,&CentralWInit::selectedChanged,this,&MainWindow::changeState);
    connect(centralwidget,&CentralWInit::selectedChanged,labelsinit,&LabelsDWInit::changeSelected);
    connect(centralwidget,&CentralWInit::labelAdded,labelsinit,&LabelsDWInit::addLabel);
    connect(this,&MainWindow::statusInit,actionsinit,&ActionsInit::initStatus);
    connect(this,&MainWindow::fileNameChanged,centralwidget,&CentralWInit::parse);
    connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
    connect(centralwidget,&CentralWInit::labelsFinished,labelsinit,&LabelsDWInit::showLabels);
    connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
    connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);

    resize(1200,800);
    this->setWindowIcon(QIcon(":/icons/icons/start.png"));
}
void MainWindow::openfile(){
    if(mode==My::TwoD){
        disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
        bool i=filesinit->openFile(fileName,dirName);
        connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        if(!i)return;
        emit(fileNameChanged(fileName,""));
        emit(statusInit(true));
        centralwidget->label->status=My::NoShape;
        connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
    }
    if(mode==My::ThreeD){
        disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget3d,&CentralWInit3D::changeShapeLabel);
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
        bool i=filesinit->openFile(fileName,dirName);
        if(!i)return;
        emit(fileNameChanged(fileName,""));
        emit(statusInit(true));
        connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget3d,&CentralWInit3D::changeShapeLabel);
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);

    }
}

void MainWindow::changefile(QListWidgetItem* current,QListWidgetItem* previous){
    disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
    disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
    filesinit->changeState(previous);
    fileName=current->text();
    emit(fileNameChanged(fileName,""));
    emit(statusInit(true));
    centralwidget->label->status=My::NoShape;
    connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
    connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
}

void MainWindow::nextimage(){
    if(mode==My::TwoD){
        filesinit->nextImage(fileName);
        emit(fileNameChanged(fileName,""));
    }
    if(mode==My::ThreeD){
        centralwidget3d->nextImage();
    }
}

void MainWindow::previmage(){
    if(mode==My::TwoD){
        filesinit->prevImage(fileName);
        emit(fileNameChanged(fileName,""));
    }
    if(mode==My::ThreeD){
        centralwidget3d->prevImage();
    }
}

void MainWindow::opendir(){
    if(mode==My::TwoD){

        disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
        bool i=filesinit->openDir(fileName,dirName);
        connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        if(!i)return;
        emit(fileNameChanged(fileName,""));
        emit(statusInit(true));
        centralwidget->label->status=My::NoShape;
        connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
    }
    if(mode==My::ThreeD){
        disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget3d,&CentralWInit3D::changeShapeLabel);
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
        bool i=filesinit->openDir(fileName,dirName);
        if(!i)return;
        emit(fileNameChanged(fileName,""));
        emit(statusInit(true));
        connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget3d,&CentralWInit3D::changeShapeLabel);
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
    }
}

void MainWindow::openrecent(){}
void MainWindow::save(){
    if(mode==My::TwoD) centralwidget->save(fileName);
    if(mode==My::ThreeD) centralwidget3d->save(fileName);
}
void MainWindow::saveas(){}
void MainWindow::saveautomatically(){}
void MainWindow::changeoutputdir(){}
void MainWindow::closefile(){
    if(mode==My::TwoD){
        int res=QMessageBox::warning(this,"Close File","Are you sure to close the file ?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
        if(res==QMessageBox::Ok){
            disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
            disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
            disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
            emit(statusInit(false));
            emit(fileNameChanged("",""));
            connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
            connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
            connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
        }
    }
    if(mode==My::ThreeD){
        int res=QMessageBox::warning(this,"Close File","Are you sure to close the file ?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
        if(res==QMessageBox::Ok){
            disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget3d,&CentralWInit3D::changeShapeLabel);
            disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
            emit(statusInit(false));
            emit(fileNameChanged("",""));
            connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget3d,&CentralWInit3D::changeShapeLabel);
            connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
        }
    }
}
void MainWindow::deletefile(){}
void MainWindow::quit(){
    int res=QMessageBox::warning(this,"Quit","Are you sure to Quit?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
    if(res==QMessageBox::Ok){
        qApp->quit();
    }
}
void MainWindow::createbrush(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::NoItemFlags);
    }
    if(mode==My::TwoD){
        status=My::Create;
        actionsinit->setStatus(My::Create);
        actionsinit->disableAction("Create Brush");
        actionsinit->enableAction("Create Rectangle");
        actionsinit->enableAction("Create Polygons");
        actionsinit->enableAction("Create Circle");
        actionsinit->enableAction("Create Curve");
        actionsinit->enableAction("Edit Brush");
        actionsinit->disableAction("Undo");
        actionsinit->disableAction("Redo");
        centralwidget->createShape(My::BrushShape);
    }
    if(mode==My::ThreeD){
        status=My::Create;
        actionsinit->setStatus(My::Create);
        actionsinit->disableAction("Create Brush");
        actionsinit->enableAction("Create Rectangle");
        actionsinit->enableAction("Create Polygons");
        actionsinit->enableAction("Create Circle");
        actionsinit->enableAction("Create Curve");
        actionsinit->enableAction("Edit Brush");
        actionsinit->disableAction("Undo");
        actionsinit->disableAction("Redo");
        centralwidget3d->createShape(My::BrushShape);
    }


}

void MainWindow::createrectangle(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::NoItemFlags);
    }
    if(mode==My::TwoD){
        status=My::Create;
        actionsinit->setStatus(My::Create);
        actionsinit->disableAction("Create Rectangle");
        actionsinit->enableAction("Create Brush");
        actionsinit->enableAction("Create Polygons");
        actionsinit->enableAction("Create Circle");
        actionsinit->enableAction("Create Curve");
        actionsinit->disableAction("Edit Brush");
        actionsinit->disableAction("Undo");
        actionsinit->disableAction("Redo");
        centralwidget->createShape(My::RectangleShape);
    }
    if(mode==My::ThreeD){
        status=My::Create;
        actionsinit->setStatus(My::Create);
        actionsinit->disableAction("Create Rectangle");
        actionsinit->enableAction("Create Brush");
        actionsinit->enableAction("Create Polygons");
        actionsinit->enableAction("Create Circle");
        actionsinit->enableAction("Create Curve");
        actionsinit->disableAction("Edit Brush");
        actionsinit->disableAction("Undo");
        actionsinit->disableAction("Redo");
        centralwidget3d->createShape(My::RectangleShape);
    }
}
void MainWindow::createcircle(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::NoItemFlags);
    }
    if(mode==My::TwoD){
        status=My::Create;
        actionsinit->setStatus(My::Create);
        actionsinit->disableAction("Create Circle");
        actionsinit->enableAction("Create Rectangle");
        actionsinit->enableAction("Create Brush");
        actionsinit->enableAction("Create Polygons");
        actionsinit->enableAction("Create Curve");
        actionsinit->disableAction("Edit Brush");
        actionsinit->disableAction("Undo");
        actionsinit->disableAction("Redo");
        centralwidget->createShape(My::CircleShape);
    }
    if(mode==My::ThreeD){
        actionsinit->setStatus(My::Create);
        actionsinit->disableAction("Create Circle");
        actionsinit->enableAction("Create Rectangle");
        actionsinit->enableAction("Create Brush");
        actionsinit->enableAction("Create Polygons");
        actionsinit->enableAction("Create Curve");
        actionsinit->disableAction("Undo");
        actionsinit->disableAction("Redo");
        actionsinit->disableAction("Edit Brush");
    }
}
void MainWindow::createpolygons(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::NoItemFlags);
    }
    if(mode==My::TwoD){
        status=My::Create;
        actionsinit->setStatus(My::Create);
        actionsinit->disableAction("Create Polygons");
        actionsinit->enableAction("Create Rectangle");
        actionsinit->enableAction("Create Brush");
        actionsinit->enableAction("Create Circle");
        actionsinit->enableAction("Create Curve");
        actionsinit->disableAction("Edit Brush");
        actionsinit->disableAction("Undo");
        actionsinit->disableAction("Redo");
        centralwidget->createShape(My::PolygonsShape);
    }
    if(mode==My::ThreeD){
        actionsinit->setStatus(My::Create);
        actionsinit->disableAction("Create Polygons");
        actionsinit->enableAction("Create Rectangle");
        actionsinit->enableAction("Create Brush");
        actionsinit->enableAction("Create Circle");
        actionsinit->enableAction("Create Curve");
        actionsinit->disableAction("Undo");
        actionsinit->disableAction("Redo");
        actionsinit->disableAction("Edit Brush");
    }
}

void MainWindow::createcurve(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::NoItemFlags);
    }
    if(mode==My::TwoD){
        status=My::Create;
        actionsinit->setStatus(My::Create);
        actionsinit->disableAction("Create Curve");
        actionsinit->enableAction("Create Rectangle");
        actionsinit->enableAction("Create Brush");
        actionsinit->enableAction("Create Circle");
        actionsinit->enableAction("Create Polygons");
        actionsinit->disableAction("Edit Brush");
        actionsinit->disableAction("Undo");
        actionsinit->disableAction("Redo");
        centralwidget->createShape(My::CurveShape);
    }
    if(mode==My::ThreeD){
        actionsinit->setStatus(My::Create);
        actionsinit->disableAction("Create Curve");
        actionsinit->enableAction("Create Rectangle");
        actionsinit->enableAction("Create Brush");
        actionsinit->enableAction("Create Circle");
        actionsinit->enableAction("Create Polygons");
        actionsinit->disableAction("Undo");
        actionsinit->disableAction("Redo");
        actionsinit->disableAction("Edit Brush");
    }
}
void MainWindow::createline(){}
void MainWindow::createpoint(){}
void MainWindow::createlinestrip(){}
void MainWindow::editpolygons(){
    for(int i=0;i<labelsinit->lw->count();i++){
        labelsinit->lw->item(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
    }
    if(mode==My::TwoD){
        status=My::Edit;
        actionsinit->setStatus(My::Edit);
        centralwidget->createShape(My::NoShape);
    }
    if(mode==My::ThreeD){
        status=My::Edit;
        actionsinit->setStatus(My::Edit);
        centralwidget3d->createShape(My::NoShape);
        for(int i=0;i<labelsinit->lw->count();i++){
            labelsinit->lw->item(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        }
    }
}
void MainWindow::editlabel(){
    if(mode==My::TwoD)centralwidget->changeLabel();
    if(mode==My::ThreeD)centralwidget3d->changeLabel();
}
void MainWindow::duplicatepolygons(){}
void MainWindow::deletepolygons(){
    if(mode==My::TwoD) {
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
        centralwidget->deleteLabel();
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
        qDebug()<<labelsinit->lw->count();
        emit(labelsinit->lw->itemSelectionChanged());
     }
    if(mode==My::ThreeD) {
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
        centralwidget3d->deleteLable();
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
        emit(labelsinit->lw->itemSelectionChanged());
    }
}
void MainWindow::undo(){
    if(mode==My::TwoD){
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
        centralwidget->command->undo();
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
        emit(labelsinit->lw->itemSelectionChanged());
    }
    if(mode==My::ThreeD){
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
        centralwidget3d->command3d->undo();
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
        emit(labelsinit->lw->itemSelectionChanged());
    }
}
void MainWindow::undolastpoint(){
    if(mode==My::TwoD) centralwidget->keyPressHandle(Qt::Key_Escape);
    if(mode==My::ThreeD) centralwidget3d->keyPressHandle(Qt::Key_Escape);
}
void MainWindow::editbrush(){
    if(mode==My::TwoD) centralwidget->editBrush();
    if(mode==My::ThreeD) centralwidget3d->editBrush();
}

void MainWindow::editcolor(){
    if(mode==My::TwoD) centralwidget->editColor();
    if(mode==My::ThreeD) centralwidget3d->editColor();
}
void MainWindow::polygonlinecolor(){}
void MainWindow::polygonfillcolor(){}
void MainWindow::polygonlabels(bool b){
    if(b){
        labelsinit->dwLabels->show();
    }
    else{
        labelsinit->dwLabels->hide();
    }
}
void MainWindow::filelist(bool b){
    if(b){
        filesinit->dwFiles->show();
    }
    else{
        filesinit->dwFiles->hide();
    }
}
void MainWindow::hidepolygons(){
    if(mode==My::TwoD){
        foreach(My::Shape* shape,centralwidget->label->shapes){
            shape->isHide=true;
        }
        centralwidget->label->update();
    }
    if(mode==My::ThreeD){
        foreach(My::Shape3D* shape,centralwidget3d->shapes){
            shape->isHide=true;
        }
        centralwidget3d->parseImage();
    }

}
void MainWindow::showpolygons(){
    if(mode==My::TwoD){
        foreach(My::Shape* shape,centralwidget->label->shapes){
            shape->isHide=false;
        }
        centralwidget->label->update();
    }
    if(mode==My::ThreeD){
        foreach(My::Shape3D* shape,centralwidget3d->shapes){
            shape->isHide=false;
        }
        centralwidget3d->parseImage();
    }
}

void MainWindow::zoomin(){
    if(mode==My::TwoD){centralwidget->zoomIn();}
    if(mode==My::ThreeD){centralwidget3d->zoomIn();}
}
void MainWindow::zoomout(){
    if(mode==My::TwoD) centralwidget->zoomOut();
    if(mode==My::ThreeD) centralwidget3d->zoomOut();
}
void MainWindow::originalsize(){
    if(mode==My::TwoD) centralwidget->originalSize();
    if(mode==My::ThreeD) centralwidget3d->originalSize();
}
void MainWindow::fitwindow(){
    if(mode==My::TwoD) centralwidget->fitWindow();
    if(mode==My::ThreeD) centralwidget3d->fitWindow();
}

void MainWindow::magnifier(bool b){
    if(mode==My::TwoD) centralwidget->magnifier(b);
    if(mode==My::ThreeD) centralwidget3d->magnifier(b);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if(mode==My::TwoD){
        if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return || event->key()==Qt::Key_Escape) centralwidget->keyPressHandle(event->key());
        else{
            QMainWindow::keyPressEvent(event);
        }
    }
    if(mode==My::ThreeD){
        centralwidget3d->keyPressHandle(event->key());
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    int res=QMessageBox::warning(this,"Quit","Are you sure to Quit?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
    if(res==QMessageBox::Ok){
        event->accept();
    }
    else{

        event->ignore();
    }
}

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

void MainWindow::import(){
    QString labelfileName=QFileDialog::getOpenFileName(this,"Open an Image",QDir::root().absolutePath(),\
                     "text(*.json)");
    if (labelfileName.isEmpty()){
        QMessageBox::warning(this,"File!","Please select an label file");
        return;

    }
    emit fileNameChanged(fileName,labelfileName);
}

void MainWindow::toggle3d(bool b){
    if(b){

        disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);

        actionsinit->toggleAction("Magnifier",false);
        emit(statusInit(false));
        emit(fileNameChanged("",""));
        connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);

        mode=My::ThreeD;
        disconnect(centralwidget,&CentralWInit::labelChanged,labelsinit,&LabelsDWInit::changeLabel);
        disconnect(centralwidget,&CentralWInit::labelDeleted,labelsinit,&LabelsDWInit::deleteLabel);
        disconnect(centralwidget,&CentralWInit::selectedChanged,this,&MainWindow::changeState);
        disconnect(centralwidget,&CentralWInit::selectedChanged,labelsinit,&LabelsDWInit::changeSelected);
        disconnect(centralwidget,&CentralWInit::labelAdded,labelsinit,&LabelsDWInit::addLabel);
        disconnect(this,&MainWindow::statusInit,actionsinit,&ActionsInit::initStatus);
//        disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);

        disconnect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        filesinit->lw->clear();

        disconnect(this,&MainWindow::fileNameChanged,centralwidget,&CentralWInit::parse);
        disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
        disconnect(centralwidget,&CentralWInit::labelsFinished,labelsinit,&LabelsDWInit::showLabels);

        centralwidget3d=new CentralWInit3D(this);
        centralwidget3d->setUp3D(this);

        centralwidget->detach();


        connect(this,&MainWindow::fileNameChanged,centralwidget3d,&CentralWInit3D::parse);
        connect(centralwidget3d,&CentralWInit3D::selectedChanged,labelsinit,&LabelsDWInit::changeSelected);
        connect(centralwidget3d,&CentralWInit3D::selectedChanged,this,&MainWindow::changeState);
        connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget3d,&CentralWInit3D::changeShapeLabel);
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
        connect(centralwidget3d,&CentralWInit3D::labelsFinished,labelsinit,&LabelsDWInit::showLabels3D);
        connect(this,&MainWindow::statusInit,actionsinit,&ActionsInit::initStatus);
        connect(centralwidget3d,&CentralWInit3D::labelChanged,labelsinit,&LabelsDWInit::changeLabel3D);
        connect(centralwidget3d,&CentralWInit3D::labelDeleted,labelsinit,&LabelsDWInit::deleteLabel);
        connect(centralwidget3d,&CentralWInit3D::labelAdded,labelsinit,&LabelsDWInit::addLabel3D);


    }
    else{



        disconnect(centralwidget3d,&CentralWInit3D::selectedChanged,labelsinit,&LabelsDWInit::changeSelected);
        disconnect(centralwidget3d,&CentralWInit3D::selectedChanged,this,&MainWindow::changeState);
        disconnect(labelsinit->lw,&QListWidget::itemChanged,centralwidget3d,&CentralWInit3D::changeShapeLabel);
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);


        disconnect(centralwidget3d,&CentralWInit3D::labelChanged,labelsinit,&LabelsDWInit::changeLabel3D);
        disconnect(centralwidget3d,&CentralWInit3D::labelDeleted,labelsinit,&LabelsDWInit::deleteLabel);
        disconnect(centralwidget3d,&CentralWInit3D::labelAdded,labelsinit,&LabelsDWInit::addLabel3D);
        actionsinit->toggleAction("Magnifier",false);
        emit(statusInit(false));
        emit(fileNameChanged("",""));
        mode=My::TwoD;
        disconnect(this,&MainWindow::statusInit,actionsinit,&ActionsInit::initStatus);
        disconnect(this,&MainWindow::fileNameChanged,centralwidget3d,&CentralWInit3D::parse);
        disconnect(centralwidget3d,&CentralWInit3D::labelsFinished,labelsinit,&LabelsDWInit::showLabels3D);
        centralwidget3d->detach();

        centralwidget=new CentralWInit(this);
        centralwidget->setUp2D(this);
        QMenu* m1=actionsinit->setUpCentralWidget();
        centralwidget->label->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(centralwidget->label,&QLabel::customContextMenuRequested,[=](){m1->exec(QCursor::pos());});
        connect(centralwidget,&CentralWInit::labelChanged,labelsinit,&LabelsDWInit::changeLabel);
        connect(centralwidget,&CentralWInit::labelDeleted,labelsinit,&LabelsDWInit::deleteLabel);
        connect(centralwidget,&CentralWInit::selectedChanged,this,&MainWindow::changeState);
        connect(centralwidget,&CentralWInit::selectedChanged,labelsinit,&LabelsDWInit::changeSelected);
        connect(centralwidget,&CentralWInit::labelAdded,labelsinit,&LabelsDWInit::addLabel);
        connect(this,&MainWindow::statusInit,actionsinit,&ActionsInit::initStatus);
        connect(this,&MainWindow::fileNameChanged,centralwidget,&CentralWInit::parse);
        connect(filesinit->lw,&QListWidget::currentItemChanged,this,&MainWindow::changefile);
        connect(centralwidget,&CentralWInit::labelsFinished,labelsinit,&LabelsDWInit::showLabels);
        connect(labelsinit->lw,&QListWidget::itemChanged,centralwidget,&CentralWInit::changeShapeLabel);
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
   }
}
void MainWindow::redo(){
    if(mode==My::TwoD){
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
        centralwidget->command->redo();
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget,&CentralWInit::changeSelected);
        emit(labelsinit->lw->itemSelectionChanged());
     }
    if(mode==My::ThreeD){
        disconnect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
        centralwidget3d->command3d->redo();
        connect(labelsinit->lw,&QListWidget::itemSelectionChanged,centralwidget3d,&CentralWInit3D::changeSelected);
        emit(labelsinit->lw->itemSelectionChanged());
    }
}

MainWindow::~MainWindow()
{

}

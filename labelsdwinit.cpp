#include"LabelsDWInit.h"
#include<QListWidgetItem>
#include"mainWindow.h"
#include"shape3d.h"
#include"rectangle3d.h"
#include"brush3d.h"

void LabelsDWInit::setUp(QMainWindow *w)
{
    dwLabels = new QDockWidget(w);
    dwLabels->setObjectName("Labels");
    dwLabels->setWindowTitle("Labels");
//        dwLabels->setMinimumWidth(150);
    w->addDockWidget(Qt::RightDockWidgetArea, dwLabels);
//        sarea=new QScrollArea();
//        dwLabels->setWidget(sarea);
    lw=new QListWidget(dwLabels);
    dwLabels->setWidget(lw);
}


void LabelsDWInit::showLabels(QList<My::Shape *> &shapes){
    lw->clear();
//    lw->setSelectionMode(QAbstractItemView::ExtendedSelection);
    foreach(My::Shape* shape,shapes){
        QListWidgetItem* item=new QListWidgetItem();
//        item->setFlags(Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::CheckState::Checked);
        item->setText(shape->label);
        lw->addItem(item);
    }
}

void LabelsDWInit::showLabels3D(QList<My::Shape3D *>& shapes){
    lw->clear();
//    lw->setSelectionMode(QAbstractItemView::ExtendedSelection);
    foreach(My::Shape3D* shape,shapes){
        QListWidgetItem* item=new QListWidgetItem();
//        item->setFlags(Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::CheckState::Checked);
        item->setText(shape->label);
        lw->addItem(item);
    }
}

void LabelsDWInit::changeSelected(int index,bool b){
    if(b && (!lw->item(index)->isSelected())){
        lw->item(index)->setSelected(true);
    }
    if((!b)&&lw->item(index)->isSelected()){
        (lw->item(index))->setSelected(false);
    }
//    emit(lw->itemSelectionChanged(lw->item(index)));
}

void LabelsDWInit::addLabel(My::Shape *shape,int index){
    MainWindow* w=qobject_cast<MainWindow*>(parent());
    QListWidgetItem* item=new QListWidgetItem();
    item->setText(shape->label);
    item->setCheckState(Qt::CheckState::Checked);
    if(w->mode==My::TwoD){
        if(w->status==My::Create)item->setFlags(Qt::NoItemFlags);
    }
    if(w->mode==My::ThreeD){
        if(w->status==My::Create)item->setFlags(Qt::NoItemFlags);
    }
    lw->insertItem(index,item);
}

void LabelsDWInit::changeLabel(int index, My::Shape *shape){
    lw->item(index)->setText(shape->label);
}

void LabelsDWInit::changeLabel3D(int index,My::Shape3D* shape){
    lw->item(index)->setText(shape->label);
}

void LabelsDWInit::deleteLabel(int index){
    QListWidgetItem* item=lw->item(index);
    lw->removeItemWidget(item);
    delete item;

}

void LabelsDWInit::addLabel3D(My::Shape3D *shape,int index){
    MainWindow* w=qobject_cast<MainWindow*>(parent());
    QListWidgetItem* item=new QListWidgetItem();
    item->setText(shape->label);
    item->setCheckState(Qt::CheckState::Checked);
    if(w->mode==My::TwoD){
        if(w->status==My::Create)item->setFlags(Qt::NoItemFlags);
    }
    if(w->mode==My::ThreeD){
        if(w->status==My::Create)item->setFlags(Qt::NoItemFlags);
    }
    lw->insertItem(index,item);
}

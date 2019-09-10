/*!
* \file labelsdwinit.cpp
* \brief 标签列表管理组件
* \author 王澳
* \date 2019.09
*/

#include<QListWidgetItem>
#include"LabelsDWInit.h"
#include"mainwindow.h"
#include"Shape3D.h"
#include"Rectangle3D.h"
#include"Brush3D.h"


/// \brief 标签列表组件的初始化
void LabelsDWInit::setUp(QMainWindow *w)
{
    dwLabels = new QDockWidget(w);
    dwLabels->setObjectName("Labels");
    dwLabels->setWindowTitle("Labels");
    w->addDockWidget(Qt::RightDockWidgetArea, dwLabels);
    lw=new QListWidget(dwLabels);
    dwLabels->setWidget(lw);
}

/// \brief 当中心组件解析完毕时，会调用该函数使标签列表同时显示
void LabelsDWInit::showLabels(QList<My::Shape *> &shapes){
    lw->clear();
    foreach(My::Shape* shape,shapes){
        QListWidgetItem* item=new QListWidgetItem();
        item->setCheckState(Qt::CheckState::Checked);
        item->setText(shape->label);
        lw->addItem(item);
    }
}

/// \brief 改变选中的item
void LabelsDWInit::changeSelected(int index,bool b){
    if(b && (!lw->item(index)->isSelected())){
        lw->item(index)->setSelected(true);
    }
    if((!b)&&lw->item(index)->isSelected()){
        (lw->item(index))->setSelected(false);
    }
}

/// \brief 添加标注
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

/// \brief 改变标注
void LabelsDWInit::changeLabel(int index, My::Shape *shape){
    lw->item(index)->setText(shape->label);
}


/// \brief 删除标注
void LabelsDWInit::deleteLabel(int index){
    QListWidgetItem* item=lw->item(index);
    lw->removeItemWidget(item);
    delete item;
}


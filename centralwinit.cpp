#include"mainwindow.h"
#include"Rectangle.h"
#include"Shape.h"
#include"Circle.h"
#include"Brush.h"
#include"Polygons.h"
#include"Curve.h"
#include"centralwinit.h"
#include<QJsonDocument>
#include<QJsonParseError>
#include<QColor>
#include<QPainter>
#include<math.h>
#include"parse.h"
#include<QInputDialog>
#include<QKeyEvent>
#include"label.h"
#include<QColorDialog>
#include"command2d.h"
#include"centralwinit.h"

void CentralWInit::setUp2D(QMainWindow *w){
    command=new My::Command2D();
    command->manager=this;
    label=new My::Label(this);
    label->manager=this;
    sarea=new QScrollArea(w);
    w->setCentralWidget(sarea);
//    central->setFrameStyle(1);
//    QPixmap p("E:/c++/qt/LabelMe/images/1.jpg");
//    int a=p.load("E:/c++/qt/LabelMe/images/1.jpg");
//    qDebug()<<a<<endl;
//    central->setPixmap(p);
    sarea->setWidget(label);
    sarea->setAlignment(Qt::AlignCenter);

//    connect(qobject_cast<MainWindow*>(w),&MainWindow::fileNameChanged,this,&CentralWInit::changePixmap);
}

//void CentralWInit::changePixmap(const QString& fileName){
//    pixmap->load(fileName);
//    this->setPixmap(*pixmap);
//    update();
//    this->adjustSize();
//}

void CentralWInit::parse(const QString &fileName,const QString &labelfileName){
    label->status=My::NoShape;
    label->current=-1;
    if(fileName.isEmpty()){
        delete label->pixmap;
        label->pixmap=new QPixmap();
        for(int i=0;i<label->shapes.length();i++){
            delete label->shapes[i];
        }
        label->shapes.clear();
        emit(labelsFinished(label->shapes));
        label->update();
        return;
    }
    label->pixmap->load(fileName);
    My::Parse::parse(fileName,label->shapes,labelfileName);
    label->update();
    emit labelsFinished(label->shapes);
}


void CentralWInit::changeShapeLabel(QListWidgetItem *item){
    QListWidget* lw=item->listWidget();
    int index=lw->row(item);
    label->shapes[index]->isHide=!(item->checkState());
    label->shapes[index]->label=item->text();
    label->update();
}

void CentralWInit::changeSelected(){
    QListWidget* lw=qobject_cast<QListWidget*>(sender());
    int temp=lw->count();
    for(int index=0;index<lw->count();index++){
        if(lw->item(index)->isSelected()){label->shapes[index]->isFill=true;temp=index;}
        else label->shapes[index]->isFill=false;
    }
    if(temp==lw->count())label->current=-1;
    else label->current=temp;
    if(label->current!=-1)emit selectedChanged(label->current,true);
    label->update();
    qDebug()<<"changeSelected "<<label->current<<endl;
}

void CentralWInit::createShape(My::shapeStatus s){
    if(label->status==My::BrushShape || label->status==My::RectangleShape || label->status==My::PolygonsShape || label->status==My::CircleShape){
        if(label->current!=-1){
            label->shapes.pop_back();
            label->current=-1;
        }
    }
    label->status=s;
//    setCursor(QCursor(QPixmap(":/icons/icons/color-line.png"),-1,-1));


    if(label->current!=-1)emit(selectedChanged(label->current,false));


    label->current=-1;
}

void CentralWInit::keyPressHandle(int k){
    if(k==Qt::Key_Enter || k==Qt::Key_Return){
        if(label->status==My::PolygonsShape || label->status==My::BrushShape || label->status==My::CurveShape){
            if(label->current==-1)return;
            bool isOk;
            QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,"",&isOk);
            if(isOk){
                label->shapes[label->current]->label=text;
                emit(labelAdded(label->shapes[label->current],label->current));
                command->logAdd(label->current);
                label->current=-1;
                return;
            }
        }
        return;
    }
    if(k==Qt::Key_Escape){
        if(label->status==My::PolygonsShape || label->status==My::CircleShape || label->status==My::RectangleShape || label->status==My::BrushShape || label->status==My::CurveShape){
            if(label->current==-1)return;
            else{
                label->shapes.removeLast();
                label->current=-1;
                label->update();
            }
        }
        return;
    }
}

void CentralWInit::changeLabel(){
    if(label->current!=-1){
        bool isOk;
        QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,label->shapes[label->current]->label,&isOk);
        if(isOk){
            command->logChange(label->current,label->shapes[label->current]->label,text);
            label->shapes[label->current]->label=text;       
            emit(labelChanged(label->current,label->shapes[label->current]));         
        }
    }
}

void CentralWInit::deleteLabel(){
    if(label->current!=-1){
        int res=QMessageBox::warning(this,"Delete Label","Are you sure to delete this label?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
        if(res==QMessageBox::Ok){
            qDebug()<<"deleteLabel "<<label->current<<endl;
            emit labelDeleted(label->current);
//            delete label->shapes[label->current];
            trashshapes.append(label->shapes[label->current]);
            command->logDelete(label->current,trashshapes.length()-1);
            label->shapes.removeAt(label->current);
            label->update();
        }
    }
    qDebug()<<"deleteLabel "<<label->current<<endl;
}

void CentralWInit::editBrush(){
    if(label->status==My::BrushShape){
        bool isOk;
        QString text=QInputDialog::getText(this,"Edit Brush","Please input the Brush size",QLineEdit::Normal,QString("%1").arg(label->size),&isOk);
        if(isOk){
            label->size=text.toFloat();
        }
    }
}

void CentralWInit::editColor(){
    QColor c=QColorDialog::getColor(Qt::white,this,"Edit Color");
    c.setAlpha(100);
    label->color=c;
}

void CentralWInit::zoomIn(){
    qDebug()<<"zoom In"<<endl;
    label->zoomLevel*=1.20;
    qDebug()<<label->zoomLevel<<endl;
    label->update();
}

void CentralWInit::zoomOut(){
    qDebug()<<"zoom Out"<<endl;
    label->zoomLevel*=0.8;
    qDebug()<<label->zoomLevel<<endl;
    label->update();
}

void CentralWInit::originalSize(){
    label->zoomLevel=1.0;
    label->update();
}

void CentralWInit::fitWindow(){
    label->zoomLevel=float(sarea->width())/float(label->pixmap->width());
    label->update();
}

void CentralWInit::magnifier(bool b){
    label->MagniFier=b;
    label->update();
}

void CentralWInit::save(const QString& fileName){

    int res=QMessageBox::information(this,"Save File","Are you sure to save the changes ?",QMessageBox::Ok | QMessageBox::Cancel);
    if(res==QMessageBox::Ok){
        My::Parse::write(fileName,label->shapes);
    }
    return;
}

void CentralWInit::detach(){
    delete sarea;
    sarea=nullptr;
    delete command;
    command=nullptr;

    //trashshape有内存泄漏的危险
}

#include"centralwinit3d.h"
#include"mainwindow.h"
#include"label3d.h"
#include"parse.h"
#include<QPointF>
#include<QInputDialog>
#include<QColorDialog>
#include<QDebug>
#include"command3d.h"

void CentralWInit3D::setUp3D(MainWindow *w){
    command3d=new My::Command3D();
    command3d->manager=this;
    sarea=new QScrollArea(w);
    gridlayout=new QGridLayout();
    for(int i=0;i<3;i++){
        sareas.push_back(new QScrollArea(w));
        labels.push_back(new My::Label3D(this,i));
        labels[i]->manager=this;
        sareas[i]->setWidget(labels[i]);
        sareas[i]->setAlignment(Qt::AlignCenter);
        gridlayout->addWidget(sareas[i],i/2,i%2,1,1);
    }
    sarea->setLayout(gridlayout);
    w->setCentralWidget(sarea);
}

//void CentralWInit3D::

void CentralWInit3D::detach(){
//    for(int i=0;i<3;i++){
//        delete sareas[i];
//    }
//    for(int i=0;i<trashshapes.length();i++){
//        delete trashshapes[i];
//    }
//    trashshapes.clear();
//    sareas.clear();
//    labels.clear();
    delete sarea;
    sarea=nullptr;
    delete command3d;
    command3d=nullptr;
}

void CentralWInit3D::parse(const QString &fileName, const QString &labelfileName){
    if(fileName.isEmpty()){
        for(int i=0;i<shapes.length();i++){
            delete shapes[i];
        }
        shapes.clear();
        emit(labelsFinished(shapes));
        for(int i=0;i<3;i++){
            delete labels[i]->pixmap;
            labels[i]->pixmap=new QPixmap();
            for(int j=0;j<labels[i]->shapes.length();j++){
                delete labels[i]->shapes[j];
            }
            labels[i]->shapes.clear();
            labels[i]->update();
        }
        return;
    }
    current=-1;
    for(int i=0;i<3;i++){
        labels[i]->zindex=0;
    }
    My::Parse::parse3D(fileName,list,shapes,labelfileName);
    parseImage();
    emit(labelsFinished(shapes));
}

void CentralWInit3D::parseImage(){
    My::Parse::parseImage(0,list,shapes,labels);

    My::Parse::parseImage(1,list,shapes,labels);

    My::Parse::parseImage(2,list,shapes,labels);

    for(int i=0;i<3;i++){
        labels[i]->update();
    }
}

void CentralWInit3D::changeSelected(){
    QListWidget* lw=qobject_cast<QListWidget*>(sender());
    int temp=lw->count();
    for(int index=0;index<lw->count();index++){
        if(lw->item(index)->isSelected()){shapes[index]->isFill=true;temp=index;}
        else shapes[index]->isFill=false;
    }
    if(temp==lw->count())current=-1;
    else current=temp;
    if(current!=-1)emit selectedChanged(current,true);
    parseImage();
    qDebug()<<"changeSelected "<<current<<endl;
}

void CentralWInit3D::changeShapeLabel(QListWidgetItem *item){
    QListWidget* lw=item->listWidget();
    int index=lw->row(item);
    shapes[index]->isHide=!(item->checkState());
    shapes[index]->label=item->text();
    parseImage();
}

void CentralWInit3D::nextImage(){
    if(Focus==0){
        if(labels[0]->zindex==list.length()-1){
            QMessageBox::information(this,"next Image","This is the last image");
            return;
        }
    }
    if(Focus==1){
        if(labels[1]->zindex==list[0].rows-1){
            QMessageBox::information(this,"next Image","This is the last image");
            return;
        }
    }
    if(Focus==2){
        if(labels[2]->zindex==list[0].cols-1){
            QMessageBox::information(this,"next Image","This is the last image");
            return;
        }
    }
    qDebug()<<Focus<<" "<<labels[Focus]->zindex;
    labels[Focus]->zindex+=1;
    My::Parse::parseImage(Focus,list,shapes,labels);
    labels[Focus]->update();
}

void CentralWInit3D::prevImage(){
    if(Focus==0){
        if(labels[0]->zindex==0){
            QMessageBox::information(this,"prev image","This is the first image");
            return;
        }
    }
    if(Focus==1){
        if(labels[1]->zindex==0){
            QMessageBox::information(this,"prev Image","This is the first image");
            return;
        }
    }
    if(Focus==2){
        if(labels[2]->zindex==0){
            QMessageBox::information(this,"prev Image","This is the first image");
            return;
        }
    }
    qDebug()<<Focus<<" "<<labels[Focus]->zindex;
    labels[Focus]->zindex-=1;
    My::Parse::parseImage(Focus,list,shapes,labels);
    labels[Focus]->update();
}

void CentralWInit3D::sync(int v, My::syncStatus s){
    if(s==My::AddShape){
        addShape(v);
    }
    if(s==My::AddPoint){
        addPoint(v);
    }
    if(s==My::Hovered){
        hovered(v);
    }
    if(s==My::UnHovered){
        unHovered(v);
    }
    if(s==My::Selected){
        selected(v);
    }
    if(s==My::UnSelected){
        unSelected(v);
        for(int i=0;i<3;i++){
            labels[i]->setCursor(Qt::ArrowCursor);
        }
    }
    if(s==My::Remove){
        remove(v);
    }
    if(s==My::ChangePoint){
        changePoint(v);
    }
    if(s==My::ShowMessage){
        showMessage(v);
        return;
    }
    if(s==My::Offset){
        offset(v);
    }
    parseImage();
}

void CentralWInit3D::showMessage(int v){
    MainWindow* w=qobject_cast<MainWindow*>(parent());
    if(v==0){
        int x=labels[0]->cursorX;
        int y=labels[0]->cursorY;
        w->statusBar()->showMessage(QString("Pos: X %1 Y %2 Z %3").arg(x).arg(y).arg(labels[0]->zindex));
    }
    if(v==1){
        int x=labels[1]->cursorX;
        int y=labels[1]->cursorY;
        w->statusBar()->showMessage(QString("Pos: X %1 Y %2 Z %3").arg(x).arg(labels[1]->zindex).arg(int(float(y)/labels[1]->realLength*list.length())));
    }
    if(v==2){
        int x=labels[2]->cursorX;
        int y=labels[2]->cursorY;
        w->statusBar()->showMessage(QString("Pos: X %1 Y %2 Z %3").arg(labels[2]->zindex).arg(list[0].rows-x).arg(int(float(y)/labels[1]->realLength*list.length())));
    }
}

void CentralWInit3D::addShape(int v){
    int size=labels[v]->shapes.length();

    if(shapes.length()<labels[v]->shapes.length()){
        if(v==0){
            if(labels[0]->shapes[size-1]->type==My::Shape::Rectangle){
                My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[0]->shapes[size-1]);
                My::Rectangle3D* rectangle3d=new My::Rectangle3D();
                rectangle3d->points.append(cv::Point3f(rectangle->points[0].x(),rectangle->points[0].y(),labels[0]->zindex));
                rectangle3d->width=rectangle->width;
                rectangle3d->height=rectangle->height;
                rectangle3d->color=rectangle->color;
                shapes.append(rectangle3d);
            }
            if(labels[0]->shapes[size-1]->type==My::Shape::Brush){
                My::Brush* brush=dynamic_cast<My::Brush*>(labels[0]->shapes[size-1]);
                My::Brush3D* brush3d=new My::Brush3D();
                brush3d->points.append(cv::Point3f(brush->points[0].x(),brush->points[0].y(),labels[0]->zindex));
                brush3d->radius.append(brush->radius[0]);
                brush3d->color=brush->color;
                shapes.append(brush3d);
            }
        }

        if(v==1){
            if(labels[1]->shapes[size-1]->type==My::Shape::Rectangle){
                My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[1]->shapes[size-1]);
                My::Rectangle3D* rectangle3d=new My::Rectangle3D();
                rectangle3d->points.append(cv::Point3f(rectangle->points[0].x(),float(labels[1]->zindex)/list[0].rows,rectangle->points[0].y()*list.length()));
                rectangle3d->width=rectangle->width;
                rectangle3d->zspan=rectangle->height*list.length();
                rectangle3d->color=rectangle->color;
                shapes.append(rectangle3d);
            }
            if(labels[1]->shapes[size-1]->type==My::Shape::Brush){
                My::Brush* brush=dynamic_cast<My::Brush*>(labels[1]->shapes[size-1]);
                My::Brush3D* brush3d=new My::Brush3D();
                brush3d->points.append(cv::Point3f(brush->points[0].x(),float(labels[1]->zindex)/list[0].rows,brush->points[0].y()*list.length()));
                brush3d->color=brush->color;
                brush3d->radius.append(brush->radius[0]);
                shapes.append(brush3d);
            }
        }
        if(v==2){
            if(labels[2]->shapes[size-1]->type==My::Shape::Rectangle){
                My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[2]->shapes[size-1]);
                My::Rectangle3D* rectangle3d=new My::Rectangle3D();
//                rectangle3d->points.append(cv::Point3f(labels[2]->zindex/list[0].cols,(1-rectangle->points[0].x()+rectangle->width)*list[0].rows,rectangle->points[0].y()*list.length()));
                rectangle3d->points.append(cv::Point3f(float(labels[2]->zindex)/list[0].cols,(1-rectangle->points[0].x()),rectangle->points[0].y()*list.length()));
                rectangle3d->height=rectangle->width;
                rectangle3d->zspan=rectangle->height*list.length();
                rectangle3d->color=rectangle->color;
                shapes.append(rectangle3d);
            }
            if(labels[2]->shapes[size-1]->type==My::Shape::Brush){
                My::Brush* brush=dynamic_cast<My::Brush*>(labels[2]->shapes[size-1]);
                My::Brush3D* brush3d=new My::Brush3D();
                brush3d->points.append(cv::Point3f(float(labels[2]->zindex)/list[0].cols,(1-brush->points[0].x()),brush->points[0].y()*list.length()));
                brush3d->color=brush->color;
                brush3d->radius.append(brush->radius[0]);
                shapes.append(brush3d);
            }
        }
        return;
    }
    if(shapes.length()==labels[v]->shapes.length()){
        if(labels[v]->shapes[size-1]->type==My::Shape::Rectangle){
            if(v==0){
                My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[0]->shapes[size-1]);
                My::Rectangle3D* rectangle3d=dynamic_cast<My::Rectangle3D*>(shapes.last());
                rectangle3d->width=rectangle->width;
                rectangle3d->height=rectangle->height;
            }
            if(v==1){
                My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[1]->shapes[size-1]);
                My::Rectangle3D* rectangle3d=dynamic_cast<My::Rectangle3D*>(shapes.last());
                rectangle3d->width=rectangle->width;
                rectangle3d->zspan=rectangle->height*list.length();
            }
            if(v==2){
                My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[1]->shapes[size-1]);
                My::Rectangle3D* rectangle3d=dynamic_cast<My::Rectangle3D*>(shapes.last());
                rectangle3d->height=rectangle->width;
                rectangle3d->zspan=rectangle->height*list.length();
            }
        }
        if(labels[v]->shapes[size-1]->type==My::Shape::Brush){
            if(v==0){
                My::Brush* brush=dynamic_cast<My::Brush*>(labels[0]->shapes[size-1]);
                My::Brush3D* brush3d=dynamic_cast<My::Brush3D*>(shapes.last());
                brush3d->points.append(cv::Point3f(brush->points[0].x(),brush->points[0].y(),labels[0]->zindex));
                brush3d->color=brush->color;
                brush3d->radius.append(brush->radius[0]);
            }
            if(v==1){
                My::Brush* brush=dynamic_cast<My::Brush*>(labels[1]->shapes[size-1]);
                My::Brush3D* brush3d=dynamic_cast<My::Brush3D*>(shapes.last());
                brush3d->points.append(cv::Point3f(brush->points[0].x(),float(labels[1]->zindex)/list[0].rows,brush->points[0].y()*list.length()));
                brush3d->color=brush->color;
                brush3d->radius.append(brush->radius[0]);
            }
            if(v==2){
                My::Brush* brush=dynamic_cast<My::Brush*>(labels[2]->shapes[size-1]);
                My::Brush3D* brush3d=dynamic_cast<My::Brush3D*>(shapes.last());
                brush3d->points.append(cv::Point3f(float(labels[2]->zindex)/list[0].cols,(1-brush->points[0].x()),brush->points[0].y()*list.length()));
                brush3d->color=brush->color;
                brush3d->radius.append(brush->radius[0]);
            }
        }

    }
}

void CentralWInit3D::addPoint(int v){
    int size=labels[v]->shapes.length();
    if(labels[v]->shapes[size-1]->type==My::Shape::Brush){
        if(v==0){
            My::Brush* brush=dynamic_cast<My::Brush*>(labels[0]->shapes[size-1]);
            My::Brush3D* brush3d=dynamic_cast<My::Brush3D*>(shapes.last());
            brush3d->points.append(cv::Point3f(brush->points.last().x(),brush->points.last().y(),labels[0]->zindex));
            brush3d->color=brush->color;
            brush3d->radius.append(brush->radius.last());
        }
        if(v==1){
            My::Brush* brush=dynamic_cast<My::Brush*>(labels[1]->shapes[size-1]);
            My::Brush3D* brush3d=dynamic_cast<My::Brush3D*>(shapes.last());
            brush3d->points.append(cv::Point3f(brush->points.last().x(),float(labels[1]->zindex)/list[0].rows,brush->points.last().y()*list.length()));
            brush3d->color=brush->color;
            brush3d->radius.append(brush->radius.last());
        }
        if(v==2){
            My::Brush* brush=dynamic_cast<My::Brush*>(labels[2]->shapes[size-1]);
            My::Brush3D* brush3d=dynamic_cast<My::Brush3D*>(shapes.last());
            brush3d->points.append(cv::Point3f(float(labels[2]->zindex)/list[0].cols,(1-brush->points.last().x()),brush->points.last().y()*list.length()));
            brush3d->color=brush->color;
            brush3d->radius.append(brush->radius.last());
        }
    }
}

void CentralWInit3D::changePoint(int v){
    int size=labels[v]->shapes.length();
    if(v==0){
        if(labels[0]->shapes[size-1]->type==My::Shape::Rectangle){
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[0]->shapes[size-1]);
            My::Rectangle3D* rectangle3d=dynamic_cast<My::Rectangle3D*>(shapes.last());
            rectangle3d->width=rectangle->width;
            rectangle3d->height=rectangle->height;
            rectangle3d->color=rectangle->color;
        }
    }
    if(v==1){
        if(labels[1]->shapes[size-1]->type==My::Shape::Rectangle){
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[1]->shapes[size-1]);
            My::Rectangle3D* rectangle3d=dynamic_cast<My::Rectangle3D*>(shapes.last());
            rectangle3d->width=rectangle->width;
            rectangle3d->zspan=rectangle->height*list.length();
            rectangle3d->color=rectangle->color;
        }
    }
    if(v==2){
        if(labels[2]->shapes[size-1]->type==My::Shape::Rectangle){
            My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[2]->shapes[size-1]);
            My::Rectangle3D* rectangle3d=dynamic_cast<My::Rectangle3D*>(shapes.last());
            rectangle3d->height=-rectangle->width;
            rectangle3d->zspan=rectangle->height*list.length();
            rectangle3d->color=rectangle->color;
        }
    }
}

void CentralWInit3D::hovered(int v){
    shapes[current]->isHover=true;

}

void CentralWInit3D::unHovered(int v){
    shapes[current]->isHover=false;

}

void CentralWInit3D::selected(int v){
    emit(selectedChanged(current,true));
}

void CentralWInit3D::unSelected(int v){
    emit(selectedChanged(current,false));
}

void CentralWInit3D::offset(int v){
    if(v==0){
        QVector<cv::Point3f>& points=shapes[current]->points;
        for(int i=0;i<points.length();i++){
            points[i].x+=double(labels[v]->xOffset);
            points[i].y+=double(labels[v]->yOffset);
        }
    }
    if(v==1){
        QVector<cv::Point3f>& points=shapes[current]->points;
        for(int i=0;i<points.length();i++){
            points[i].x+=double(labels[v]->xOffset);
            points[i].z+=double(labels[v]->yOffset*list.length());
        }
    }
    if(v==2){
        QVector<cv::Point3f>& points=shapes[current]->points;
        for(int i=0;i<points.length();i++){
            points[i].y-=double(labels[v]->xOffset);
            points[i].z+=double(labels[v]->yOffset*list.length());
        }
    }
}

void CentralWInit3D::remove(int v){
    int size=shapes.length();
    delete  shapes[size-1];
    shapes.removeLast();
    for(int i=0;i<3;i++){
        if(labels[i]->shapes.length()==size){
            delete labels[i]->shapes.last();
            labels[i]->shapes.removeLast();
        }
    }
}

void CentralWInit3D::keyPressHandle(int k){
    if(k==Qt::Key_Enter || k==Qt::Key_Return){
        if(status==My::BrushShape || status==My::RectangleShape){
            if(current==-1)return;
            bool isOk;
            QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,"",&isOk);
            if(isOk){
                shapes[current]->label=text;
                emit(labelAdded(shapes[current],current));
                command3d->logAdd(current);
                current=-1;
                for(int i=0;i<3;i++){
                    labels[i]->isFinished=false;
                }
                return;
            }
        }
        return;
    }
    if(k==Qt::Key_Escape){
        if(status==My::RectangleShape || status==My::BrushShape){
            if(current==-1)return;
            else{
                shapes.removeLast();
                current=-1;
                for(int i=0;i<3;i++){
                    labels[i]->isFinished=false;
                }
                parseImage();
            }
        }
        return;
    }
}

void CentralWInit3D::createShape(My::shapeStatus s){
    if(status==My::BrushShape || status==My::RectangleShape){
        if(current!=-1){
            shapes.pop_back();
            current=-1;
            for(int i=0;i<3;i++){
                labels[i]->isFinished=false;
            }
            parseImage();
        }
    }
    status=s;
    if(current!=-1)emit(selectedChanged(current,false));
    current=-1;
}

void CentralWInit3D::changeLabel(){
    if(current!=-1){
        bool isOk;
        QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,shapes[current]->label,&isOk);
        if(isOk){
            command3d->logChange(current,shapes[current]->label,text);
            shapes[current]->label=text;
            emit(labelChanged(current,shapes[current])); 
        }
    }
}

void CentralWInit3D::deleteLable(){
    if(current!=-1){
        int res=QMessageBox::warning(this,"Delete Label","Are you sure to delete this label?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
        if(res==QMessageBox::Ok){
            int currentTemp=current;
            emit labelDeleted(current);
            trashshapes.append(shapes[currentTemp]);
//            delete shapes[currentTemp];
            command3d->logDelete(current,trashshapes.length()-1);
            shapes.removeAt(currentTemp);
            parseImage();
        }
    }
    qDebug()<<"delete Label "<<current<<endl;
}

void CentralWInit3D::editBrush(){
    if(status==My::BrushShape){
        bool isOk;
        QString text=QInputDialog::getText(this,"Edit Brush","Please input the Brush size",QLineEdit::Normal,QString("%1").arg(size),&isOk);
        if(isOk){
            size=text.toFloat();
        }
    }
}

void CentralWInit3D::editColor(){
    QColor c=QColorDialog::getColor(Qt::white,this,"Edit Color");
    c.setAlpha(100);
    color=c;
}

void CentralWInit3D::magnifier(bool b){
    if(b) labels[Focus]->MagniFier=true;
    else  labels[Focus]->MagniFier=false;
    labels[Focus]->update();
}

void CentralWInit3D::save(QString fileName){
    int res=QMessageBox::information(this,"Save File","Are you sure to save the changes ?",QMessageBox::Ok | QMessageBox::Cancel);
    if(res==QMessageBox::Ok){
        My::Parse::write3D(fileName,shapes);
    }
    return;
}

void CentralWInit3D::zoomIn(){
    labels[Focus]->zoomLevel*=1.2;
    labels[Focus]->update();
}

void CentralWInit3D::zoomOut(){
    labels[Focus]->zoomLevel*=0.8;
    labels[Focus]->update();
}

void CentralWInit3D::originalSize(){
    labels[Focus]->zoomLevel=1;
    labels[Focus]->update();
}

void CentralWInit3D::fitWindow(){
    labels[Focus]->zoomLevel=float(sareas[Focus]->width())/labels[Focus]->width();
    labels[Focus]->update();
}

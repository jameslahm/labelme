/*!
* \file centralwinit3d.cpp
* \brief 3d中心组件类
* \author 王澳
* \date 2019.09
*/

#include<QPointF>
#include<QInputDialog>
#include<QColorDialog>
#include<QDebug>
#include"CentralWInit3D.h"
#include"mainwindow.h"
#include"Shape3D.h"
#include"Label3D.h"
#include"Parse.h"
#include"Rectangle.h"
#include"Rectangle3D.h"
#include"Brush.h"
#include"Brush3D.h"
#include"Command3D.h"

/// \brief 构造函数，初始化各个组件
CentralWInit3D::CentralWInit3D(QWidget* w):My::CentralWidget (w){

   //初始化command3d
   command3d=new My::Command3D();
   command3d->manager=this;

   //初始化中心滚动区域
   sarea=new QScrollArea();
   gridlayout=new QGridLayout();

   //添加三个视角的图片区域
   for(int i=0;i<3;i++){
       sareas.push_back(new QScrollArea(w));
       labels.push_back(new My::Label3D(this,i));
       labels[i]->manager=this;
       sareas[i]->setWidget(labels[i]);
       sareas[i]->setAlignment(Qt::AlignCenter);
       gridlayout->addWidget(sareas[i],i/2,i%2,1,1);
   }
}

/// \brief 3d中心组件的初始化函数
void CentralWInit3D::setUp(QMainWindow *w){
    sarea->setLayout(gridlayout);
    w->setCentralWidget(sarea);
}

/// \brief 3d组件的解析函数
///
/// 当由一些列图片读入时，会调用该解析函数
void CentralWInit3D::parse(const QString &fileName, const QString &labelfileName){

    current=-1;

    //初始化各个视角的zindex为0
    for(int i=0;i<3;i++){
        labels[i]->zindex=0;
    }
    //解析
    My::Parse::parse3D(fileName,list,shapes,labelfileName);

    //更新3个视角的图片和标注信息
    parseImage();

    //清空trashshape
    trashshapes.clear();

    //发送信号，标签列表组件同步
    QList<My::Shape*> temp;
    for(int i=0;i<shapes.length();i++){
        temp.push_back(shapes[i]);
    }

    emit(labelsFinished(temp));
}

/// \brief 3d组件的三个视角图片同时更新函数
void CentralWInit3D::parseImage(){
    My::Parse::parseImage(0,list,shapes,labels);

    My::Parse::parseImage(1,list,shapes,labels);

    My::Parse::parseImage(2,list,shapes,labels);

    for(int i=0;i<3;i++){
        labels[i]->update();

    }
}

/// \brief 改变选中的标注
void CentralWInit3D::changeSelected(){

    //获取标签列表组件
    QListWidget* lw=qobject_cast<QListWidget*>(sender());
    int temp=lw->count();

    //被选中的改变为选中，反之亦然
    for(int index=0;index<lw->count();index++){
        if(lw->item(index)->isSelected()){shapes[index]->isFill=true;temp=index;}
        else shapes[index]->isFill=false;
    }

    //没有被选中的，则更新为-1
    if(temp==lw->count())current=-1;
    else current=temp;

    //发射信号，mainwindow同步更新
    if(current!=-1)emit selectedChanged(current,true);

    //更新三个视角图片
    parseImage();

}

/// \brief 当标签列表的item发生改变时，会调用该槽函数
void CentralWInit3D::changeShapeLabel(QListWidgetItem *item){
    QListWidget* lw=item->listWidget();
    int index=lw->row(item);

    //更新是否隐藏以及标签文字
    shapes[index]->isHide=!(item->checkState());
    shapes[index]->label=item->text();

    //更新三个视角图片
    parseImage();
}

/// \brief 3d状态下，下一张图片命令会由mainwindow调用该函数处理
void CentralWInit3D::nextImage(){

    //若视角为0
    if(Focus==0){
        if(labels[0]->zindex==list.length()-1){
            QMessageBox::information(this,"next Image","This is the last image");
            return;
        }
    }

    //若视角为1
    if(Focus==1){
        if(labels[1]->zindex==list[0].rows-1){
            QMessageBox::information(this,"next Image","This is the last image");
            return;
        }
    }

    //若视角为2
    if(Focus==2){
        if(labels[2]->zindex==list[0].cols-1){
            QMessageBox::information(this,"next Image","This is the last image");
            return;
        }
    }

    //当前视角的顺序加1
    labels[Focus]->zindex+=1;

    //更新对应视角图片
    My::Parse::parseImage(Focus,list,shapes,labels);
    labels[Focus]->update();
}

/// \brief 3d状态下，上一张图片命令会由mainwindow调用该函数处理
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

    labels[Focus]->zindex-=1;

    My::Parse::parseImage(Focus,list,shapes,labels);
    labels[Focus]->update();
}

/// \brief 3d组件的同步函数
///
/// 这是其内部各视角操作标注的统一接口，v代表当前视角，s代表当前的同步状态
void CentralWInit3D::sync(int v, My::syncStatus s){

    //若为增加标注形状
    if(s==My::AddShape){
        addShape(v);
    }

    //若为增加像素点
    if(s==My::AddPoint){
        addPoint(v);
    }

    //若当前鼠标在标注形状内
    if(s==My::Hovered){
        hovered(v);
    }

    //若当前鼠标不在标注形状内
    if(s==My::UnHovered){
        unHovered(v);
    }

    //若当前形状为被选中
    if(s==My::Selected){
        selected(v);
    }

    //若当前形状显示未选中
    if(s==My::UnSelected){
        unSelected(v);
        for(int i=0;i<3;i++){
            labels[i]->setCursor(Qt::ArrowCursor);
        }
    }

    //若当前形状被移除
    if(s==My::Remove){
        remove(v);
    }

    //若改变绘制标注形状的像素点
    if(s==My::ChangePoint){
        changePoint(v);
    }

    //若为在状态栏显示坐标
    if(s==My::ShowMessage){
        showMessage(v);
        return;
    }

    //若为标注形状位置改变
    if(s==My::Offset){
        offset(v);
    }

    //更新三个视角的图片
    parseImage();
}

/// \brief 状态栏显示坐标信息
void CentralWInit3D::showMessage(int v){

    //获取主窗口，便于调用状态栏
    MainWindow* w=qobject_cast<MainWindow*>(parent());

    //第一个视角
    if(v==0){
        int x=labels[0]->cursorX;
        int y=labels[0]->cursorY;
        w->statusBar()->showMessage(QString("Pos: X %1 Y %2 Z %3").arg(x).arg(y).arg(labels[0]->zindex));
    }

    //第二个视角
    if(v==1){
        int x=labels[1]->cursorX;
        int y=labels[1]->cursorY;
        w->statusBar()->showMessage(QString("Pos: X %1 Y %2 Z %3").arg(x).arg(labels[1]->zindex).arg(int(float(y)/labels[1]->realLength*list.length())));
    }

    //第三个视角
    if(v==2){
        int x=labels[2]->cursorX;
        int y=labels[2]->cursorY;
        w->statusBar()->showMessage(QString("Pos: X %1 Y %2 Z %3").arg(labels[2]->zindex).arg(list[0].rows-x).arg(int(float(y)/labels[1]->realLength*list.length())));
    }
}

/// \brief 添加标注形状
void CentralWInit3D::addShape(int v){

    //同步视角的标注形状的长度
    int size=labels[v]->shapes.length();

    //若当前视角的标注形状长度比3d组件的3d标注形状长度长，则说明是新创建的标注形状
    if(shapes.length()<labels[v]->shapes.length()){

        //根据当前视角以及添加的标注形状，进行添加并更新
        //第一个视角
        if(v==0){

            //添加矩形
            if(labels[0]->shapes[size-1]->type==My::Shape::Rectangle){
                My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[0]->shapes[size-1]);
                My::Rectangle3D* rectangle3d=new My::Rectangle3D();
                rectangle3d->points.append(cv::Point3f(rectangle->points[0].x(),rectangle->points[0].y(),labels[0]->zindex));
                rectangle3d->width=rectangle->width;
                rectangle3d->height=rectangle->height;
                rectangle3d->color=rectangle->color;
                shapes.append(rectangle3d);
            }

            //添加画刷（分割标注）
            if(labels[0]->shapes[size-1]->type==My::Shape::Brush){
                My::Brush* brush=dynamic_cast<My::Brush*>(labels[0]->shapes[size-1]);
                My::Brush3D* brush3d=new My::Brush3D();
                brush3d->points.append(cv::Point3f(brush->points[0].x(),brush->points[0].y(),labels[0]->zindex));
                brush3d->radius.append(brush->radius[0]);
                brush3d->color=brush->color;
                shapes.append(brush3d);
            }
        }

        //第二个视角
        if(v==1){

            //添加矩形
            if(labels[1]->shapes[size-1]->type==My::Shape::Rectangle){
                My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[1]->shapes[size-1]);
                My::Rectangle3D* rectangle3d=new My::Rectangle3D();
                rectangle3d->points.append(cv::Point3f(rectangle->points[0].x(),float(labels[1]->zindex)/list[0].rows,rectangle->points[0].y()*list.length()));
                rectangle3d->width=rectangle->width;
                rectangle3d->zspan=rectangle->height*list.length();
                rectangle3d->color=rectangle->color;
                shapes.append(rectangle3d);
            }

            //添加画刷
            if(labels[1]->shapes[size-1]->type==My::Shape::Brush){
                My::Brush* brush=dynamic_cast<My::Brush*>(labels[1]->shapes[size-1]);
                My::Brush3D* brush3d=new My::Brush3D();
                brush3d->points.append(cv::Point3f(brush->points[0].x(),float(labels[1]->zindex)/list[0].rows,brush->points[0].y()*list.length()));
                brush3d->color=brush->color;
                brush3d->radius.append(brush->radius[0]);
                shapes.append(brush3d);
            }
        }

        //第三个视角
        if(v==2){

            //添加矩形
            if(labels[2]->shapes[size-1]->type==My::Shape::Rectangle){
                My::Rectangle* rectangle=dynamic_cast<My::Rectangle*>(labels[2]->shapes[size-1]);
                My::Rectangle3D* rectangle3d=new My::Rectangle3D();
                rectangle3d->points.append(cv::Point3f(float(labels[2]->zindex)/list[0].cols,(1-rectangle->points[0].x()),rectangle->points[0].y()*list.length()));
                rectangle3d->height=rectangle->width;
                rectangle3d->zspan=rectangle->height*list.length();
                rectangle3d->color=rectangle->color;
                shapes.append(rectangle3d);
            }

            //添加画刷
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

    //若当前视角的标注形状与3d组件的标注形状长度一样，说明已正在创建过程中，且已经在其他视角创建了形状
    if(shapes.length()==labels[v]->shapes.length()){

        //若为矩形
        if(labels[v]->shapes[size-1]->type==My::Shape::Rectangle){

            //分视角讨论
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

        //若为画刷，则为添加像素点
        if(labels[v]->shapes[size-1]->type==My::Shape::Brush){

            //分视角讨论
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

/// \brief 添加画刷形状，进行分割标注
void CentralWInit3D::addPoint(int v){
    int size=labels[v]->shapes.length();

    //必定为创建画刷
    if(labels[v]->shapes[size-1]->type==My::Shape::Brush){

        //分视角讨论
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

/// \brief 改变像素点
void CentralWInit3D::changePoint(int v){
    int size=labels[v]->shapes.length();

    //必定为创建矩形，分视角讨论
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

/// \brief 当前鼠标在标注形状内
void CentralWInit3D::hovered(int v){
    shapes[current]->isHover=true;

}

/// \brief 当前鼠标不在标注形状内
void CentralWInit3D::unHovered(int v){
    shapes[current]->isHover=false;

}

/// \brief 当前形状被选中
void CentralWInit3D::selected(int v){
    emit(selectedChanged(current,true));
}

/// \brief 当前形状未被选中
void CentralWInit3D::unSelected(int v){
    emit(selectedChanged(current,false));
}

/// \brief 当前标注形状发生移动
void CentralWInit3D::offset(int v){

    //分视角进行讨论
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

/// \brief 移除标注形状，当取消创建的形状时
void CentralWInit3D::remove(int v){
    int size=shapes.length();

    //移除最后一个标注形状，
    delete  shapes[size-1];
    shapes.removeLast();

    for(int i=0;i<3;i++){
        if(labels[i]->shapes.length()==size){
            delete labels[i]->shapes.last();
            labels[i]->shapes.removeLast();
        }
    }
}

/// \brief 处理按键事件
///
/// 当为回车键和esc键时，mainwindow会交由该函数处理
void CentralWInit3D::keyPressHandle(int k){
    if(k==Qt::Key_Enter || k==Qt::Key_Return){

        //若为正在创建，且由形状被添加，则添加该标注
        if(status==My::BrushShape || status==My::RectangleShape){
            if(current==-1)return;

            bool isOk;
            QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,"",&isOk);
            if(isOk){
                shapes[current]->label=text;

                //发送标注添加信号，标签列表进行同步
                emit(labelAdded(shapes[current],current));

                //命令栈记录添加命令
                command3d->logAdd(current);

                //初始化
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

        //若为正在创建，且由形状被创建，则移除
        if(status==My::RectangleShape || status==My::BrushShape){
            if(current==-1)return;
            else{

                //移除
                shapes.removeLast();

                //初始化
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

/// \brief 创建标注形状
///
/// 3d状态下在mainwidow中各种创建标注形状命令被触发时，均会由该函数处理
void CentralWInit3D::createShape(My::shapeStatus s){

    //若正在创建且由形状被添加，则移除正在创建的形状
    if(status==My::BrushShape || status==My::RectangleShape){
        if(current!=-1){
            shapes.pop_back();

            //初始化
            current=-1;
            for(int i=0;i<3;i++){
                labels[i]->isFinished=false;
            }
            parseImage();
        }
    }

    status=s;

    //若current不为-1，则发送选中取消信号
    if(current!=-1)emit(selectedChanged(current,false));

    current=-1;
}


/// \brief 编辑标注
void CentralWInit3D::editLabel(){

    //若current不为-1，则提示用户键入新标注文字
    if(current!=-1){
        bool isOk;
        QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,shapes[current]->label,&isOk);
        if(isOk){

            //命令栈记录
            command3d->logChange(current,shapes[current]->label,text);

            //更改标注文字，并发送信号，标签列表同步更改
            shapes[current]->label=text;
            emit(labelChanged(current,shapes[current])); 
        }
    }
}

/// \brief 删除标注
void CentralWInit3D::deleteLabel(){

    //若current不为-1，则提醒用户是否要删除
    if(current!=-1){

        //弹出对话框
        int res=QMessageBox::warning(this,"Delete Label","Are you sure to delete this label?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
        if(res==QMessageBox::Ok){

            //发送删除信号，标签列表同步更改
            emit labelDeleted(current);
            trashshapes.append(shapes[current]);

            //命令栈记录
            command3d->logDelete(current,trashshapes.length()-1);
            shapes.removeAt(current);

            //更新图片
            parseImage();
        }
    }

}

/// \brief 编辑画刷大小
void CentralWInit3D::editBrush(){
    if(status==My::BrushShape){

        //用户键入画刷大小并更改
        bool isOk;
        QString text=QInputDialog::getText(this,"Edit Brush","Please input the Brush size",QLineEdit::Normal,QString("%1").arg(size),&isOk);
        if(isOk){
            size=text.toFloat();
        }
    }
}

/// \brief 编辑颜色
void CentralWInit3D::editColor(){
    QColor c=QColorDialog::getColor(Qt::white,this,"Edit Color");
    c.setAlpha(100);
    color=c;
}

/// \brief 是否开启放大镜
void CentralWInit3D::magnifier(bool b){
    if(b) labels[Focus]->MagniFier=true;
    else  labels[Focus]->MagniFier=false;
    labels[Focus]->update();
}

/// \brief 保存文件
void CentralWInit3D::save(const QString& fileName){
    int res=QMessageBox::information(this,"Save File","Are you sure to save the changes ?",QMessageBox::Ok | QMessageBox::Cancel);
    if(res==QMessageBox::Ok){
        
        //调用Parse的写入文件函数
        My::Parse::write3D(fileName,shapes);
    }
    return;
}

/// \brief 放大功能
void CentralWInit3D::zoomIn(){
    labels[Focus]->zoomLevel*=1.2;
    labels[Focus]->update();
}

/// \brief 缩小功能
void CentralWInit3D::zoomOut(){
    labels[Focus]->zoomLevel*=0.8;
    labels[Focus]->update();
}

/// \brief 原始大小
void CentralWInit3D::originalSize(){
    labels[Focus]->zoomLevel=1;
    labels[Focus]->update();
}

/// \brief 宽度适应窗口
void CentralWInit3D::fitWindow(){
    labels[Focus]->zoomLevel=float(sareas[Focus]->width())/labels[Focus]->width();
    labels[Focus]->update();
}

/// \brief 关闭文件
void CentralWInit3D::closeFile(){

    //初始化label图片为空图片
    for(int i=0;i<3;i++){
        delete labels[i]->pixmap;
        labels[i]->pixmap=new QPixmap();
        for(int j=0;j<labels[i]->shapes.length();j++){
            delete labels[i]->shapes[j];
        }
        labels[i]->shapes.clear();
        labels[i]->update();
    }

    //清空各个视角的标注形状
    for(int i=0;i<shapes.length();i++){
        delete shapes[i];
        shapes[i]=nullptr;
    }
    shapes.clear();

    //清空命令栈用标注形状
//    for(int i=0;i<trashshapes.length();i++){
//        if(trashshapes[i]!=nullptr)
//            delete trashshapes[i];
//    }
    trashshapes.clear();

    //发送信号，标签列表组件同步
    QList<My::Shape*> temp;
    for(int i=0;i<shapes.length();i++){
        temp.push_back(shapes[i]);
    }
    emit(labelsFinished(temp));
}

void CentralWInit3D::hidePolygons(){
    foreach(My::Shape3D* shape,shapes){
        shape->isHide=true;
    }
    parseImage();
}

void CentralWInit3D::showPolygons(){
    foreach(My::Shape3D* shape,shapes){
        shape->isHide=false;
    }
    parseImage();
}

CentralWInit3D::~CentralWInit3D(){
    delete command3d;
    delete sarea;
}

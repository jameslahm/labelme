/*!
* \file centralwinit2D.cpp
* \brief 2d中心组件类
* \author 王澳
* \date 2019.09
*/

#include<QJsonDocument>
#include<QJsonParseError>
#include<QColor>
#include<QPainter>
#include<QInputDialog>
#include<QKeyEvent>
#include<QColorDialog>
#include<math.h>
#include"mainwindow.h"
#include"Shape2D.h"
#include"Parse.h"
#include"Label.h"
#include"Command2D.h"
#include"CentralWInit2D.h"


/// \brief 2d中心组件构造函数
CentralWInit2D::CentralWInit2D(QWidget* w):My::CentralWidget (w){
    
    //初始化command
    command=new My::Command2D();
    command->manager=this;
    
    //初始化中心滚动区域
    sarea=new QScrollArea();
    
    //初始化label
    label=new My::Label(sarea);
    label->manager=this;
}

/// \brief 2d中心组件初始化函数
void CentralWInit2D::setUp(QMainWindow *w){
    w->setCentralWidget(sarea);
    sarea->setWidget(label);
    sarea->setAlignment(Qt::AlignCenter);
}

/// \brief 2d中心组件的解析函数
///
/// 在2d模式下，有文件读入时，会调用解析函数，初始化label中的图片以及标注等信息
void CentralWInit2D::parse(const QString &fileName,const QString &labelfileName){
    label->status=My::NoShape;//初始为noshape状态
    label->current=-1;//当前标注初始为-1，即不存在
    
    //加载图片
    label->pixmap->load(fileName);
    
    //调用Parse的静态解析函数，初始化标注形状
    My::Parse::parse(fileName,label->shapes,labelfileName);
    
    //清空trashshape
    trashshapes.clear();

    //更新label
    label->update();
      
    //发送解析完毕信号，初始化标签列表
    QList<My::Shape*> temp;
    for(int i=0;i<label->shapes.length();i++){
        temp.push_back(label->shapes[i]);
    }


    emit labelsFinished(temp);
}

/// \brief 改变标注状态
///
/// 当标签列表中的item发生改变时，便会调用该槽函数
void CentralWInit2D::changeShapeLabel(QListWidgetItem *item){
    
    //获得标签列表组件
    QListWidget* lw=item->listWidget();
    
    //定位改变item的位置
    int index=lw->row(item);
    
    //同步item的状态，如被隐藏，以及标注文字被改变等
    label->shapes[index]->isHide=!(item->checkState());
    label->shapes[index]->label=item->text();
    
    //更新
    label->update();
}

/// \brief 改变当前被选中的标注
///
/// 当标签列表中选中发生改变时，会调用该槽函数
void CentralWInit2D::changeSelected(){
    
    //获取标签列表组件
    QListWidget* lw=qobject_cast<QListWidget*>(sender());
    int temp=lw->count();
    
    //同步选中状态，若被选中，则改变形状的填充状态为被选中，同时改变当前标注值即current。若不被选中，则反之
    for(int index=0;index<lw->count();index++){
        if(lw->item(index)->isSelected()){
            label->shapes[index]->isFill=true;
            temp=index;
        }
        else label->shapes[index]->isFill=false;
    }
    
    //若无被选中的标注，则更新current为-1；反之则为被选中的序号
    if(temp==lw->count())label->current=-1;
    else label->current=temp;
    
    //改变current的同时，发出信号，使mainwindow改变为Focus状态，该状态下可对标注进行响应的更改
    if(label->current!=-1)emit selectedChanged(label->current,true);
    
    //更新
    label->update();

}


/// \brief 创建标注形状
///
/// 当触发创建各种形状时，会经过mainwindow调用该函数
void CentralWInit2D::createShape(My::shapeStatus s){
    
    //若是在创建形状的状态下改变创建的形状类型，若之前有未创建完的标注，则删掉
    if(label->status==My::BrushShape || label->status==My::RectangleShape || label->status==My::PolygonsShape || label->status==My::CircleShape){
        if(label->current!=-1){
            label->shapes.pop_back();
            label->current=-1;
        }
    }
    
    //同步更改label的状态
    label->status=s;
    
    //如果之前是选中状态即Focus状态，则发射信号，使mainwindow进入创建状态
    if(label->current!=-1)emit(selectedChanged(label->current,false));

    //更改current为-1；
    label->current=-1;
}

/// \brief 处理创建标注过程中的按键事件
///
/// 当是回车键和esc键时，mainwindow会将事件转发到这里来进行处理
void CentralWInit2D::keyPressHandle(int k){
    
    if(k==Qt::Key_Enter || k==Qt::Key_Return){
        
        //如果是创建形状状态
        if(label->status==My::PolygonsShape || label->status==My::BrushShape || label->status==My::CurveShape){
            if(label->current==-1)return;
            
            //如果当前已创建标注形状
            bool isOk;
            
            //提醒用户键入标注文字
            QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,"",&isOk);
            
            if(isOk){
                
                //添加标注，同时发射信号，使标签列表组件同步的增加标注
                label->shapes[label->current]->label=text;
                emit(labelAdded(label->shapes[label->current],label->current));
                command->logAdd(label->current);
                
                //更改current为-1
                label->current=-1;
                return;
            }
        }
        return;
    }
    if(k==Qt::Key_Escape){
        
        //如果是在创建形状过程中
        if(label->status==My::PolygonsShape || label->status==My::CircleShape || label->status==My::RectangleShape || label->status==My::BrushShape || label->status==My::CurveShape){
            if(label->current==-1)return;
            
            //如果之前有创建的形状，则删掉
            else{
                label->shapes.removeLast();
                label->current=-1;
                label->update();
            }
        }
        return;
    }
}

/// \brief 改变标注文字
///
/// 当editlabel动作触发时，会经由mainwindow调用该函数
void CentralWInit2D::editLabel(){
    if(label->current!=-1){
        
        //用户键入想要更改的标注文字
        bool isOk;
        QString text=QInputDialog::getText(this,"label me!","Please input the label",QLineEdit::Normal,label->shapes[label->current]->label,&isOk);
        if(isOk){
            
            //命令栈记录当前更改
            command->logChange(label->current,label->shapes[label->current]->label,text);
            
            label->shapes[label->current]->label=text;     
            
            //发射label更改信号，使标签列表同步更改
            emit(labelChanged(label->current,label->shapes[label->current]));         
        }
    }
}

/// \brief 删除标注
///
/// 当deletepolygons动作被触发时，会经由mainwindow调用该函数
void CentralWInit2D::deleteLabel(){
    
    //若当前标注不为-1时，才能进行删除
    if(label->current!=-1){
        
        //提醒用户是否要删除
        int res=QMessageBox::warning(this,"Delete Label","Are you sure to delete this label?",QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);
        
        if(res==QMessageBox::Ok){
            
            //发射标注删除信号，使标签列表同步删除
            emit labelDeleted(label->current);
            
            //命令栈所用标注形状添加被删除的形状并记录
            trashshapes.append(label->shapes[label->current]);
            command->logDelete(label->current,trashshapes.length()-1);
            
            //移除标注，（不delete）
            label->shapes.removeAt(label->current);
            label->update();
        }
    }
}

/// \brief 编辑画刷大小
void CentralWInit2D::editBrush(){
    
    //当前状态为创建画刷形状
    if(label->status==My::BrushShape){
        
        //用户键入画刷大小
        bool isOk;
        QString text=QInputDialog::getText(this,"Edit Brush","Please input the Brush size",QLineEdit::Normal,QString("%1").arg(label->size),&isOk);
        if(isOk){
            
            //改变当前画刷大小
            label->size=text.toFloat();
        }
    }
}

/// \brief 编辑颜色
void CentralWInit2D::editColor(){
    
    //用户选择颜色
    QColor c=QColorDialog::getColor(Qt::white,this,"Edit Color");
    c.setAlpha(100);
    
    //改变当前颜色
    label->color=c;
}

/// \brief 放大功能
void CentralWInit2D::zoomIn(){
    
    //默认每次放大1.2倍
    label->zoomLevel*=1.20;
    
    label->update();
}

/// \brief 缩小功能
void CentralWInit2D::zoomOut(){
    
    //默认每次缩小0.8倍
    label->zoomLevel*=0.8;
    
    label->update();
}

/// \brief 恢复原始大小
void CentralWInit2D::originalSize(){
    //恢复图片原始大小，即改为1
    label->zoomLevel=1.0;
    
    label->update();
}

/// \brief 宽度适应主窗口
void CentralWInit2D::fitWindow(){
    //使图片宽度适应主窗口宽度
    label->zoomLevel=float(sarea->width())/float(label->pixmap->width());
    
    label->update();
}

/// \brief 放大镜功能
void CentralWInit2D::magnifier(bool b){
    //同步放大镜状态：是否开启
    label->MagniFier=b;
    
    label->update();
}

/// \brief 保存文件
void CentralWInit2D::save(const QString& fileName){
    
    //提醒用户是否要保存
    int res=QMessageBox::information(this,"Save File","Are you sure to save the changes ?",QMessageBox::Ok | QMessageBox::Cancel);
    if(res==QMessageBox::Ok){
        
        //调用Parse的函数，把新的标注信息写入文件中
        My::Parse::write(fileName,label->shapes);
    }
    return;
}


/// \brief 关闭文件
void CentralWInit2D::closeFile(){
    
    //初始化label的图片为空图片
    delete label->pixmap;
    label->pixmap=new QPixmap();
    
    //清空标注形状
    for(int i=0;i<label->shapes.length();i++){
        delete label->shapes[i];
        label->shapes[i]=nullptr;
    }
    label->shapes.clear();
    
    //清空撤销、重做用标注形状
//    for(int i=0;i<trashshapes.length();i++){
//        if(trashshapes[i]!=nullptr)
//            delete trashshapes[i];
//    }
    trashshapes.clear();
    
    //发送解析完毕信号，初始化标签列表
    QList<My::Shape*> temp;
    for(int i=0;i<label->shapes.length();i++){
        temp.push_back(label->shapes[i]);
    }

    emit(labelsFinished(temp));
    
    label->update();
    
    return;
}

/// \brief 隐藏标注
void CentralWInit2D::hidePolygons(){
    foreach(My::Shape2D* shape,label->shapes){
        shape->isHide=true;
    }
    label->update();
}

/// \brief 显示标注
void CentralWInit2D::showPolygons(){
    foreach(My::Shape2D* shape,label->shapes){
        shape->isHide=false;
    }
    label->update();
}

CentralWInit2D::~CentralWInit2D(){
    delete command;
    delete label;
    delete sarea;
}

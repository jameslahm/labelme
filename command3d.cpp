
#include"label3d.h"
#include"mainwindow.h"
#include<QObject>
#include"command3d.h"

void My::Command3D::undo(){
    if(undoCommands.count()==0)return;
    QString s=undoCommands.top();
    undoCommands.pop_back();
    QStringList list=s.split(" ");
    if(list[0]=="Add"){

        int index=list[1].toInt();
        manager->trashshapes.append(manager->shapes[index]);
        manager->shapes.removeAt(index);
        QString str=QString("Add %1 %2").arg(index).arg(manager->trashshapes.length()-1);
        redoCommands.push_back(str);
        emit(manager->labelDeleted(index));
        manager->parseImage();
    }
    if(list[0]=="Delete"){
        int index=list[1].toInt();
        int id=list[2].toInt();
        manager->shapes.insert(index,manager->trashshapes[id]);
        QString str=QString("Delete %1").arg(index);
        redoCommands.push_back(str);
        emit(manager->labelAdded(manager->trashshapes[id],index));
        manager->parseImage();
    }
    if(list[0]=="Move"){
        int index=list[1].toInt();
        int id=list[2].toInt();
        float xoffset=0;
        float yoffset=0;
        xoffset-=list[3].toFloat();
        yoffset-=list[4].toFloat();

        if(id==0){
            QVector<cv::Point3f>& points=manager->shapes[index]->points;
            for(int i=0;i<points.length();i++){
                points[i].x+=double(xoffset);
                points[i].y+=double(yoffset);
            }
        }
        if(id==1){
            QVector<cv::Point3f>& points=manager->shapes[index]->points;
            for(int i=0;i<points.length();i++){
                points[i].x+=double(xoffset);
                points[i].z+=double(yoffset*list.length());
            }
        }
        if(id==2){
            QVector<cv::Point3f>& points=manager->shapes[index]->points;
            for(int i=0;i<points.length();i++){
                points[i].y-=double(xoffset);
                points[i].z+=double(yoffset*list.length());
            }
        }

        QString str=QString("Move %1 %2 %3 %4").arg(index).arg(id).arg(-xoffset).arg(-yoffset);
        redoCommands.push_back(str);
        manager->parseImage();
    }
    if(list[0]=="Change"){
        int index=list[1].toInt();
        QString s1=list[2];
        QString s2=list[3];
        manager->shapes[index]->label=s1;
        redoCommands.push_back(s);
        emit(manager->labelChanged(index,manager->shapes[index]));
        manager->parseImage();
    }
}

void My::Command3D::redo(){
    if(redoCommands.count()==0)return;
    QString s=redoCommands.top();
    redoCommands.pop_back();
    QStringList list=s.split(" ");
    if(list[0]=="Add"){
        int index=list[1].toInt();
        int id=list[2].toInt();
        manager->shapes.insert(index,manager->trashshapes[id]);
        QString str=QString("Add %1").arg(index);
        undoCommands.push_back(str);
        emit(manager->labelAdded(manager->trashshapes[id],index));
        manager->parseImage();
    }
    if(list[0]=="Delete"){
        int index=list[1].toInt();
        manager->trashshapes.append(manager->shapes[index]);
        int id=manager->trashshapes.length()-1;
        manager->shapes.removeAt(index);
        QString str=QString("Delete %1 %2").arg(index).arg(id);
        undoCommands.push_back(str);
        emit(manager->labelDeleted(index));
        manager->parseImage();
    }
    if(list[0]=="Move"){
        int index=list[1].toInt();
        int id=list[2].toInt();
        float xoffset=list[3].toFloat();
        float yoffset=list[4].toFloat();

        if(id==0){
            QVector<cv::Point3f>& points=manager->shapes[index]->points;
            for(int i=0;i<points.length();i++){
                points[i].x+=double(xoffset);
                points[i].y+=double(yoffset);
            }
        }
        if(id==1){
            QVector<cv::Point3f>& points=manager->shapes[index]->points;
            for(int i=0;i<points.length();i++){
                points[i].x+=double(xoffset);
                points[i].z+=double(yoffset*list.length());
            }
        }
        if(id==2){
            QVector<cv::Point3f>& points=manager->shapes[index]->points;
            for(int i=0;i<points.length();i++){
                points[i].y-=double(xoffset);
                points[i].z+=double(yoffset*list.length());
            }
        }

        QString str=QString("Move %1 %2 %3 %4").arg(index).arg(id).arg(xoffset).arg(yoffset);
        undoCommands.push_back(str);
        manager->parseImage();
    }
    if(list[0]=="Change"){
        int index=list[1].toInt();
        manager->shapes[index]->label=list[3];
        undoCommands.push_back(s);
        emit(manager->labelChanged(index,manager->shapes[index]));
        manager->parseImage();
    }
}

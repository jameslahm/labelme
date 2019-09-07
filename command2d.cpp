#include"command2d.h"
#include"label.h"
#include"mainwindow.h"
#include<QObject>

void My::Command2D::logAdd(int index){
    QString s=QString("Add %1").arg(index);
    undoCommands.push_back(s);
    redoCommands.clear();
}

void My::Command2D::logDelete(int index,int id){
    QString s=QString("Delete %1 %2").arg(index).arg(id);
    undoCommands.push_back(s);
    redoCommands.clear();
}

void My::Command2D::logMove(int index,float xoffset,float yoffset,int id){
    QString s=QString("Move %1 %2 %3").arg(index).arg(xoffset).arg(yoffset);
    undoCommands.push_back(s);
    redoCommands.clear();
}

void My::Command2D::logChange(int index,QString s1,QString s2){
    QString s=QString("Change %1 %2 %3").arg(index).arg(s1).arg(s2);
    undoCommands.push_back(s);
    redoCommands.clear();
}

void My::Command2D::undo(){
    if(undoCommands.count()==0)return;
    QString s=undoCommands.top();
    undoCommands.pop_back();
    QStringList list=s.split(" ");
    if(list[0]=="Add"){

        int index=list[1].toInt();
        manager->trashshapes.append(manager->label->shapes[index]);
        manager->label->shapes.removeAt(index);
        QString str=QString("Add %1 %2").arg(index).arg(manager->trashshapes.length()-1);
        redoCommands.push_back(str);
        emit(manager->labelDeleted(index));
        manager->label->update();
    }
    if(list[0]=="Delete"){
        int index=list[1].toInt();
        int id=list[2].toInt();
        manager->label->shapes.insert(index,manager->trashshapes[id]);
        QString str=QString("Delete %1").arg(index);
        redoCommands.push_back(str);
        emit(manager->labelAdded(manager->trashshapes[id],index));
        manager->label->update();
    }
    if(list[0]=="Move"){
        int index=list[1].toInt();
        float xoffset=0;
        float yoffset=0;
        xoffset-=list[2].toFloat();
        yoffset-=list[3].toFloat();
        manager->label->shapes[index]->offset(xoffset,yoffset);
        QString str=QString("Move %1 %2 %3").arg(index).arg(-xoffset).arg(-yoffset);
        redoCommands.push_back(str);
        manager->label->update();
    }
    if(list[0]=="Change"){
        int index=list[1].toInt();
        QString s1=list[2];
        QString s2=list[3];
        manager->label->shapes[index]->label=s1;
        redoCommands.push_back(s);
        emit(manager->labelChanged(index,manager->label->shapes[index]));
        manager->label->update();
    }
}

void My::Command2D::redo(){
    if(redoCommands.count()==0)return;
    QString s=redoCommands.top();
    redoCommands.pop_back();
    QStringList list=s.split(" ");
    if(list[0]=="Add"){
        int index=list[1].toInt();
        int id=list[2].toInt();
        manager->label->shapes.insert(index,manager->trashshapes[id]);
        QString str=QString("Add %1").arg(index);
        undoCommands.push_back(str);
        emit(manager->labelAdded(manager->trashshapes[id],index));
        manager->label->update();
    }
    if(list[0]=="Delete"){
        int index=list[1].toInt();
        manager->trashshapes.append(manager->label->shapes[index]);
        int id=manager->trashshapes.length()-1;
        manager->label->shapes.removeAt(index);
        QString str=QString("Delete %1 %2").arg(index).arg(id);
        undoCommands.push_back(str);
        emit(manager->labelDeleted(index));
        manager->label->update();
    }
    if(list[0]=="Move"){
        int index=list[1].toInt();
        float xoffset=list[2].toFloat();
        float yoffset=list[3].toFloat();
        manager->label->shapes[index]->offset(xoffset,yoffset);
        QString str=QString("Move %1 %2 %3").arg(index).arg(xoffset).arg(yoffset);
        undoCommands.push_back(str);
        manager->label->update();
    }
    if(list[0]=="Change"){
        int index=list[1].toInt();
        manager->label->shapes[index]->label=list[3];
        undoCommands.push_back(s);
        emit(manager->labelChanged(index,manager->label->shapes[index]));
        manager->label->update();
    }
}

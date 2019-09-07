#ifndef COMMAND3D_H
#define COMMAND3D_H

#include"Namespace.h"
#include<QStack>
#include"centralwinit3d.h"
#include"command.h"

class My::Command3D: public My::Command{

public:

    CentralWInit3D* manager;
    void logAdd(int index){
        QString s=QString("Add %1").arg(index);
        undoCommands.push_back(s);
        redoCommands.clear();
    }

    void logDelete(int index,int id){
        QString s=QString("Delete %1 %2").arg(index).arg(id);
        undoCommands.push_back(s);
        redoCommands.clear();
    }

    void logMove(int index,float xoffset,float yoffset,int id){
        QString s=QString("Move %1 %2 %3 %4").arg(index).arg(id).arg(xoffset).arg(yoffset);
        undoCommands.push_back(s);
        redoCommands.clear();
    }

    void logChange(int index,QString s1,QString s2){
        QString s=QString("Change %1 %2 %3").arg(index).arg(s1).arg(s2);
        undoCommands.push_back(s);
        redoCommands.clear();
    }

    void undo();
    void redo();
    ~Command3D(){
        delete manager;
    }
};



#endif // COMMAND3D_H

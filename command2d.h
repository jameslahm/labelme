#ifndef COMMAND2D_H
#define COMMAND2D_H
#include"Namespace.h"
#include "centralwinit.h"
#include"command.h"

class My::Command2D:public My::Command{
public:

    CentralWInit* manager;
    void logAdd(int index);

    void logDelete(int index,int id);

    void logMove(int index,float xoffset,float yoffset,int id=0);

    void logChange(int index,QString s1,QString s2);

    void undo();
    void redo();
    ~Command2D(){
        delete manager;
    }
};

#endif // COMMAND2D_H

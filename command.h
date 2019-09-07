#ifndef COMMAND_H
#define COMMAND_H
#include<QStack>
#include"Namespace.h"

class My::Command{

public:
    QStack<QString> undoCommands;
    QStack<QString> redoCommands;
    virtual void logAdd(int index);
    virtual void logDelete(int index,int id);
    virtual void logMove(int index,float xoffset,float yoffset,int id=0);
    virtual void logChange(int index,QString s1,QString s2);
    virtual void undo();
    virtual void redo();
    virtual ~Command()=0;
};

#endif // COMMAND_H

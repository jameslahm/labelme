/*!
* \file Command.h
* \brief 命令栈基类
* \author 王澳
* \date 2019.09
*/

#ifndef COMMAND_H
#define COMMAND_H
#include<QStack>
#include"Namespace.h"

/// \brief 命令栈基类，用来实现撤销和重做功能
class My::Command{

public:
    /// \brief 存储undo（撤销）命令栈
    QStack<QString> undoCommands;

    /// \brief 存储redo（重做）命令栈
    QStack<QString> redoCommands;

    /// \brief 记录添加标注
    virtual void logAdd(int index);

    /// \brief 记录删除标注
    virtual void logDelete(int index,int id);

    /// \brief 记录移动标注
    virtual void logMove(int index,float xoffset,float yoffset,int id=0);

    /// \brief 记录更改标注
    virtual void logChange(int index,QString s1,QString s2);

    /// \brief 撤销功能
    virtual void undo();

    /// \brief 重做功能
    virtual void redo();
    virtual ~Command()=0;
};

#endif // COMMAND_H

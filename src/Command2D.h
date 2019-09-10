/*!
* \file Command2D.h
* \brief 2d命令栈类
* \author 王澳
* \date 2019.09
*/

#ifndef COMMAND2D_H
#define COMMAND2D_H

#include "CentralWInit2D.h"
#include"Command.h"

/// \brief 2d命令栈类，用于实现撤销和重做功能，继承Command类
class My::Command2D:public My::Command{
public:

    /// \brief 存放中心组件指针
    CentralWInit2D* manager;

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

    ~Command2D(){
        delete manager;
    }
};

#endif // COMMAND2D_H

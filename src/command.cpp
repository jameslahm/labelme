/*!
* \file command.cpp
* \brief 命令栈基类
* \author 王澳
* \date 2019.09
*/

#include"Command.h"
#include"Namespace.h"

My::Command::~Command(){

}


/// \brief 记录添加标注
void My::Command::logAdd(int index){

}

/// \brief 记录删除标注
void My::Command::logDelete(int index,int id){

}

/// \brief 记录移动标注
void My::Command::logMove(int index,float xoffset,float yoffset,int id){

}

/// \brief 记录更改标注
void My::Command::logChange(int index,QString s1,QString s2){

}

/// \brief 撤销功能
void My::Command::undo(){

}

/// \brief 重做功能
void My::Command::redo(){

}

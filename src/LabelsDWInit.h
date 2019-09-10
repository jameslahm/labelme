/*!
* \file LabelsDWInit.h
* \brief 标签列表管理组件
* \author 王澳
* \date 2019.09
*/

#ifndef LABELS_H
#define LABELS_H

#include <QDockWidget>
#include <QMainWindow>
#include <QWidget>
#include<QList>
#include<QListWidget>
#include"Namespace.h"

/// \brief 负责标签列表组件的初始化及状态管理
///
/// LabelsDWInit类负责标签列表浮动窗口组件的界面初始化，
/// 以及后续的标签列表的管理。
/// 由MainWindow连接该类与CentralWidget类的信号与槽，
/// 建立起双向信息传递通道以进行同步。
class LabelsDWInit : public QWidget
{
    Q_OBJECT
public:

    /// \brief 负责展示标签列表的浮动窗口
    QDockWidget* dwLabels;

    /// \brief 标签列表组件
    QListWidget* lw;

    LabelsDWInit(QWidget* parent):QWidget(parent){}

    /// \brief 组件初始化
    void setUp(QMainWindow* w);


    //以下均为槽函数

    /// \brief 显示标注标签，当中心组件解析完毕后，会调用该槽函数
    void showLabels(QList<My::Shape*>& shapes);

    /// \brief 改变选中状态，中心组件选中状态发生改变时，会调用该函数同步
    void changeSelected(int index,bool b);

    /// \brief 添加标注，中心组件添加标注时，调用该函数同步
    void addLabel(My::Shape* shape,int index);

    /// \brief 更改标注，中心组件更改标注文字时，调用该函数同步
    void changeLabel(int index,My::Shape* shape);

    /// \brief 删除标注，中心组件删除标注时，调用该函数同步
    void deleteLabel(int index);

};

#endif // LABELS_H

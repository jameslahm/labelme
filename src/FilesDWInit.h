/*!
* \file FilesDWInit.h
* \brief 文件列表管理组件
* \author 王澳
* \date 2019.09
*/

#ifndef FILES_H
#define FILES_H

#include <QDockWidget>
#include <QMainWindow>
#include <QWidget>
#include<QFileDialog>
#include<QFile>
#include<QDir>
#include<QFileInfo>
#include<QMessageBox>
#include<QDirIterator>
#include<QListWidget>

/// \brief 负责文件列表组件的初始化及状态管理
///
/// 负责文件列表浮动窗口组件的界面初始化，
/// 以及后续的文件列表的管理。
/// 与文件相关的动作比如打开文件、打开文件夹等均由MainWindow进行消息传递转发到此处进行处理。
class FilesDWInit : public QWidget
{
    Q_OBJECT

public:

    /// \brief 浮动窗口，用来显示文件列表
    QDockWidget* dwFiles;

    /// \brief 显示文件列表的组件
    QListWidget* lw;

    FilesDWInit(QWidget* parent):QWidget(parent){}

    /// \brief 界面初始化函数
    void setUp(QMainWindow *w);

    /// \brief 打开文件功能
    bool openFile(QString& fileName,QString& dirName);

    /// \brief 打开文件夹功能
    bool openDir(QString& fileName,QString& dirName);

    /// \brief 切换item的checkstate状态，保证同一时刻只有一个被勾选
    void changeState(QListWidgetItem* previous);

    /// \brief 下一张图片
    void nextImage(QString& fileName);

    /// \brief 上一张图片
    void prevImage(QString& fileName);
};

#endif // FILES_H

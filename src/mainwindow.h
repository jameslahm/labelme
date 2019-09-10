/*!
* \file mainwindow.h
* \brief 主窗口
* \author 王澳
* \date 2019.09
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFile>
#include<QFileInfo>
#include<QKeyEvent>
#include<QDir>
#include"MenubarInit.h"
#include"ToolbarInit.h"
#include"ActionsInit.h"
#include"LabelsDWInit.h"
#include"FilesDWInit.h"
#include"StatusbarInit.h"
#include"CentralWidget.h"
#include"CentralWInit2D.h"
#include"CentralWInit3D.h"

/// \brief 主窗口
///
/// MainWindow 类是主窗口类，采用
/// 策略模式进行管理，成员变量包括窗口的各个组件部分（代理），
/// 所有的UI界面的设计及初始化均在组件的setup函数中完成。MainWindow只负责消息的传递，
/// 包括信号与槽的连接、QAction的触发和消息的转发等等，不负责具体的处理，
/// 具体的处理全部分别交由具体的组件。组件包括动作管理组件，菜单栏管理组件，状态栏管理组件，工具栏管理组件，标签列表管理组件，
/// 文件列表管理组件以及中心窗口组件
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    //控件初始化部分

    /// \brief 动作管理组件
    ActionsInit* actionsinit;

    /// \brief 菜单栏管理组件
    MenubarInit* mbarinit;

    /// \brief 工具栏管理组件
    ToolbarInit* tbarinit;

    /// \brief 标签列表管理组件
    LabelsDWInit* labelsinit;

    /// \brief 文件列表管理组件
    FilesDWInit* filesinit;

    /// \brief 状态栏管理组件
    StatusbarInit* sbarinit;

    /// \brief 中心组件
    My::CentralWidget* centralwidget;

    /// \brief 2d中心组件
    CentralWInit2D* centralwidget2d;

    /// \brief 3d中心组件
    CentralWInit3D* centralwidget3d;

    /// \brief 文件名
    QString fileName;

    /// \brief 目录名
    QString dirName;

public:
    signals:

    /// \brief 文件改变信号
    void fileNameChanged(QString fileName,QString labelfileName);

    /// \brief 状态初始化信号
    void statusInit(bool b);

public:

    /// \brief 改变状态槽函数
    void changeState(int index,bool b);

public:

    /// \brief 主窗口状态变量
    ///
    /// 默认为Edit状态，Focus状态代表有标注形状被选中状态，Create状态代表正在创建标注形状状态
    My::Status status=My::Status::Edit;

    /// \brief 模式变量
    ///
    /// TwoD代表2d模式，ThreeD代表3d模式
    My::Mode mode=My::TwoD;

    //所有QAction触发连接的槽函数

    /// \brief 切换文件
    void changefile(QListWidgetItem* current,QListWidgetItem* prvious);

    /// \brief 打开文件
    void openfile();

    /// \brief 下一张图片
    void nextimage();

    /// \brief 上一张图片
    void previmage();

    /// \brief 打开文件夹
    void opendir();

    /// \brief 保存文件
    void save();

    /// \brief 另存文件
    void saveas();

    /// \brief 自动保存
    void saveautomatically();

    /// \brief 切换输出文件夹
    void changeoutputdir();

    /// \brief 关闭文件
    void closefile();

    /// \brief 删除文件
    void deletefile();

    /// \brief 退出程序
    void quit();

    /// \brief 创建画刷（分割标注）
    void createbrush();

    /// \brief 创建矩形
    void createrectangle();

    /// \brief 创建多边形
    void createpolygons();

    /// \brief 创建圆
    void createcircle();

    /// \brief 创建平滑曲线
    void createcurve();

    /// \brief 编辑标注形状
    void editpolygons();

    /// \brief 编辑标注标签文字
    void editlabel();

    /// \brief 删除标注
    void deletepolygons();

    /// \brief 撤销
    void undo();

    /// \brief 重做
    void redo();

    /// \brief 撤销正在创建的标注形状
    void undolastpoint();

    /// \brief 编辑画刷大小
    void editbrush();

    /// \brief 标签列表组件是否显示
    void polygonlabels(bool b);

    /// \brief 文件列表组件是否显示
    void filelist(bool b);

    /// \brief 隐藏标注形状
    void hidepolygons();

    /// \brief 显示标注形状
    void showpolygons();

    /// \brief 放大
    void zoomin();

    /// \brief 缩小
    void zoomout();

    /// \brief 恢复原始大小
    void originalsize();

    /// \brief 宽度适应中心组件宽度
    void fitwindow();

    /// \brief 编辑颜色
    void editcolor();

    /// \brief 是否开启放大镜
    void magnifier(bool b);

    /// \brief 导入标注文件
    void import();

    /// \brief 切换3d模式
    void toggle3d(bool b);

    /// \brief 构造函数
    MainWindow(QWidget *parent = 0);

    ~MainWindow();

    /// \brief 按键控制
    void keyPressEvent(QKeyEvent* event);

    /// \brief 关闭事件重写
    void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_H

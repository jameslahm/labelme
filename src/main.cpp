/*!
* \file main.cpp
* \brief 主函数文件
* \mainpage a image annotation software for 2D or 3D images
* \author 王澳
* \date 2019.09
* \version 0.1.0
*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

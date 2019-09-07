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
#include<QGroupBox>
#include<QCheckBox>
#include<QDirIterator>
#include<QDebug>
#include<QListWidget>
#include<QPalette>


class FilesDWInit : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
    QDockWidget* dwFiles;
    QListWidget* lw;

public:

    FilesDWInit(QWidget* parent):QWidget(parent){}
    QDockWidget* dockwidget() const{
        return dwFiles;
    }
    void setUp(QMainWindow *w)
    {
        dwFiles = new QDockWidget(w);
        dwFiles->setWindowTitle("File list");
        dwFiles->setObjectName("Files");
        w->addDockWidget(Qt::RightDockWidgetArea, dwFiles);
        lw=new QListWidget(dwFiles);
        dwFiles->setWidget(lw);
    }

    bool openFile(QString& fileName,QString& dirName);
    bool openDir(QString& fileName,QString& dirName);
    void changeState(QListWidgetItem* previous);
    void nextImage(QString& fileName);
    void prevImage(QString& fileName);
};

#endif // FILES_H

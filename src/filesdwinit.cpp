/*!
* \file filesdwinit.cpp
* \brief 文件列表管理组件
* \author 王澳
* \date 2019.09
*/

#include"mainwindow.h"
#include"FilesDWInit.h"

/// \brief 界面初始化函数
void FilesDWInit::setUp(QMainWindow *w)
{
    dwFiles = new QDockWidget(w);
    dwFiles->setWindowTitle("File list");
    dwFiles->setObjectName("Files");
    w->addDockWidget(Qt::RightDockWidgetArea, dwFiles);
    lw=new QListWidget(dwFiles);
    dwFiles->setWidget(lw);
}


/// \brief 打开文件
bool FilesDWInit::openFile(QString &fileName, QString &dirName){

    //打开文件
    fileName=QFileDialog::getOpenFileName(this,"Open an Image",QDir::root().absolutePath(),\
                     "images(*.png *.bmp *.jpeg *.jpg *.gif)");
    if (fileName.isEmpty()){
        QMessageBox::information(this,"File!","Please select an Image");
        return false;
    }
    else{
        QFileInfo fileInfo(fileName);
        dirName=fileInfo.absoluteDir().absolutePath();
    }

    //递归遍历当前目录下的所有图片文件
    QStringList filters;
    QStringList fileNames;
    filters<<"*.jpg"<<"*.bmp"<<"*.jpeg"<<"*.png"<<"*.gif";
    QDirIterator iter(dirName,filters,QDir::Files,QDirIterator::Subdirectories);
    while(iter.hasNext()){
        iter.next();
        fileNames.append(iter.fileInfo().absoluteFilePath());
    }

    //移除之前目录里的图片列表
    lw->clear();

    //添加文件列表组件中的item
    int temp=0;
    for(int i=0;i<fileNames.length();i++){
        QListWidgetItem* item=new QListWidgetItem();
        item->setCheckState(Qt::CheckState::Unchecked);
        item->setText(fileNames[i]);
        lw->addItem(item);
        if(fileNames.at(i)==fileName){
            temp=i;
            item->setCheckState(Qt::CheckState::Checked);
        }
    }
    MainWindow* w=qobject_cast<MainWindow*>(parent());
    if(w->mode==My::TwoD)lw->setCurrentItem(lw->item(temp));
    return true;
}


/// \brief 打开文件夹
bool FilesDWInit::openDir(QString &fileName, QString &dirName){

    dirName=QFileDialog::getExistingDirectory(this,"Open a directory",QDir::root().absolutePath());
    if(dirName.isEmpty()){
        QMessageBox::information(this,"Directory!","Please select a directory");
        return false;
    }
    QStringList filters;
    QStringList fileNames;
    filters<<"*.jpg"<<"*.bmp"<<"*.jpeg"<<"*.png"<<"*.gif";
    QDirIterator iter(dirName,filters,QDir::Files,QDirIterator::Subdirectories);
    while(iter.hasNext()){
        iter.next();
        fileNames.append(iter.fileInfo().absoluteFilePath());
    }
    if(fileNames.length()==0){
        QMessageBox::warning(this,"Warning!","This directory doesn't have images");
        return false;
    }

    lw->clear();
    fileName=fileNames[0];
    for(int i=0;i<fileNames.length();i++){
        QListWidgetItem* item=new QListWidgetItem();
        item->setCheckState(Qt::CheckState::Unchecked);
        item->setText(fileNames[i]);
        lw->addItem(item);
    }
    lw->item(0)->setCheckState(Qt::CheckState::Checked);
    lw->setCurrentItem(lw->item(0));
    return true;
}


/// \brief 改变item的checkstate状态
///
/// 保证只有一个item为checked，后续可扩展开发
void FilesDWInit::changeState(QListWidgetItem* previous){
    if(previous==nullptr)return;
    previous->setCheckState(Qt::CheckState::Unchecked);
    lw->currentItem()->setCheckState(Qt::CheckState::Checked);
}


/// \brief 下一张图片
void FilesDWInit::nextImage(QString& fileName){
    int index=lw->row(lw->currentItem());
    if(index<lw->count()-1){
        lw->setCurrentItem(lw->item(index+1));
        fileName=lw->item(index+1)->text();
    }
    else{
        QMessageBox::information(this,"next Image","This is the last image");
        return;
    }
}

/// \brief 上一张图片
void FilesDWInit::prevImage(QString& fileName){
    int index=lw->row(lw->currentItem());
    if(index>0){
        lw->setCurrentItem(lw->item(index-1));
        fileName=lw->item(index-1)->text();
    }
    else{
        QMessageBox::information(this,"prev Image","This is the first image");
        return;
    }
}

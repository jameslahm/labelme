/*!
* \file centralwidget.cpp
* \brief 中心组件基类
* \author 王澳
* \date 2019.09
*/

#include"CentralWidget.h"
#include"Command.h"


/// \brief 用来初始化中心组件
void My::CentralWidget::setUp(QMainWindow* w){

}

/// \brief 解析文件（显示图片，解析标注形状）
void My::CentralWidget::parse(const QString& fileName,const QString& labelfileName){

}

/// \brief 改变标注的状态，槽函数
void My::CentralWidget::changeShapeLabel(QListWidgetItem* item){

}

/// \brief 改变标注的选中状态、槽函数
void My::CentralWidget::changeSelected(){

}

/// \brief 创建标注形状
void My::CentralWidget::createShape(My::shapeStatus s){

}

/// \brief 处理按键事件
void My::CentralWidget::keyPressHandle(int k){

}

/// \brief 编辑标注
void My::CentralWidget::editLabel(){

}

/// \brief 删除标注
void My::CentralWidget::deleteLabel(){

}

/// \brief 编辑画刷大小，画刷是用来进行分割标注
void My::CentralWidget::editBrush(){

}

/// \brief 编辑标注形状的颜色
void My::CentralWidget::editColor(){

}

/// \brief 放大功能
void My::CentralWidget::zoomIn(){

}

/// \brief 缩小功能
void My::CentralWidget::zoomOut(){

}

/// \brief 使中心组件宽度适应主窗口
void My::CentralWidget::fitWindow(){

}

/// \brief 恢复图片原始大小
void My::CentralWidget::originalSize(){

}

/// \brief 放大镜功能
void My::CentralWidget::magnifier(bool b){

}

/// \brief 保存文件功能
void My::CentralWidget::save(const QString& fileName){

}

/// \brief 隐藏标注
void My::CentralWidget::hidePolygons(){

}

/// \brief 显示标注
void My::CentralWidget::showPolygons(){

}

/// \brief 关闭文件功能
void My::CentralWidget::closeFile(){

}

My::CentralWidget::~CentralWidget(){

}

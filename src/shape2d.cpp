/*!
* \file Shape2d.cpp
* \brief 2d形状纯虚基类
* \author 王澳
* \date 2019.09
*/

#include"Shape2D.h"

My::Shape2D::~Shape2D(){}


/// \brief 绘制函数，默认虚函数实现
void My::Shape2D::draw(QWidget *w){}

/// \brief 判断是否在形状内函数，默认虚函数实现
bool My::Shape2D::isInShape(QPointF p,QWidget* w){

}

/// \brief pian'yi函数，默认虚函数实现
void My::Shape2D::offset(float xOffset,float yOffset){

}

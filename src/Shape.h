/*!
* \file Shape.h
* \brief 所有形状纯虚基类
* \author 王澳
* \date 2019.09
*/

#ifndef SHAPE_H
#define SHAPE_H
#include<QString>
#include<QColor>

#include"Namespace.h"

/// \brief 所有标注形状的基类
///
/// 负责形状的绘制及形状的存储
class My::Shape{
public:

    /// \brief 标注形状的类型
    ///
    /// Brush代表画刷形状，用于分割标注，
    /// Rectangle代表矩形形状，Polygons代表多边形形状，Circle代表圆形形状，Curve代表平滑曲线形状
    /// Rectangle3D代表3d长方体形状，Brush3D代表3d画刷形状，用于3d分割标注
    enum Type{Brush,Rectangle,Polygons,Circle,Curve,Rectangle3D,Brush3D};

    /// \brief 标注类型
    const Type type;

    /// \brief 标注对应的标签文字
    QString label;

    /// \brief 默认标注颜色
    QColor color=QColor(100,255,0,100);

    /// \brief 是否填充内部
    ///
    /// 当被选中时，isFill会变为true，填充标注形状的内部，便于用户交互
    bool isFill=false;

    /// \brief 是否隐藏标注
    bool isHide=false;

    /// \brief 是否悬浮
    ///
    /// 当前鼠标是否悬浮在标注内部，若悬浮在内部，则变为true，填充内部颜色，便于用户交互
    bool isHover=false;

    Shape(Type t);

    virtual ~Shape()=0;
};
#endif // SHAPE_H

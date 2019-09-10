/*!
* \file Namespace.h
* \brief 自定义组件的命名空间
* \author 王澳
* \date 2019.09
*/

#ifndef NAMESPACE_H
#define NAMESPACE_H

/// \brief 存放一些自定义组件和状态变量以及标注形状的命名空间
namespace My {

    //标注形状类

    /// \brief 所有标注形状的纯虚基类
    class Shape;

    /// \brief 2d标注形状的纯虚基类
    class Shape2D;

    /// \brief 3d标注形状的纯虚基类
    class Shape3D;

    /// \brief 矩形标注
    class Rectangle;

    /// \brief 圆形标注
    class Circle;

    /// \brief 多边形标注
    class Polygons;

    /// \brief 画刷标注（分割标注）
    class Brush;

    /// \brief 平滑曲线标注
    class Curve;

    /// \brief 存储3d长方体，检测标注形状
    class Rectangle3D;

    /// \brief 存储3d画刷，分割标注形状
    class Brush3D;

    /// \brief 主窗口的状态变量
    ///
    /// edit为默认状态，此时处于编辑标注的状态，focus为选中状态，表明此时有标注被选中，可以进行响应的操作，create为创建标注状态，表明此时正在创建标注
    enum Status {Edit,Focus,Create};

    /// \brief 中心组件状态
    ///
    /// noshape表明此时鼠标不在标注形状内部，且不在创建状态，inshape表明此时鼠标在标注形状内部，brushshape表明此时处于创建画刷状态，（分割标注）
    /// rectangleshape表明此时处于绘制矩形状态，circleshape表明处于创建圆形状态，polygonsshape表明处于创建多边形状态，curveshape表明处于创建平滑曲线状态
    enum shapeStatus {BrushShape,PolygonsShape,RectangleShape,CircleShape,CurveShape,NoShape,InShape};

    /// \brief 2d中心组件负责图片及标注绘制展示
    class Label;

    /// \brief 3d中心组件负责图片及标注绘制展示
    class Label3D;

    /// \brief 主窗口模式状态变量
    ///
    /// TwoD代表处于2d状态，ThreeD代表处于3d状态
    enum Mode {TwoD,ThreeD};

    /// \brief 3d组件对label3d的统一接口中同步状态变量
    ///
    /// addshape表明添加标注形状，addpoint表明添加像素点，selected表明选中，unselected表明未选中，remove表明移除正在创建的形状
    /// changepoint表明更改正在绘制的像素点，showmessage表明显示鼠标坐标，hovered表明鼠标在该标注形状内，unhovered表明鼠标已不在该标注形状内，offset表明偏移标注位置
    enum syncStatus {AddShape,AddPoint,Selected,Remove,UnSelected,ChangePoint,ShowMessage,Hovered,UnHovered,Offset};

    /// \brief 命令栈类的基类
    ///
    /// 负责实现撤销和重做功能
    class Command;

    /// \brief 2d组件的命令栈类
    class Command2D;

    /// \brief 3d组件的命令栈类
    class Command3D;

    /// \brief 中心组件的纯虚基类
    ///
    /// 2d和3d中心组件均继承于它，定义了一系列虚基类接口
    class CentralWidget;

    /// \brief 解析文件类
    ///
    /// 负责所有与解析文件相关的功能，负责解析2d和3db标注文件以及图片
    class Parse;
}

#endif // NAMESPACE_H

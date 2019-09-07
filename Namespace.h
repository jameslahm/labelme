#ifndef NAMESPACE_H
#define NAMESPACE_H


namespace My {
    class Shape;
    class Rectangle;
    class Circle;
    class Polygons;
    class Brush;
    class Curve;
    enum Status {Edit,Focus,Create};
    enum shapeStatus {BrushShape,PolygonsShape,RectangleShape,CircleShape,CurveShape,NoShape,InShape};
    class Label;
    enum Mode {TwoD,ThreeD};
    class Label3D;
    class Shape3D;
    class Rectangle3D;
    class Brush3D;
    enum syncStatus {AddShape,AddPoint,Selected,Remove,UnSelected,ChangePoint,ShowMessage,Hovered,UnHovered,Offset};
    class Command2D;
    class Command3D;
    class Command;
}

#endif // NAMESPACE_H

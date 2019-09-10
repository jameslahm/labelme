#-------------------------------------------------
#
# Project created by QtCreator 2019-08-28T14:51:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LabelMe
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        actionsinit.cpp \
        brush.cpp \
        brush3d.cpp \
        centralwidget.cpp \
        centralwinit2d.cpp \
        centralwinit3d.cpp \
        circle.cpp \
        command.cpp \
        command2d.cpp \
        command3d.cpp \
        curve.cpp \
        filesdwinit.cpp \
        label.cpp \
        label3d.cpp \
        labelsdwinit.cpp \
        main.cpp \
        mainwindow.cpp \
        parse.cpp \
        polygons.cpp \
        rectangle.cpp \
        rectangle3d.cpp \
        shape.cpp \
        shape2d.cpp \
        shape3d.cpp

HEADERS += \
        ActionsInit.h \
        Brush.h \
        Brush3D.h \
        CentralWInit2D.h \
        CentralWInit3D.h \
        CentralWidget.h \
        Circle.h \
        Command.h \
        Command2D.h \
        Command3D.h \
        Curve.h \
        FilesDWInit.h \
        Label.h \
        Label3D.h \
        LabelsDWInit.h \
        MenubarInit.h \
        Namespace.h \
        Parse.h \
        Polygons.h \
        Rectangle.h \
        Rectangle3D.h \
        Shape.h \
        Shape2D.h \
        Shape3D.h \
        StatusbarInit.h \
        ToolbarInit.h \
        mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc


INCLUDEPATH += $$(OPENCV_SDK_DIR)/include

LIBS += -L$$(OPENCV_SDK_DIR)/x64/mingw/lib \
        -lopencv_core401      \
        -lopencv_highgui401    \
        -lopencv_imgcodecs401   \
        -lopencv_imgproc401      \
        -lopencv_features2d401    \
        -lopencv_calib3d401

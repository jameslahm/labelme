#ifndef LABELLIST_H
#define LABELLIST_H

#include <QDockWidget>
#include <QMainWindow>
#include <QWidget>

class LabellistDWInit : public QWidget
{
    Q_OBJECT
public:
    LabellistDWInit(QWidget* parent):QWidget(parent){}
    void setUp(QMainWindow *w)
    {
        QDockWidget *dwLabels = new QDockWidget(w);
        dwLabels->setWindowTitle("labellist");
        w->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dwLabels);
    }
};
#endif // LABELLIST_H

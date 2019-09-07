#ifndef FLAG_H
#define FLAG_H

#include <QDockWidget>
#include <QMainWindow>
#include <QWidget>

class FlagDWInit : public QWidget
{
    Q_OBJECT
public:
    FlagDWInit(QWidget* parent):QWidget(parent){}
    void setUp(QMainWindow *w)
    {
        QDockWidget *dwLabels = new QDockWidget(w);
        dwLabels->setWindowTitle("flag");
        w->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dwLabels);
    }
};
#endif // FLAG_H

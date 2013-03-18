#ifndef NAVDATAWINDOW_H
#define NAVDATAWINDOW_H

#include <QMainWindow>
#include "ardrone.h"

namespace Ui {
class navdataWindow;
}

class navdataWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit navdataWindow(QWidget *parent = 0);
    explicit navdataWindow(ARDrone *drone_, QWidget *parent = 0);
    ~navdataWindow();

private:
    Ui::navdataWindow *ui;
    ARDrone *drone;
};

#endif // NAVDATAWINDOW_H

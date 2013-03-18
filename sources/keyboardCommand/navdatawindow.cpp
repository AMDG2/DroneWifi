#include "navdatawindow.h"
#include "ui_navdatawindow.h"

navdataWindow::navdataWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::navdataWindow)
{
    ui->setupUi(this);
    drone = new ARDrone(this);
    drone->connectToDrone();
    drone->initNavData();
}

navdataWindow::navdataWindow(ARDrone *drone_, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::navdataWindow)
{
    ui->setupUi(this);
    drone = drone_;
    drone->initNavData();
}

navdataWindow::~navdataWindow()
{
    delete ui;
}

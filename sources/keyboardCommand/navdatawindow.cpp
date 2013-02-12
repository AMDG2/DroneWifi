#include "navdatawindow.h"
#include "ui_navdatawindow.h"

navdataWindow::navdataWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::navdataWindow)
{
    ui->setupUi(this);
    navdataFlow = new ARDrone(this);
    navdataFlow->connectToDrone();
    navdataFlow->initNavData();
}

navdataWindow::~navdataWindow()
{
    delete ui;
}

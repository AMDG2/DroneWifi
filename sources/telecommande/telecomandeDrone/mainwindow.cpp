#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    at = new arCommand("192.168.1.1", 5554, this);

    connect(at, SIGNAL(newTrame(QString)), this, SLOT(updateStdout(QString)));
    ui->stdout->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    at->changeData("AT*REF=", ",290718208\r");
    at->sendAT();

    at->changeData("AT*PCMD=", "1,0,0,0,0\r");
    at->sendAT();
}

void MainWindow::updateStdout(QString data)
{
    ui->stdout->setText(ui->stdout->text() + data + "\n");
}

void MainWindow::on_btnLand_clicked()
{
    at->changeData("AT*REF=", ",290717696");
    at->sendAT();
}

void MainWindow::on_pushButton_2_clicked()
{
    at->connectToDrone();
    at->initDrone();
}

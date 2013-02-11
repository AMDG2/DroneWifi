#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    at = new arCommand("192.168.1.1", 5556, this);

    connect(at, SIGNAL(newTrame(QString)), this, SLOT(updateStdout(QString)));
    ui->stdout->clear();

    at->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    at->changeData("AT*REF=", ",290718208\r");
    at->sendAT();

    at->setARU(false);
    at->start();

    at->changeData("AT*PCMD=", ",1,0,0,0,0\r");
}

void MainWindow::updateStdout(QString data)
{
    ui->stdout->setText(ui->stdout->text() + data + "\n");
}

void MainWindow::on_btnLand_clicked()
{
    at->setARU();
    at->changeData("AT*REF=", ",290717696\r");
    at->sendAT();
}

void MainWindow::on_pushButton_2_clicked()
{
    at->connectToDrone();
    at->initDrone();
}

void MainWindow::on_arretUrgence_clicked()
{
    at->setARU();
    at->changeData("AT*REF=", ",290717952\r");
    at->sendAT();
}

void MainWindow::on_btn_up_clicked()
{

}

void MainWindow::on_btn_vol_stat_clicked()
{
    at->changeData("AT*PCMD=", ",1,0,0,0,0\r");
}

void MainWindow::on_btn_up_clicked(bool checked)
{
    float buff = 0.5;
    char buffer[50];
    sprintf(buffer, ",1,0,0,%d,0\r", *(int*)(&buff));
    QString strBuff;
    strBuff.operator =(buffer);
    at->changeData("AT*PCMD=", strBuff);
}

void MainWindow::on_btn_down_clicked(bool checked)
{
    float buff = -0.5;
    char buffer[50];
    sprintf(buffer, ",1,0,0,%d,0\r", *(int*)(&buff));
    QString strBuff;
    strBuff.operator =(buffer);
    at->changeData("AT*PCMD=", strBuff);
}

void MainWindow::on_btn_left_clicked()
{
    float buff = -0.5;
    char buffer[50];
    sprintf(buffer, ",1,0,0,0,%d\r", *(int*)(&buff));
    QString strBuff;
    strBuff.operator =(buffer);
    at->changeData("AT*PCMD=", strBuff);
}

void MainWindow::on_btn_right_clicked()
{
    float buff = 0.5;
    char buffer[50];
    sprintf(buffer, ",1,0,0,0,%d\r", *(int*)(&buff));
    QString strBuff;
    strBuff.operator =(buffer);
    at->changeData("AT*PCMD=", strBuff);
}

void MainWindow::on_pushButton_4_clicked()
{
    float buff = -0.5;
    char buffer[50];
    sprintf(buffer, ",1,0,%d,0,0\r", *(int*)(&buff));
    QString strBuff;
    strBuff.operator =(buffer);
    at->changeData("AT*PCMD=", strBuff);
}

void MainWindow::on_pushButton_10_clicked()
{
    float buff = 0.5;
    char buffer[50];
    sprintf(buffer, ",1,0,%d,0,0\r", *(int*)(&buff));
    QString strBuff;
    strBuff.operator =(buffer);
    at->changeData("AT*PCMD=", strBuff);
}

void MainWindow::on_pushButton_8_clicked()
{
    float buff = -0.5;
    char buffer[50];
    sprintf(buffer, ",1,%d,0,0,0\r", *(int*)(&buff));
    QString strBuff;
    strBuff.operator =(buffer);
    at->changeData("AT*PCMD=", strBuff);
}

void MainWindow::on_pushButton_6_clicked()
{
    float buff = -0.5;
    char buffer[50];
    sprintf(buffer, ",1,%d,0,0,0\r", *(int*)(&buff));
    QString strBuff;
    strBuff.operator =(buffer);
    at->changeData("AT*PCMD=", strBuff);
}

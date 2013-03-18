#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mode = 0;
    drone = new ARDrone(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnKeyboardMode_clicked()
{
    mode = 1;
}

void MainWindow::on_btnMouseMode_clicked()
{
    mode = 2;
}

void MainWindow::on_btnUp_clicked()
{
    if(mode == 2)
    {
        if(ui->btnUp->text() == "Up")
        {
            drone->setGoUpDown(0.5);
            ui->btnUp->setText("Stop Up");
        }
        else
        {
            drone->setGoUpDown(0);
            ui->btnUp->setText("Up");
        }

    }
    else
        qDebug("Mode souris désactivée\n");
}

void MainWindow::on_btnStat_clicked()
{
    if(mode == 2)
    {
        drone->setGoUpDown(0);
        drone->setTiltFrontBack(0);
        drone->setTiltLeftRight(0);
        drone->setTurnLeftRight(0);
    }
    else
        qDebug("Mode souris désactivée\n");
}

void MainWindow::on_btnStat1_clicked()
{
    if(mode == 2)
    {
        drone->setGoUpDown(0);
        drone->setTiltFrontBack(0);
        drone->setTiltLeftRight(0);
        drone->setTurnLeftRight(0);
    }
    else
        qDebug("Mode souris désactivée\n");
}

void MainWindow::on_btnLeft_clicked()
{
    if(mode == 2)
    {
        if(ui->btnLeft->text() == "Turn Left")
        {
            drone->setTurnLeftRight(-0.5);
            ui->btnLeft->setText("Stop turn");
        }
        else
        {
            drone->setTurnLeftRight(0);
            ui->btnLeft->setText("Turn Left");
        }
    }
    else
        qDebug("Mode souris désactivée\n");
}

void MainWindow::on_btnRight_clicked()
{
    if(mode == 2)
    {
        if(ui->btnRight->text() == "Turn Right")
        {
            drone->setTurnLeftRight(0.5);
            ui->btnRight->setText("Stop turn");
        }
        else
        {
            drone->setTurnLeftRight(0);
            ui->btnRight->setText("Turn Right");
        }
    }
    else
        qDebug("Mode souris désactivée\n");
}

void MainWindow::on_btnDown_clicked()
{
    if(mode == 2)
    {
        if(ui->btnDown->text() == "Down")
        {
            drone->setGoUpDown(-0.5);
            ui->btnDown->setText("Stop Down");
        }
        else
        {
            drone->setGoUpDown(0);
            ui->btnDown->setText("Down");
        }
    }
    else
        qDebug("Mode souris désactivée\n");
}

void MainWindow::on_btnFront_clicked()
{
    if(mode == 2)
    {
        if(ui->btnFront->text() == "Tilt front")
        {
            drone->setTiltFrontBack(-0.5);
            ui->btnFront->setText("Stop tilt");
        }
        else
        {
            drone->setTiltFrontBack(0);
            ui->btnFront->setText("Tilt front");
        }
    }
    else
        qDebug("Mode souris désactivée\n");
}

void MainWindow::on_btnBack_clicked()
{
    if(mode == 2)
    {
        if(ui->btnBack->text() == "Tilt back")
        {
            drone->setTiltFrontBack(0.5);
            ui->btnBack->setText("Stop tilt");
        }
        else
        {
            drone->setTiltFrontBack(0);
            ui->btnBack->setText("Tilt back");
        }
    }
    else
        qDebug("Mode souris désactivée\n");
}

void MainWindow::on_btnTiltLeft_clicked()
{
    if(mode == 2)
    {
        if(ui->btnTiltLeft->text() == "Tilt left")
        {
            drone->setTiltLeftRight(0.5);
            ui->btnTiltLeft->setText("Stop tilt");
        }
        else
        {
            drone->setTiltLeftRight(0);
            ui->btnTiltLeft->setText("Tilt left");
        }
    }
    else
        qDebug("Mode souris désactivée\n");
}

void MainWindow::on_btnTiltRight_clicked()
{
    if(mode == 2)
    {
        if(ui->btnTiltRight->text() == "Tilt right")
        {
            drone->setTiltLeftRight(-0.5);
            ui->btnTiltRight->setText("Stop tilt");
        }
        else
        {
            drone->setTiltLeftRight(0);
            ui->btnTiltRight->setText("Tilt right");
        }
    }
    else
        qDebug("Mode souris désactivée\n");
}

void MainWindow::on_btnConnect_clicked()
{
    drone->connectToDrone();
    drone->initDrone();
}

void MainWindow::on_btnARU_clicked()
{
    if(ui->btnARU->text() == "Arret Urgence")
    {
        drone->aru();
        ui->btnARU->setText("ok");
    }
    else
    {
        drone->aru();
        ui->btnARU->setText("Arret Urgence");
    }
}

void MainWindow::on_btnTakeOffLand_clicked()
{
    if(ui->btnARU->text() == "Take Off")
    {
        drone->takeoff();
        ui->btnARU->setText("Land");
    }
    else
    {
        drone->land();
        ui->btnARU->setText("Arret Urgence");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *k)
{
    if(mode == 1)
    {
        switch(k->key())
        {
        case Qt::Key_Z: drone->setGoUpDown(1); break;
        case Qt::Key_S: drone->setGoUpDown(-1); break;
        case Qt::Key_Q: drone->setTurnLeftRight(-1); break;
        case Qt::Key_D: drone->setTurnLeftRight(1); break;

        case Qt::Key_I: drone->setTiltFrontBack(-1); break;
        case Qt::Key_K: drone->setTiltFrontBack(1); break;
        case Qt::Key_J: drone->setTiltLeftRight(-1); break;
        case Qt::Key_L: drone->setTiltLeftRight(1); break;

        case Qt::Key_Space: drone->aru(); break;
        case Qt::Key_T: drone->takeoff(); break;
        case Qt::Key_Y: drone->land(); break;

        default: qDebug("This key is useless on press");
        }
    }
    else
        qDebug("Mode clavier desactive");
}

void MainWindow::keyReleaseEvent(QKeyEvent *k)
{
    if(mode == 1)
    {
        switch(k->key())
        {
        case Qt::Key_Z: drone->setGoUpDown(0); break;
        case Qt::Key_S: drone->setGoUpDown(0); break;
        case Qt::Key_Q: drone->setTurnLeftRight(0); break;
        case Qt::Key_D: drone->setTurnLeftRight(0); break;

        case Qt::Key_I: drone->setTiltFrontBack(0); break;
        case Qt::Key_K: drone->setTiltFrontBack(0); break;
        case Qt::Key_J: drone->setTiltLeftRight(0); break;
        case Qt::Key_L: drone->setTiltLeftRight(0); break;

        default: qDebug("This key is useless on release");
        }
    }
    else
        qDebug("Mode clavier desactive");
}

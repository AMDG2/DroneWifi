#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "ardrone.h"
#include "navdatawindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    navdataWindow *navdata;
    ~MainWindow();

private slots:
    void on_btnKeyboardMode_clicked();

    void on_btnMouseMode_clicked();

    void on_btnUp_clicked();

    void on_btnStat_clicked();

    void on_btnStat1_clicked();

    void on_btnLeft_clicked();

    void on_btnRight_clicked();

    void on_btnDown_clicked();

    void on_btnFront_clicked();

    void on_btnBack_clicked();

    void on_btnTiltLeft_clicked();

    void on_btnTiltRight_clicked();

    void on_btnConnect_clicked();

    void on_btnARU_clicked();

    void on_btnTakeOffLand_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    int mode;
    ARDrone *drone;

    void keyPressEvent(QKeyEvent *k);
    void keyReleaseEvent(QKeyEvent *k);
};

#endif // MAINWINDOW_H

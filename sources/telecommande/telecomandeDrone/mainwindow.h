#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "arcommand.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void updateStdout(QString data);

    void on_btnLand_clicked();

    void on_pushButton_2_clicked();

    void on_arretUrgence_clicked();

    void on_btn_up_clicked();

    void on_btn_vol_stat_clicked();

    void on_btn_up_clicked(bool checked);

    void on_btn_down_clicked(bool checked);

    void on_btn_left_clicked();

    void on_btn_right_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    arCommand *at;
};

#endif // MAINWINDOW_H

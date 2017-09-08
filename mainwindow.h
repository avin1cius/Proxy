#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include "producer.h"
#include "reader.h"
#include "semaphores.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Producer &producer;
    Reader &reader;
    Data &data;
    std::thread *tReader;
    std::thread &tProducer;

    Ui::MainWindow *ui;

public:
    explicit MainWindow( Producer &p, Reader &r, Data &d, std::thread *tR, std::thread &tP, QWidget *parent=0 );
    ~MainWindow();
    bool c_enable[5] = { false };
    bool s_enable;
    bool* sv_on;

private slots:
       //void iniciarServidor();
       void on_pushButton_released();

       void on_pushButton_2_released();

       void on_pushButton_3_released();

       void on_pushButton_4_released();

       void on_pushButton_5_released();

       void on_pushButton_6_released();
};

#endif // MAINWINDOW_H

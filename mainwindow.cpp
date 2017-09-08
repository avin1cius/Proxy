#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <thread>
#include "main.h"
#include "producer.h"
#include "reader.h"

MainWindow::MainWindow( Producer &p, Reader &r, Data &d, std::thread *tR, std::thread &tP, QWidget *parent)
    : producer(p), reader(r), data(d), tReader(tR), tProducer(tP), QMainWindow(parent), ui(new Ui::MainWindow)
{
    s_enable = false;

    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_released()
{
    //Servidor
    if( s_enable )
    {
        ui->pushButton->setText("Play");
        s_enable=false;
        QCoreApplication::quit();
    }
    else
    {
       tProducer = std::thread( &Producer::run, producer, ui->lineEdit->text().toStdString().c_str(), ui->lineEdit_2->text().toInt() );
       ui->pushButton->setText("Pause");
       ui->pushButton_2->setEnabled(true);
       ui->pushButton_3->setEnabled(true);
       ui->pushButton_4->setEnabled(true);
       ui->pushButton_5->setEnabled(true);
       ui->pushButton_6->setEnabled(true);
       s_enable=true;
       //tProducer.join();
    }
}

void MainWindow::on_pushButton_2_released()
{
    // Cliente 1
    if( c_enable[0] )
    {
        data.Start[0] = false;
        //tReader[0].join();
        ui->pushButton_2->setText("Play");
        c_enable[0]=false;
    }
    else
    {
        data.Start[0] = true;
        if( data.cPaused[0] )
        {
           producer.sem.V( producer.sem.c[0] );
           ui->pushButton_2->setText("Pause");
           c_enable[0]= true;
        }
        else
        {
            tReader[0] = std::thread( &Reader::run, reader, ui->lineEdit_3->text().toStdString(),
                                                              ui->lineEdit_4->text().toInt(), ui->checkBox->isChecked(), 0 );

            ui->pushButton_2->setText("Pause");
            c_enable[0]= true;
        }
    }
}

void MainWindow::on_pushButton_3_released()
{
    // Cliente 2
    if(c_enable[1])
    {
        data.Start[1] = false;
        //tReader[1].join();
        ui->pushButton_3->setText("Play");
        c_enable[1] = false;
    }
    else
    {
        data.Start[1] = true;
        if( data.cPaused[1] )
        {
           producer.sem.V( producer.sem.c[1] );
           ui->pushButton_3->setText("Pause");
           c_enable[1]= true;
        }
        else
        {
            tReader[1] = std::thread( &Reader::run, reader, ui->lineEdit_5->text().toStdString(),
                                                              ui->lineEdit_6->text().toInt(), ui->checkBox_2->isChecked(), 1 );

            ui->pushButton_3->setText("Pause");
            c_enable[1] = true;
        }
    }

}

void MainWindow::on_pushButton_4_released()
{
    // Cliente 3
    if( c_enable[2] )
    {
        data.Start[2] = false;
        //tReader[2].join();
        ui->pushButton_4->setText("Play");
        c_enable[2] = false;
    }
    else
    {
        data.Start[2] = true;
        if( data.cPaused[2] )
        {
           producer.sem.V( producer.sem.c[2] );
           ui->pushButton_4->setText("Pause");
           c_enable[2]= true;
        }
        else
        {
            tReader[2] = std::thread( &Reader::run, reader, ui->lineEdit_7->text().toStdString(),
                                                              ui->lineEdit_8->text().toInt(), ui->checkBox_3->isChecked(), 2 );

            ui->pushButton_4->setText("Pause");
            c_enable[2] = true;
        }
    }

}

void MainWindow::on_pushButton_5_released()
{
    // Cliente 4
    if( c_enable[3] )
    {
        data.Start[3] = false;
        //tReader[3].join();
        ui->pushButton_5->setText("Play");
        c_enable[3] = false;
    }
    else
    {
        data.Start[3] = true;
        if( data.cPaused[3] )
        {
           producer.sem.V( producer.sem.c[3] );
           ui->pushButton_5->setText("Pause");
           c_enable[3]= true;
        }
        else
        {
            tReader[3] = std::thread( &Reader::run, reader, ui->lineEdit_9->text().toStdString(),
                                                              ui->lineEdit_10->text().toInt(), ui->checkBox_4->isChecked(), 3 );

            ui->pushButton_5->setText("Pause");
            c_enable[3] = true;
        }
    }
}

void MainWindow::on_pushButton_6_released()
{
    // Cliente 5
    if( c_enable[4] )
    {
        data.Start[4] = false;
        //tReader[4].join();
        ui->pushButton_6->setText("Play");
        c_enable[4] = false;
    }
    else
    {
        data.Start[4] = true;
        if( data.cPaused[4] )
        {
           producer.sem.V( producer.sem.c[4] );
           ui->pushButton_6->setText("Pause");
           c_enable[4]= true;
        }
        else
        {
            tReader[4] = std::thread( &Reader::run, reader, ui->lineEdit_11->text().toStdString(),
                                                              ui->lineEdit_12->text().toInt(), ui->checkBox_5->isChecked(), 4 );

            ui->pushButton_6->setText("Pause");
            c_enable[4] = true;
        }
    }
}

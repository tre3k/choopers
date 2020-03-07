/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setMinimumHeight(200);
    this->setMinimumWidth(500);
    this->setGeometry(0,0,1024,600);
    this->setWindowTitle("Choppers ");

    CentralWidget *central_widget = new CentralWidget();
    this->setCentralWidget(central_widget);

}

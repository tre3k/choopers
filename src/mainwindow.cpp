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
    this->setGeometry(0,0,1024,650);
    this->setWindowTitle("Choppers ");

    /* default options */
    options.distance_range_max = 40.0;
    options.distance_range_min = 0.0;

    options.time_range_max = 500;
    options.time_range_min = 0;
    options.time_step = 1.0;

    options.lambda_max = 20;
    options.lambda_min = 1;
    options.lambda_step = 1;

    options.therads = 5;

    /* create central widget */

    CentralWidget *central_widget = new CentralWidget(this->statusBar());
    connect(this,SIGNAL(sendOptions(s_options)),central_widget,SLOT(getOptions(s_options)));
    emit sendOptions(options);
    this->setCentralWidget(central_widget);

    createMenuBar();
    createStatusBar();
}

void MainWindow::createMenuBar(){
    menu_bar.file_menu = new QMenu("file");
    menu_bar.option = new QAction("options");
    menu_bar.file_menu->addAction(menu_bar.option);

    this->menuBar()->addMenu(menu_bar.file_menu);

}

void MainWindow::createStatusBar(){

    this->statusBar()->show();
}



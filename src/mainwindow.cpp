/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QLocale locale("en_EN.UTF-8");
    this->setLocale(locale);
    this->setWindowIcon(QIcon(":/icons/icon.svg"));
    this->setMinimumHeight(200);
    this->setMinimumWidth(500);
    this->setGeometry(0,0,1024,650);
    this->setWindowTitle("Choopers ");

    //QFont fontDroid(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/fonts/droid.ttf")).at(0),9);
    //this->setFont(fontDroid);


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

    /* create about dialog */
    about_dialog = new AboutDialog();
    /* create options dialog */
    options_dialog = new OptionsDialog();
    connect(this,SIGNAL(sendOptions(s_options)),options_dialog,SLOT(getOptions(s_options)));

    /* create central widget */
    CentralWidget *central_widget = new CentralWidget(this->statusBar());
    connect(this,SIGNAL(sendOptions(s_options)),central_widget,SLOT(getOptions(s_options)));
    emit sendOptions(options);
    connect(options_dialog,SIGNAL(sendOptions(s_options)),central_widget,SLOT(getOptions(s_options)));
    this->setCentralWidget(central_widget);

    createMenuBar();
    createStatusBar();
}

void MainWindow::createMenuBar(){
    menu_bar.file_menu = new QMenu("&File");
    menu_bar.quit = new QAction("&Quit");
    menu_bar.tools_menu = new QMenu("&Tools");
    menu_bar.option = new QAction("&Options");
    menu_bar.help_menu = new QMenu("&Help");
    menu_bar.about = new QAction("&About");

    menu_bar.file_menu->addAction(menu_bar.quit);
    connect(menu_bar.quit,SIGNAL(triggered()),this,SLOT(action_quit()));
    menu_bar.tools_menu->addAction(menu_bar.option);
    connect(menu_bar.option,SIGNAL(triggered()),this,SLOT(action_options()));
    menu_bar.help_menu->addAction(menu_bar.about);
    connect(menu_bar.about,SIGNAL(triggered()),this,SLOT(action_about()));

    this->menuBar()->addMenu(menu_bar.file_menu);
    this->menuBar()->addMenu(menu_bar.tools_menu);
    this->menuBar()->addMenu(menu_bar.help_menu);

}

void MainWindow::createStatusBar(){
    emit sendOptions(options);
    this->statusBar()->show();
}


void MainWindow::action_options(){
    options_dialog->show();
}


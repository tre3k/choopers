/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QMenuBar>
#include <QMenuBar>
#include <QStatusBar>

#include "widgets.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    struct s_status_bar{
        QProgressBar *progressBar;
    } status_bar;

    struct s_menu_bar{
        QMenu *file_menu;
        QAction *quit;
        QMenu *tools_menu;
        QAction *calc;
        QAction *option;
        QMenu *help_menu;
        QAction *about;
    } menu_bar;

    void createStatusBar();
    void createMenuBar();

    s_options options;
    OptionsDialog *options_dialog;
    AboutDialog *about_dialog;
    CalculatorDialog *calc_dialog;


public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
    void sendStatusBar(QStatusBar *sb);
    void sendOptions(s_options opt);

public slots:
    void action_options(){
        options_dialog->show();
    }
    void action_quit(){
        QApplication::quit();
    }
    void action_about(){
        about_dialog->show();
    }
    void action_neutron_calc(){
        calc_dialog->show();
    }


};

#endif // MAINWINDOW_H

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
        QAction *option;
    } menu_bar;

    void createStatusBar();
    void createMenuBar();

    s_options options;


public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
    void sendStatusBar(QStatusBar *sb);
    void sendOptions(s_options opt);

public slots:


};

#endif // MAINWINDOW_H

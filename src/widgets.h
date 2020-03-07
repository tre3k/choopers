/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#ifndef WIDGETS_H
#define WIDGETS_H

#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QString>
#include <QSplitter>
#include <QVector>
#include <qcustomplot.h>

#include "calculate.h"

class ChooperWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChooperWidget(QString str_label = "chooper #", QWidget *parent = nullptr);

public:
    QLabel *label;
    QDoubleSpinBox *spinbox_distance;
    QDoubleSpinBox *spinbox_phase;
    QDoubleSpinBox *spinbox_period;
    QDoubleSpinBox *spinbox_duty;

private slots:
    void GenerateSignal(QString str_value){
        double distance,phase,period,duty;

        distance = spinbox_distance->value();
        phase = spinbox_phase->value();
        period = spinbox_period->value();
        duty = spinbox_duty->value();

        emit ValuesChanged(distance,phase,period,duty);
    }

signals:
    void ValuesChanged(double distance, double phase,double period,double duty);
};


class InteractivePlot : public QCustomPlot
{
    Q_OBJECT
public:
    explicit InteractivePlot(QWidget *parent = nullptr);
    ~InteractivePlot();

protected:
    bool x_log = false;
    bool y_log = false;

private:

public slots:

protected slots:
    void slot_sAxies_drag_zoom(QCPAxis *,QCPAxis::SelectablePart,QMouseEvent *);
    void slot_full_drag_zoom(QMouseEvent *);
    void slot_selectionChanged();
    void slot_contextMenuReq(QPoint p);

    void exportToPDF();
    void exportToBMP();
    void exportToJPG();
    void exportToPNG();

    void setXLog();
    void setYLog();

signals:

};


class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);

private:
    struct s_double_range{
        double min;
        double max;
    };
    s_double_range time_range;
    s_double_range distance_range;

    InteractivePlot *plot;

    ChooperWidget *chooper_widget1;
    ChooperWidget *chooper_widget2;
    ChooperWidget *chooper_widget3;
    ChooperWidget *chooper_widget4;

    QPushButton *button_calculate;

    QDoubleSpinBox *spinbox_sample_position;


    /* QCP Items */
    QCPItemText *text_sample_position = nullptr;
    QCPItemLine *line_sample_position = nullptr;

    QCPItemText *text_chooper[4];
    QVector<QCPItemLine *> lines_chooper[4];


private:
    void PlotRescaleAxis(void){
        plot->xAxis->setRange(time_range.min,time_range.max);
        plot->yAxis->setRange(distance_range.min,distance_range.max);
        plot->replot();
    }
    void paintChooper(int chooper_no, QString text, double distance, double phase,double period,double duty);
    s_windows getWindowsFromChooper(double distance, double phase,double period,double duty);

private slots:
    void paintSampleDistance(double distance);
    void paintChooper1(double distance, double phase,double period,double duty){paintChooper(0,"Chooper #1",distance,phase,period,duty);}
    void paintChooper2(double distance, double phase,double period,double duty){paintChooper(1,"Chooper #2",distance,phase,period,duty);}
    void paintChooper3(double distance, double phase,double period,double duty){paintChooper(2,"Chooper #3",distance,phase,period,duty);}
    void paintChooper4(double distance, double phase,double period,double duty){paintChooper(3,"Chooper #4",distance,phase,period,duty);}

public slots:
    void ReleaseCalculatrion();

};


#endif // WIDGETS_H
/* EOF */

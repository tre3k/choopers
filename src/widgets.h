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
#include <QStatusBar>

#include "calculate.h"
#include "dialogs.h"

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

private:
    QPushButton *addetional_button;
    ChopperDialog *chopper_dialog;

private slots:
    void GenerateSignal(QString str_value){
        double distance,phase,period,duty;

        distance = spinbox_distance->value();
        phase = spinbox_phase->value();
        period = spinbox_period->value();
        duty = spinbox_duty->value();

        emit ValuesChanged(distance,phase,period,duty);
    }

    void add_button_click(void);


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
    explicit CentralWidget(QStatusBar *sb = nullptr, QWidget *parent = nullptr);

private:
    s_options options;

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
    QDoubleSpinBox *spinbox_detector_position;


    /* QCP Items */
    QCPItemText *text_sample_position = nullptr;
    QCPItemLine *line_sample_position = nullptr;

    QCPItemText *text_detector_position = nullptr;
    QCPItemLine *line_detector_position = nullptr;

    QCPItemText *text_chooper[4];
    QVector<QCPItemLine *> lines_chooper[4];

    ResultDialog *rd;

    QVector<Neutron *> neutrons;

    /* Main Windows communication */
    QStatusBar *main_status_bar;

private:
    void PlotRescaleAxis(void){
        plot->xAxis->setRange(options.time_range_min,options.time_range_max);
        plot->yAxis->setRange(options.distance_range_min,options.distance_range_max);
        plot->replot();
    }
    void paintChooper(int chooper_no, QString text, double distance, double phase,double period,double duty); 
    QColor colorFromLambda(double lambda);

private slots:
    void paintDetectorPosition(double distance);
    void paintSampleDistance(double distance);
    void paintChooper1(double distance, double phase,double period,double duty){paintChooper(0,"Chopper #1",distance,phase,period,duty);}
    void paintChooper2(double distance, double phase,double period,double duty){paintChooper(1,"Chopper #2",distance,phase,period,duty);}
    void paintChooper3(double distance, double phase,double period,double duty){paintChooper(2,"Chopper #3",distance,phase,period,duty);}
    void paintChooper4(double distance, double phase,double period,double duty){paintChooper(3,"Chopper #4",distance,phase,period,duty);}

public slots:
    void ReleaseCalculatrion();
    void PlotNeutron(Neutron *neutron);
    void EndThread(CalculateThread *ct);

    void showMessageFromPlot(QCPAbstractPlottable *plot,int index,QMouseEvent *event){
        if(main_status_bar!=nullptr) main_status_bar->showMessage(plot->name());
    }

    void getOptions(s_options opt){
        options = opt;
        PlotRescaleAxis();

        paintDetectorPosition(spinbox_detector_position->value());
        paintSampleDistance(spinbox_sample_position->value());
        paintChooper1(chooper_widget1->spinbox_distance->value(),
                      chooper_widget1->spinbox_phase->value(),
                      chooper_widget1->spinbox_period->value(),
                      chooper_widget1->spinbox_duty->value());
        paintChooper2(chooper_widget2->spinbox_distance->value(),
                      chooper_widget2->spinbox_phase->value(),
                      chooper_widget2->spinbox_period->value(),
                      chooper_widget2->spinbox_duty->value());
        paintChooper3(chooper_widget3->spinbox_distance->value(),
                      chooper_widget3->spinbox_phase->value(),
                      chooper_widget3->spinbox_period->value(),
                      chooper_widget3->spinbox_duty->value());
        paintChooper4(chooper_widget4->spinbox_distance->value(),
                      chooper_widget4->spinbox_phase->value(),
                      chooper_widget4->spinbox_period->value(),
                      chooper_widget4->spinbox_duty->value());

    }

signals:
    void sendMinMaxLambda(double,double);
    void sendPercentLiveNeutrons(double);

};


#endif // WIDGETS_H
/* EOF */

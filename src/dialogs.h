#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QRadioButton>
#include <math.h>

#include <QDoubleSpinBox>

struct s_options{
    double time_range_max;
    double time_range_min;
    double time_step;

    double distance_range_max;
    double distance_range_min;

    double lambda_min;
    double lambda_max;
    double lambda_step;

    int therads;
};


/* Chopper dialog */
class ChopperDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChopperDialog(QString title, QWidget *parent = nullptr);


private:
    QDoubleSpinBox *spinbox_frequency;
    QDoubleSpinBox *spinbox_rpm;
    QRadioButton *radio_frequency;
    QRadioButton *radio_rpm;

    QSpinBox *spinbox_gaps;
    QDoubleSpinBox *spinbox_width_gap;
    QDoubleSpinBox *spinbox_cb;

    QPushButton *button_ok;
    QPushButton *button_close;

    double HzToRPM(double val){
        return 60.0*val;
    }
    double RPMToHz(double val){
        return val/60.0;
    }

private slots:
    void calculate();
    void close(){
        this->hide();
    }
    void FreqChanged(double value){
        spinbox_rpm->setValue(HzToRPM(value));
    }
    void RpmChanged(double value){
        spinbox_frequency->setValue(RPMToHz(value));
    }

signals:
    void sendDuty(double);
    void sendPeriod(double);


};

/* ResultDialog */
class ResultDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ResultDialog(QWidget *parent = nullptr);

private:
    QLabel *percentLabel = nullptr;
    QFormLayout *mainLayout = nullptr;

    QLabel *min_lambda = nullptr;
    QLabel *max_lambda = nullptr;

    QPushButton *close_button;
public slots:
    void showPercentNeutron(double);
    void recvMinMaxLambda(double min,double max){
        min_lambda->setText(QString::number(min)+" Å");
        max_lambda->setText(QString::number(max)+" Å");
    }
    void close_press(){
        this->hide();
    }
};

/* OptionsDialog */
class OptionsDialog : public QDialog
{
    Q_OBJECT
private:
    QDoubleSpinBox *spinbox_time_max;
    QDoubleSpinBox *spinbox_time_min;
    QDoubleSpinBox *spinbox_time_step;

    QDoubleSpinBox *spinbox_lambda_max;
    QDoubleSpinBox *spinbox_lambda_min;
    QDoubleSpinBox *spinbox_lambda_step;

    QDoubleSpinBox *spinbox_distance_max;
    QDoubleSpinBox *spinbox_distance_min;

    QSpinBox *spinbox_threads;

    QPushButton *button_ok,*button_close;

    s_options options;
public:
    explicit OptionsDialog(QWidget *parent = nullptr);

signals:
    void sendOptions(s_options);

public slots:
    void getOptions(s_options opt){
        options = opt;

        spinbox_lambda_max->setValue(options.lambda_max);
        spinbox_lambda_min->setValue(options.lambda_min);
        spinbox_lambda_step->setValue(options.lambda_step);

        spinbox_time_max->setValue(options.time_range_max);
        spinbox_time_min->setValue(options.time_range_min);
        spinbox_time_step->setValue(options.time_step);

        spinbox_distance_max->setValue(options.distance_range_max);
        spinbox_distance_min->setValue(options.distance_range_min);

        spinbox_threads->setValue(options.therads);
    }

private slots:
    void button_ok_press(){
        options.lambda_max = spinbox_lambda_max->value();
        options.lambda_min = spinbox_lambda_min->value();
        options.lambda_step = spinbox_lambda_step->value();

        options.time_range_max= spinbox_time_max->value();
        options.time_range_min = spinbox_time_min->value();
        options.time_step = spinbox_time_step->value();

        options.distance_range_max = spinbox_distance_max->value();
        options.distance_range_min = spinbox_distance_min->value();

        options.therads = spinbox_threads->value();

        emit sendOptions(options);
        this->hide();
    }

    void button_close_press(){
        this->hide();
    }

};


class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);

public slots:
    void close(){
        this->hide();
    }

};

#endif // DIALOGS_H

#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>

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

/* ResultDialog */
class ResultDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ResultDialog(QWidget *parent = nullptr);

private:
    QLabel *percentLabel = nullptr;
    QFormLayout *mainLayout = nullptr;

    QPushButton *close_button;
public slots:
    void showPercentNeutron(double);
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
    }

    void button_close_press(){
        this->hide();
    }

};

#endif // DIALOGS_H

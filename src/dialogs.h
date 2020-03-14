#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QLabel>
#include <QFormLayout>

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

class ResultDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ResultDialog(QWidget *parent = nullptr);

private:
    QLabel *percentLabel = nullptr;
    QFormLayout *mainLayout = nullptr;

public slots:
    void showPercentNeutron(double);
};

class OptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit OptionsDialog(QWidget *parent = nullptr);
};

#endif // DIALOGS_H

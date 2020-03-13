#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QLabel>
#include <QFormLayout>

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

#endif // DIALOGS_H

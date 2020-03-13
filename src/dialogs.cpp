/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#include "dialogs.h"

ResultDialog::ResultDialog(QWidget *parent) : QDialog(parent){
    percentLabel = new QLabel();
    mainLayout = new QFormLayout();

    mainLayout->addRow("Neutrons is live: ",percentLabel);

    this->setLayout(mainLayout);
}

void ResultDialog::showPercentNeutron(double percent){
    percentLabel->setText(QString::number(percent)+" %");
}


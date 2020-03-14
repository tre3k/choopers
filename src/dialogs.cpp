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


/* options dialog */
OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent){
    auto *spinboxs_layout = new QFormLayout();
    auto *main_layout = new QVBoxLayout();
    auto *button_layout = new QHBoxLayout();

    spinbox_lambda_max = new QDoubleSpinBox();
    spinbox_lambda_max->setSuffix(" Å");
    spinbox_lambda_max->setRange(0.01,999);

    spinbox_lambda_min = new QDoubleSpinBox();
    spinbox_lambda_min->setSuffix(" Å");
    spinbox_lambda_min->setRange(0.01,999);

    spinbox_lambda_step = new QDoubleSpinBox();
    spinbox_lambda_step->setSuffix(" Å");
    spinbox_lambda_step->setSingleStep(0.1);

    spinboxs_layout->addRow("λ<sub>min</sub>: ",spinbox_lambda_min);
    spinboxs_layout->addRow("λ<sub>max </sub>: ",spinbox_lambda_max);
    spinboxs_layout->addRow("λ<sub>step </sub>: ",spinbox_lambda_step);

    spinbox_time_max = new QDoubleSpinBox();
    spinbox_time_max->setSuffix(" ms");
    spinbox_time_max->setRange(0,99999);

    spinbox_time_min = new QDoubleSpinBox();
    spinbox_time_min->setSuffix(" ms");
    spinbox_time_min->setRange(0,99999);

    spinbox_time_step = new QDoubleSpinBox();
    spinbox_time_step->setSuffix(" ms");
    spinbox_time_step->setRange(0,9999);
    spinbox_time_step->setSingleStep(0.1);

    spinboxs_layout->addRow("time<sub>min </sub>: ",spinbox_time_min);
    spinboxs_layout->addRow("time<sub>max </sub>: ",spinbox_time_max);
    spinboxs_layout->addRow("time<sub>step </sub>: ",spinbox_time_step);

    spinbox_distance_max = new QDoubleSpinBox();
    spinbox_distance_max->setSuffix(" m");
    spinbox_distance_max->setRange(0,99);

    spinbox_distance_min = new QDoubleSpinBox();
    spinbox_distance_min->setSuffix(" m");
    spinbox_distance_min->setRange(0,99);

    spinboxs_layout->addRow("distance<sub>min </sub>: ",spinbox_distance_min);
    spinboxs_layout->addRow("distance<sub>max </sub>: ",spinbox_distance_max);


    button_close = new QPushButton("close");
    button_ok = new QPushButton("ok");

    button_layout->addStretch();
    button_layout->addWidget(button_ok);
    button_layout->addWidget(button_close);

    main_layout->addLayout(spinboxs_layout);
    main_layout->addLayout(button_layout);

    this->setLayout(main_layout);

    connect(button_ok,SIGNAL(clicked()),this,SLOT(button_ok_press()));
    connect(button_close,SIGNAL(clicked()),this,SLOT(button_close_press()));
}

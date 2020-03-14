/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#include "dialogs.h"

ChopperDialog::ChopperDialog(QString title, QWidget *parent) : QDialog(parent){
    this->setWindowTitle(title);
    this->setWindowIcon(QIcon(":/icons/sett.svg"));

    auto main_layout = new QVBoxLayout();
    auto form_layout = new QFormLayout();

    auto freq_layout = new QGridLayout();
    radio_frequency = new QRadioButton();
    radio_rpm = new QRadioButton();
    spinbox_frequency = new QDoubleSpinBox();
    spinbox_frequency->setSuffix(" Hz");
    spinbox_rpm = new QDoubleSpinBox();
    spinbox_rpm->setSuffix(" rpm");
    spinbox_frequency->setRange(0,999);
    spinbox_rpm->setRange(0,60*9999);

    spinbox_frequency->setMinimumHeight(20);
    spinbox_rpm->setMinimumHeight(20);

    freq_layout->setMargin(0);
    freq_layout->addWidget(spinbox_frequency,0,0); freq_layout->addWidget(radio_frequency,0,1);
    freq_layout->addWidget(spinbox_rpm,1,0); freq_layout->addWidget(radio_rpm,1,1);

    spinbox_gaps = new QSpinBox();
    spinbox_width_gap = new QDoubleSpinBox();
    spinbox_width_gap->setSuffix(" mm");
    spinbox_width_gap->setRange(0,99);
    spinbox_cb = new QDoubleSpinBox();
    spinbox_cb->setSuffix(" cm");
    spinbox_cb->setRange(0,99);
    spinbox_gaps->setRange(1,99);

    auto button_layout = new QHBoxLayout();
    button_ok = new QPushButton("ok");
    button_close = new QPushButton("close");

    button_layout->addStretch();
    button_layout->addWidget(button_ok);
    button_layout->addWidget(button_close);

    spinbox_frequency->setValue(10.0);
    spinbox_rpm->setValue(HzToRPM(spinbox_frequency->value()));
    spinbox_gaps->setValue(4);
    spinbox_width_gap->setValue(10);
    spinbox_cb->setValue(20);



    form_layout->addRow("frequency: ",freq_layout);
    form_layout->addRow("gaps: ",spinbox_gaps);
    form_layout->addRow("widht of gap: ",spinbox_width_gap);
    form_layout->addRow("distance center to beam: ",spinbox_cb);
    main_layout->addLayout(form_layout);
    main_layout->addLayout(button_layout);

    this->setLayout(main_layout);

    connect(radio_rpm,SIGNAL(toggled(bool)),spinbox_rpm,SLOT(setEnabled(bool)));
    connect(radio_frequency,SIGNAL(toggled(bool)),spinbox_frequency,SLOT(setEnabled(bool)));

    radio_frequency->setChecked(true);
    spinbox_rpm->setDisabled(true);

    connect(button_ok,SIGNAL(clicked()),this,SLOT(calculate()));
    connect(button_close,SIGNAL(clicked()),this,SLOT(close()));

    connect(spinbox_frequency,SIGNAL(valueChanged(double)),this,SLOT(FreqChanged(double)));
    connect(spinbox_rpm,SIGNAL(valueChanged(double)),this,SLOT(RpmChanged(double)));
}

void ChopperDialog::calculate(){
    double duty;
    double period;

    double freq = spinbox_frequency->value();
    int gaps = spinbox_gaps->value();
    double widht_gap = spinbox_width_gap->value();
    double cb = spinbox_cb->value();

    period = 1000.0/freq/gaps;          // in ms
    duty = 10*widht_gap/(2*M_PI*cb/gaps);

    emit sendDuty(duty);
    emit sendPeriod(period);
}


ResultDialog::ResultDialog(QWidget *parent) : QDialog(parent){
    this->setWindowTitle("Result");

    percentLabel = new QLabel();
    min_lambda = new QLabel();
    max_lambda = new QLabel();
    mainLayout = new QFormLayout();
    auto button_layout = new QHBoxLayout();

    close_button = new QPushButton("close");
    connect(close_button,SIGNAL(clicked()),this,SLOT(close_press()));

    button_layout->addStretch();
    button_layout->addWidget(close_button);

    mainLayout->addRow("Neutrons is live: ",percentLabel);
    mainLayout->addRow("Minimum wavelenght: ",min_lambda);
    mainLayout->addRow("Maximum wavelenght: ",max_lambda);
    mainLayout->addRow(button_layout);

    this->setLayout(mainLayout);
}

void ResultDialog::showPercentNeutron(double percent){
    percentLabel->setText(QString::number(percent)+" %");
}


/* options dialog */
OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent){
    this->setWindowTitle("Options");

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

    spinbox_threads = new QSpinBox();
    spinboxs_layout->addRow("threads (cpus-1): ",spinbox_threads);

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


AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent){
    this->setWindowTitle("About");
    this->setWindowIcon(QIcon(":/icons/about.svg"));

    auto close_button = new QPushButton("close");
    auto main_layout = new QVBoxLayout();
    auto button_layout = new QHBoxLayout();
    auto label = new QLabel("<b>Choopers v1.0</b><br><hr>"
                            "Under license GNU GLPv3 (c) 2020<br>"
                            "Petersburg Nuclear Physics Institute named by B.P.Konstantinov of NRC «Kurchatov Institute» PNPI<br>"
                            "Saint-Petersburg State University<br>"
                            "Especially for Luba, Autor: Kirill Pshenichnyi (pshcyrill@mail.ru)<br><hr>"
                            "GNU GPLv3, Source code: <a href=\"https://github.com/tre3k/choopers\">GitHub: https://github.com/tre3k/choopers</a>");

    main_layout->addWidget(label);

    connect(close_button,SIGNAL(clicked()),this,SLOT(close()));

    button_layout->addStretch();
    button_layout->addWidget(close_button);
    main_layout->addLayout(button_layout);
    this->setLayout(main_layout);

}

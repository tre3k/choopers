/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#include "dialogs.h"

ChopperDialog::ChopperDialog(QString title, QWidget *parent) : QDialog(parent){
    QLocale locale("en_EN.UTF-8");
    this->setLocale(locale);
    this->setWindowTitle(title);
    this->setWindowIcon(QIcon(":/icons/sett.svg"));

    auto main_layout = new QVBoxLayout();
    auto form_layout = new QFormLayout();

    auto freq_layout = new QGridLayout();
    auto gap_layout = new QGridLayout();

    radio_frequency = new QRadioButton();
    radio_rpm = new QRadioButton();
    spinbox_frequency = new QDoubleSpinBox();
    spinbox_frequency->setSuffix(" Hz");
    spinbox_rpm = new QDoubleSpinBox();
    spinbox_rpm->setSuffix(" rpm");
    spinbox_frequency->setRange(0,999);
    spinbox_rpm->setRange(0,60*9999);


    freq_layout->setMargin(0);
    freq_layout->addWidget(spinbox_frequency,0,0); freq_layout->addWidget(radio_frequency,0,1);
    freq_layout->addWidget(spinbox_rpm,1,0); freq_layout->addWidget(radio_rpm,1,1);
    spinbox_frequency->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    spinbox_gaps = new QSpinBox();
    spinbox_width_gap = new QDoubleSpinBox();
    spinbox_width_gap->setSuffix(" mm");
    spinbox_width_gap->setRange(0,9999);
    radio_widht_gap = new QRadioButton();
    spinbox_angle = new QDoubleSpinBox();
    spinbox_angle->setSuffix(" deg.");
    spinbox_angle->setRange(0,360);

    radio_angle_gap = new QRadioButton();
    gap_layout->addWidget(new QLabel("width: "),0,0);
    gap_layout->addWidget(spinbox_width_gap,0,1);
    gap_layout->addWidget(radio_widht_gap,0,2);
    gap_layout->addWidget(new QLabel("angle: "),1,0);
    gap_layout->addWidget(spinbox_angle,1,1);
    gap_layout->addWidget(radio_angle_gap,1,2);
    gap_layout->addWidget(new QLabel("gaps: "),2,0);
    gap_layout->addWidget(spinbox_gaps,2,1);
    spinbox_width_gap->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

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
    spinbox_gaps->setValue(2);
    spinbox_width_gap->setValue(10);
    spinbox_cb->setValue(30);

    //form_layout->addRow("frequency: ",freq_layout);
    auto group_freq_box = new QGroupBox("Frequency");
    group_freq_box->setLayout(freq_layout);

    auto gap_group_layout = new QGroupBox("Gap");
    gap_group_layout->setLayout(gap_layout);

    form_layout->addRow(group_freq_box);
    form_layout->addRow(gap_group_layout);
    form_layout->addRow("distance center to beam (r): ",spinbox_cb);
    main_layout->addLayout(form_layout);
    main_layout->addLayout(button_layout);

    this->setLayout(main_layout);

    connect(radio_rpm,SIGNAL(toggled(bool)),spinbox_rpm,SLOT(setEnabled(bool)));
    connect(radio_frequency,SIGNAL(toggled(bool)),spinbox_frequency,SLOT(setEnabled(bool)));
    radio_frequency->setChecked(true);
    spinbox_rpm->setDisabled(true);

    connect(radio_angle_gap,SIGNAL(toggled(bool)),spinbox_angle,SLOT(setEnabled(bool)));
    connect(radio_widht_gap,SIGNAL(toggled(bool)),spinbox_width_gap,SLOT(setEnabled(bool)));
    radio_angle_gap->setChecked(true);
    spinbox_width_gap->setDisabled(true);

    connect(spinbox_frequency,SIGNAL(valueChanged(double)),this,SLOT(FreqChanged(double)));
    connect(spinbox_rpm,SIGNAL(valueChanged(double)),this,SLOT(RpmChanged(double)));

    connect(spinbox_angle,SIGNAL(valueChanged(double)),this,SLOT(AngleChanged(double)));
    connect(spinbox_width_gap,SIGNAL(valueChanged(double)),this,SLOT(WidhtChanged(double)));
    connect(spinbox_cb,SIGNAL(valueChanged(double)),this,SLOT(CBChanged(double)));
    spinbox_angle->setValue(120);

    connect(button_ok,SIGNAL(clicked()),this,SLOT(calculate()));
    connect(button_close,SIGNAL(clicked()),this,SLOT(close()));
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
    auto form_layout = new QFormLayout();
    auto main_layout = new QVBoxLayout();

    auto button_layout = new QHBoxLayout();

    plot_bars = new InteractivePlot();
    plot_bars->setMinimumSize(600,400);
    plot_bars->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    bars = new QCPBars(plot_bars->xAxis,plot_bars->yAxis);
    QCPSelectionDecorator *s_decorator_for_bars = new QCPSelectionDecorator;
    QPen bars_pen_normal;
    QPen bars_pen_sel;
    QBrush bars_brush_normal(QColor("#a35d8135"));
    QBrush bars_brush_sel(QColor("#83c510"));

    bars_pen_normal.setColor("#4e5dd8");
    bars_pen_normal.setWidth(1);
    bars_pen_sel.setColor("#233aee");
    bars_pen_sel.setWidth(2);

    bars->setPen(bars_pen_normal);
    bars->setBrush(bars_brush_normal);

    s_decorator_for_bars->setPen(bars_pen_sel);
    s_decorator_for_bars->setBrush(bars_brush_sel);

    bars->setSelectionDecorator(s_decorator_for_bars);

    plot_bars->xAxis->setLabel("λ, Å");
    plot_bars->yAxis->setLabel("count");


    close_button = new QPushButton("close");
    connect(close_button,SIGNAL(clicked()),this,SLOT(close_press()));

    button_layout->addStretch();
    button_layout->addWidget(close_button);

    form_layout->setSizeConstraint(QLayout::SetFixedSize);
    form_layout->addRow("Neutrons is live: ",percentLabel);
    form_layout->addRow("Minimum wavelenght: ",min_lambda);
    form_layout->addRow("Maximum wavelenght: ",max_lambda);
    form_layout->addRow(button_layout);
    main_layout->addWidget(plot_bars);
    main_layout->addLayout(form_layout);


    this->setLayout(main_layout);
}

void ResultDialog::buildHistogramm(QVector<double> key, QVector<double> value){
    bars->data().clear();
    bars->setData(key,value);
    if(key.size() > 2){
        bars->setWidth((key.at(1)-key.at(0))*0.8);
    }else{
        bars->setWidth(1.0);
    }
    plot_bars->rescaleAxes(true);
    plot_bars->replot();
}

void ResultDialog::showPercentNeutron(double percent){
    percentLabel->setText(QString::number(percent)+" %");
}


/* options dialog */
OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent){
    this->setWindowTitle("Options");

    QLocale locale("en_EN.UTF-8");
    this->setLocale(locale);

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
    spinbox_lambda_step->setRange(0.01,999);
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
    spinbox_time_step->setRange(0.01,9999);
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
    spinbox_threads->setRange(1,1024);

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
    auto pixmap_label = new QLabel();
    QPixmap logo(":/icons/icon.svg");
    pixmap_label->setPixmap(logo.scaled(70,70,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    auto label = new QLabel("<b>Choopers v1.1</b><br><hr>"
                            "Under license GNU GLPv3 (c) 2020<br>"
                            "Petersburg Nuclear Physics Institute named by B.P.Konstantinov of NRC «Kurchatov Institute» PNPI<br>"
                            "Saint-Petersburg State University<br>"
                            "Especially for Luba, Autor: Kirill Pshenichnyi (pshcyrill@mail.ru)<br><hr>"
                            "GNU GPLv3, Source code: <a href=\"https://github.com/tre3k/choopers\">GitHub: https://github.com/tre3k/choopers</a>");

    main_layout->addWidget(pixmap_label);
    main_layout->addWidget(label);

    connect(close_button,SIGNAL(clicked()),this,SLOT(close()));

    button_layout->addStretch();
    button_layout->addWidget(close_button);
    main_layout->addLayout(button_layout);
    this->setLayout(main_layout);

}



CalculatorDialog::CalculatorDialog(QWidget *parent) : QDialog(parent){
    spinbox_lambda = new QDoubleSpinBox();
    spinbox_lambda->setRange(0.01,999.99);
    spinbox_lambda->setSingleStep(0.1);
    spinbox_lambda->setSuffix(" Å");
    button_close = new QPushButton("close");
    label_speed = new QLabel();
    label_energy_joul = new QLabel();
    label_energy_mev = new QLabel();
    label_energy_kelvin = new QLabel();

    label_speed->setAlignment(Qt::AlignRight);
    label_energy_joul->setAlignment(Qt::AlignRight);
    label_energy_mev->setAlignment(Qt::AlignRight);
    label_energy_kelvin->setAlignment(Qt::AlignRight);

    auto form_layout = new QFormLayout(this);

    auto button_layout = new QHBoxLayout();
    button_layout->addStretch();
    button_layout->addWidget(button_close);
    auto v_button_layout = new QVBoxLayout();
    v_button_layout->addStretch();
    v_button_layout->addLayout(button_layout);

    form_layout->addRow("Wavelenght: ",spinbox_lambda);
    form_layout->addRow("Speed: ",label_speed);
    form_layout->addRow("Energy (Joul): ",label_energy_joul);
    form_layout->addRow("Energy (meV): ",label_energy_mev);
    form_layout->addRow("Energy (K): ",label_energy_kelvin);
    form_layout->addRow(v_button_layout);

    connect(spinbox_lambda,SIGNAL(valueChanged(double)),this,SLOT(calculate(double)));
    connect(button_close,SIGNAL(clicked()),this,SLOT(close()));

    spinbox_lambda->setValue(6.0);

}

void CalculatorDialog::calculate(double value){
    Neutron n;
    n.setWavelength(value);
    n.setStartTime(0);
    label_speed->setText(QString::number(n.getVelosity())+" m/s");
    label_energy_joul->setText(QString::number(n.getEnergyJoul())+ " Joul");
    label_energy_mev->setText(QString::number(n.getEnergyMev())+ " meV");
    label_energy_kelvin->setText(QString::number(n.getEnergyKelvin())+ " K");
}


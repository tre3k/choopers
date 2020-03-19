/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#include "widgets.h"

ChooperWidget::ChooperWidget(QString str_label, QWidget *parent) : QWidget(parent)
{
    auto main_layout = new QVBoxLayout(this);
    auto form_layout = new QFormLayout();
    auto title_layout = new QHBoxLayout();

    chopper_dialog = new ChopperDialog(str_label);

    title_layout->setSpacing(0);
    title_layout->setMargin(0);
    addetional_button = new QPushButton();
    addetional_button->setIcon(QIcon(":/icons/sett.svg"));
    connect(addetional_button,SIGNAL(clicked()),this,SLOT(add_button_click()));

    label  = new QLabel("<b>"+str_label+":</b>");
    title_layout->addWidget(label);
    title_layout->addStretch();
    title_layout->addWidget(addetional_button);
    main_layout->addLayout(title_layout);
    main_layout->addLayout(form_layout);

    spinbox_distance = new QDoubleSpinBox();
    spinbox_distance->setSuffix(" m");
    spinbox_distance->setRange(0,99999);
    spinbox_distance->setSingleStep(0.1);

    spinbox_phase = new QDoubleSpinBox();
    spinbox_phase->setSuffix(" ms");
    spinbox_phase->setRange(-9999,9999);

    spinbox_period = new QDoubleSpinBox();
    spinbox_period->setSuffix(" ms");
    spinbox_period->setRange(0,99999);

    spinbox_duty = new QDoubleSpinBox();
    spinbox_duty->setRange(0,100);
    spinbox_duty->setSuffix(" %");

    form_layout->addRow("position: ", spinbox_distance);
    form_layout->addRow("phase: ", spinbox_phase);
    form_layout->addRow("period: ",spinbox_period);
    form_layout->addRow("duty: ", spinbox_duty);

    connect(spinbox_distance,SIGNAL(valueChanged(QString)),this,SLOT(GenerateSignal(QString)));
    connect(spinbox_phase,SIGNAL(valueChanged(QString)),this,SLOT(GenerateSignal(QString)));
    connect(spinbox_period,SIGNAL(valueChanged(QString)),this,SLOT(GenerateSignal(QString)));
    connect(spinbox_duty,SIGNAL(valueChanged(QString)),this,SLOT(GenerateSignal(QString)));

    connect(chopper_dialog,SIGNAL(sendDuty(double)),spinbox_duty,SLOT(setValue(double)));
    connect(chopper_dialog,SIGNAL(sendPeriod(double)),spinbox_period,SLOT(setValue(double)));
}

void ChooperWidget::add_button_click(){
    chopper_dialog->show();
}


/* Central Widget */
CentralWidget::CentralWidget(QStatusBar *sb, QWidget *parent) : QWidget(parent){
    auto main_layout = new QHBoxLayout(this);
    auto right_widget = new QWidget();
    auto right_layout = new QVBoxLayout(right_widget);
    auto sample_position_layout = new QFormLayout();

    auto splitter = new QSplitter(Qt::Horizontal);

    button_calculate = new QPushButton("Calculate");

    spinbox_detector_position = new QDoubleSpinBox();
    spinbox_detector_position->setSuffix(" m");
    spinbox_detector_position->setSingleStep(0.1);

    spinbox_sample_position = new QDoubleSpinBox();
    spinbox_sample_position->setSuffix(" m");
    spinbox_sample_position->setSingleStep(0.1);
    sample_position_layout->addRow("Detector position: ",spinbox_detector_position);
    sample_position_layout->addRow("Sample position: ",spinbox_sample_position);

    main_layout->setMargin(0);
    main_layout->addWidget(splitter);

    for(int i=0;i<4;i++) text_chooper[i] = nullptr;

    plot = new InteractivePlot();
    plot->xAxis->setLabel("time, ms");
    plot->yAxis->setLabel("Distance, m");
    PlotRescaleAxis();

    chooper_widget1 = new ChooperWidget("Chopper #1");
    chooper_widget2 = new ChooperWidget("Chopper #2");
    chooper_widget3 = new ChooperWidget("Chopper #3");
    chooper_widget4 = new ChooperWidget("Chopper #4");

    right_layout->addLayout(sample_position_layout);
    right_layout->addWidget(chooper_widget1);
    right_layout->addWidget(chooper_widget2);
    right_layout->addWidget(chooper_widget3);
    right_layout->addWidget(chooper_widget4);
    right_layout->addStretch();
    right_layout->addWidget(button_calculate);

    splitter->addWidget(plot);
    splitter->addWidget(right_widget);
    QList<int> splitter_sizes;
    splitter_sizes.append(0.8 * 1024);
    splitter_sizes.append(0.2 * 1024);
    splitter->setSizes(splitter_sizes);


    connect(spinbox_detector_position,SIGNAL(valueChanged(double)),
            this,SLOT(paintDetectorPosition(double)));
    connect(spinbox_sample_position,SIGNAL(valueChanged(double)),
            this,SLOT(paintSampleDistance(double)));
    connect(chooper_widget1,SIGNAL(ValuesChanged(double,double,double,double)),
            this,SLOT(paintChooper1(double,double,double,double)));
    connect(chooper_widget2,SIGNAL(ValuesChanged(double,double,double,double)),
            this,SLOT(paintChooper2(double,double,double,double)));
    connect(chooper_widget3,SIGNAL(ValuesChanged(double,double,double,double)),
            this,SLOT(paintChooper3(double,double,double,double)));
    connect(chooper_widget4,SIGNAL(ValuesChanged(double,double,double,double)),
            this,SLOT(paintChooper4(double,double,double,double)));
    connect(button_calculate,SIGNAL(clicked()),this,SLOT(ReleaseCalculatrion()));


    // default value for sample position
    spinbox_sample_position->setValue(20);
    spinbox_detector_position->setValue(25);

    chooper_widget1->spinbox_distance->setValue(2.8);
    chooper_widget1->spinbox_period->setValue(50);
    chooper_widget1->spinbox_phase->setValue(55);
    chooper_widget1->spinbox_duty->setValue(66.67);

    chooper_widget2->spinbox_distance->setValue(2.30);
    chooper_widget2->spinbox_period->setValue(50);
    chooper_widget2->spinbox_phase->setValue(38);
    chooper_widget2->spinbox_duty->setValue(66.67);

    chooper_widget3->spinbox_distance->setValue(1.50);
    chooper_widget3->spinbox_period->setValue(50);
    chooper_widget3->spinbox_phase->setValue(19);
    chooper_widget3->spinbox_duty->setValue(66.67);

    chooper_widget4->spinbox_distance->setValue(1);
    chooper_widget4->spinbox_period->setValue(50);
    chooper_widget4->spinbox_phase->setValue(0);
    chooper_widget4->spinbox_duty->setValue(66.67);

    rd = new ResultDialog();
    connect(this,SIGNAL(sendPercentLiveNeutrons(double)),rd,SLOT(showPercentNeutron(double)));
    connect(this,SIGNAL(sendMinMaxLambda(double,double)),rd,SLOT(recvMinMaxLambda(double,double)));
    connect(this,SIGNAL(sendDataForHistogramm(QVector<double>,QVector<double>)),rd,SLOT(buildHistogramm(QVector<double>,QVector<double>)));

    main_status_bar = sb;
    main_status_bar->showMessage("done.");
    connect(plot,SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)),this,SLOT(showMessageFromPlot(QCPAbstractPlottable*,int,QMouseEvent*)));
}

void CentralWidget::paintSampleDistance(double distance){
    if(text_sample_position != nullptr) delete text_sample_position;
    if(line_sample_position != nullptr) delete line_sample_position;
    text_sample_position = new QCPItemText(plot);
    text_sample_position->setText("Sample position");
    text_sample_position->setColor(QColor("red"));
    text_sample_position->setPositionAlignment(Qt::AlignLeft|Qt::AlignBottom);
    text_sample_position->position->setCoords(10,distance);

    line_sample_position = new QCPItemLine(plot);
    QPen line_pen;
    line_pen.setColor(QColor("red"));
    line_pen.setWidth(2);
    line_sample_position->setPen(line_pen);
    line_sample_position->start->setCoords(plot->xAxis->range().lower,distance);
    line_sample_position->end->setCoords(plot->xAxis->range().upper,distance);

    plot->replot();

}

void CentralWidget::paintDetectorPosition(double distance){
    if(text_detector_position != nullptr) delete text_detector_position;
    if(line_detector_position != nullptr) delete line_detector_position;
    text_detector_position = new QCPItemText(plot);
    text_detector_position->setText("Detector position");
    text_detector_position->setColor(QColor("green"));
    text_detector_position->setPositionAlignment(Qt::AlignLeft|Qt::AlignBottom);
    text_detector_position->position->setCoords(10,distance);

    line_detector_position = new QCPItemLine(plot);
    QPen line_pen;
    line_pen.setColor(QColor("green"));
    line_pen.setWidth(2);
    line_detector_position->setPen(line_pen);
    line_detector_position->start->setCoords(plot->xAxis->range().lower,distance);
    line_detector_position->end->setCoords(plot->xAxis->range().upper,distance);

    plot->replot();
}


void CentralWidget::paintChooper(int chooper_no, QString text, double distance, double phase, double period, double duty){
    if(text_chooper[chooper_no] != nullptr) delete text_chooper[chooper_no];
    text_chooper[chooper_no] = new QCPItemText(plot);
    text_chooper[chooper_no]->setText(text);
    text_chooper[chooper_no]->setPositionAlignment(Qt::AlignLeft|Qt::AlignBottom);
    text_chooper[chooper_no]->setPositionAlignment(Qt::AlignLeft|Qt::AlignBottom);
    text_chooper[chooper_no]->position->setCoords(10,distance);


    for(int i=0;i<lines_chooper[chooper_no].count();i++) delete lines_chooper[chooper_no].at(i);
    lines_chooper[chooper_no].clear();

    int N_lines = (options.time_range_max - options.time_range_min)/period;
    double x_coord = phase, y_coord = distance;
    QCPItemLine *item_line;
    QPen line_pen;
    line_pen.setColor(QColor("black"));
    line_pen.setWidth(3);
    for(int i=-1;i<N_lines;i++){
        item_line = new QCPItemLine(plot);
        item_line->setPen(line_pen);
        x_coord = phase+i*period;
        item_line->start->setCoords(x_coord,y_coord);
        x_coord = phase+(i+1)*period - period*duty/100;
        item_line->end->setCoords(x_coord,y_coord);
        lines_chooper[chooper_no].append(item_line);
    }
    plot->replot();
}

void CentralWidget::ReleaseCalculatrion(){
    plot->clearGraphs();
    PlotRescaleAxis();
    plot->replot();
    QThread::msleep(200);
    for(int i=0;i<v_threads.size();i++){
        if(v_threads.at(i)!=nullptr){
            if(v_threads.at(i)->isRunning()) v_threads.at(i)->terminate();
            delete v_threads.at(i);
        }
    }
    v_threads.clear();

    double time,lambda;

    double time_min, time_max, time_step;
    double lambda_min,lambda_max,lambda_step;

    lambda_min = options.lambda_min;
    lambda_max = options.lambda_max;
    lambda_step = options.lambda_step;

    time_min = options.time_range_min;
    time_max = options.time_range_max;
    time_step = options.time_step;

    int threads = options.therads;

    for(int i=0;i<neutrons.size();i++) delete neutrons.at(i);
    neutrons.clear();

    windows *wins1 = new windows();
    wins1->setTimeRange(time_min,time_max);
    wins1->setWindows(chooper_widget1->spinbox_distance->value(),
                      chooper_widget1->spinbox_phase->value(),
                      chooper_widget1->spinbox_period->value(),
                      chooper_widget1->spinbox_duty->value());
    windows *wins2 = new windows();
    wins2->setTimeRange(time_min,time_max);
    wins2->setWindows(chooper_widget2->spinbox_distance->value(),
                      chooper_widget2->spinbox_phase->value(),
                      chooper_widget2->spinbox_period->value(),
                      chooper_widget2->spinbox_duty->value());
    windows *wins3 = new windows();
    wins3->setTimeRange(time_min,time_max);
    wins3->setWindows(chooper_widget3->spinbox_distance->value(),
                      chooper_widget3->spinbox_phase->value(),
                      chooper_widget3->spinbox_period->value(),
                      chooper_widget3->spinbox_duty->value());
    windows *wins4 = new windows();
    wins4->setTimeRange(time_min,time_max);
    wins4->setWindows(chooper_widget4->spinbox_distance->value(),
                      chooper_widget4->spinbox_phase->value(),
                      chooper_widget4->spinbox_period->value(),
                      chooper_widget4->spinbox_duty->value());

    /* Generate Neutrons */
    time = time_min;
    while(time <= time_max){
        lambda = lambda_min;
        while(lambda <= lambda_max){
            Neutron *neutron = new Neutron(time);                     // time of birth
            neutron->setWavelength(lambda);
            neutron->setSampleDistance(spinbox_sample_position->value());

            neutron->addChopper(wins4);
            neutron->addChopper(wins3);
            neutron->addChopper(wins2);
            neutron->addChopper(wins1);
            lambda += lambda_step;

            neutrons.append(neutron);
        }
        time += time_step;
    }


    int n_from,n_to;
    int count_for_thread = neutrons.size()/threads;
    qDebug() << "neutrons: " << neutrons.size();
    qDebug() << "count: " << count_for_thread;
    for(int i=0;i<=threads;i++){
        n_from = i*count_for_thread;
        n_to = (i+1)*count_for_thread;
        if(threads==i) n_to = neutrons.size()-n_from;

        CalculateThread *ct = new CalculateThread();
        v_threads.append(ct);
        connect(ct,SIGNAL(PlotNeutron(Neutron*)),this,SLOT(PlotNeutron(Neutron*)));
        connect(ct,SIGNAL(end(CalculateThread*)),this,SLOT(EndThread(CalculateThread*)));
        ct->setVisibleTimeAndPoint(time_max,5);
        ct->setNeutrons(&neutrons,n_from,n_to);
        ct->start();
    }

    rd->show();
}


void CentralWidget::PlotNeutron(Neutron *neutron){
    plot->addGraph();
    plot->graph()->setName("λ = "+QString::number(neutron->getWavelenght())+" Å, start time: "+QString::number(neutron->getStartTime())+" ms");
    plot->graph()->setData(neutron->v_time,neutron->v_distance);
    QPen pen_style;
    QPen pen_style_selection;
    QCPSelectionDecorator *decor = new QCPSelectionDecorator();

    pen_style.setWidth(1);
    pen_style.setColor(colorFromLambda(neutron->getWavelenght()));

    pen_style_selection.setWidth(2);
    pen_style_selection.setColor(colorFromLambda(neutron->getWavelenght()));
    decor->setPen(pen_style_selection);

    plot->graph()->setPen(pen_style);
    plot->graph()->setSelectionDecorator(decor);

    plot->replot();

    colorFromLambda(neutron->getWavelenght());
}

void CentralWidget::EndThread(CalculateThread *ct){
    disconnect(ct,SIGNAL(PlotNeutron(Neutron*)),this,SLOT(PlotNeutron(Neutron*)));
    disconnect(ct,SIGNAL(end(CalculateThread*)),this,SLOT(EndThread(CalculateThread*)));

    QVector<Neutron *> v_live_neutrons;
    int live_neutrons = 0;
    double min_lambda = options.lambda_max;
    double max_lambda = options.lambda_min;
    for(int i = 0;i<neutrons.size();i++){
        if(neutrons.at(i)->isLive()){
            if(neutrons.at(i)->getWavelenght() > max_lambda) max_lambda = neutrons.at(i)->getWavelenght();
            if(neutrons.at(i)->getWavelenght() < min_lambda) min_lambda = neutrons.at(i)->getWavelenght();
            v_live_neutrons.append(neutrons.at(i));
            live_neutrons ++;
        }
    }
    emit sendMinMaxLambda(min_lambda,max_lambda);
    emit sendPercentLiveNeutrons(100.0*live_neutrons/(double)neutrons.size());

    int val;
    QVector<double> lambda,value;
    for(double wl=options.lambda_min;wl<=options.lambda_max;wl+=options.lambda_step){
        lambda.append(wl);
        val = 0;
        for(int i=0;i<v_live_neutrons.size();i++){
            if(v_live_neutrons.at(i)->isLive() && v_live_neutrons.at(i)->getWavelenght()==wl) val++;
        }
        value.append(val);
    }
    emit sendDataForHistogramm(lambda,value);

}

QColor CentralWidget::colorFromLambda(double lambda){
    QColor retval;
    const double max_color = 235;
    const double min_color = 20;
    const double alpha = 200;
    double r = 0,g = r,b = r;
    double delta_lambda = (options.lambda_max - options.lambda_min)/6;
    double coeff;

    /* #1 interval */
    if(lambda >= options.lambda_min && lambda < options.lambda_min+delta_lambda){
        coeff = (lambda-options.lambda_min)/delta_lambda;
        r = min_color + coeff*(max_color-min_color); g = min_color; b = max_color;
    }

    /* #2 interval */
    if(lambda >= options.lambda_min + delta_lambda && lambda < options.lambda_min + 2*delta_lambda){
        coeff = (lambda - options.lambda_min - delta_lambda)/delta_lambda;
        r = min_color; g = max_color - coeff*(max_color-min_color); b = max_color;
    }

    /* #3 interval */
    if(lambda >= options.lambda_min + 2*delta_lambda && lambda < options.lambda_min + 3*delta_lambda){
        coeff = (lambda-options.lambda_min - 2*delta_lambda)/delta_lambda;
        r = min_color; g = max_color; b = min_color + coeff*(max_color-min_color);
    }

    /* #4 interval */
    if(lambda >= options.lambda_min + 3*delta_lambda && lambda < options.lambda_min + 4*delta_lambda){
        coeff = (lambda-options.lambda_min - 3*delta_lambda)/delta_lambda;
       r = max_color - coeff*(max_color-min_color); g = max_color; b = min_color;
    }

    /* #5 interval */
    if(lambda >= options.lambda_min + 4*delta_lambda && lambda < options.lambda_min + 5*delta_lambda){
        coeff = (lambda - options.lambda_min - 4*delta_lambda)/delta_lambda;
        r = max_color; g = min_color + coeff*(max_color-min_color); b = min_color;
    }

    /* #6 interval */
    if(lambda >= options.lambda_min + 5*delta_lambda && lambda <= options.lambda_max){
        coeff = (lambda-options.lambda_min - 5*delta_lambda)/delta_lambda;
        r = max_color; g = min_color; b = max_color - coeff*(max_color-min_color);
    }

    retval = QColor((int)r,(int)g,(int)b,alpha);

    return retval;
}

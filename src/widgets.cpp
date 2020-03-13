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

    label  = new QLabel("<b>"+str_label+":</b>");
    main_layout->addWidget(label);
    main_layout->addLayout(form_layout);

    spinbox_distance = new QDoubleSpinBox();
    spinbox_distance->setSuffix(" m");

    spinbox_phase = new QDoubleSpinBox();
    spinbox_phase->setSuffix(" ms");

    spinbox_period = new QDoubleSpinBox();
    spinbox_period->setSuffix(" ms");

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
}



/*  ============================================ InteractivePlot - general plot for others plots ============================================ */
InteractivePlot::InteractivePlot(QWidget *parent) : QCustomPlot(parent){
    this->setInteractions(QCP::iRangeZoom |
                          QCP::iRangeDrag |
                          QCP::iMultiSelect |
                          QCP::iSelectLegend |
                          QCP::iSelectPlottables |
                          QCP::iSelectAxes);
    this->xAxis2->setVisible(true);
    this->yAxis2->setVisible(true);

    connect(this->xAxis, SIGNAL(rangeChanged(QCPRange)),this->xAxis2, SLOT(setRange(QCPRange)));
    connect(this->yAxis, SIGNAL(rangeChanged(QCPRange)), this->yAxis2, SLOT(setRange(QCPRange)));

    connect(this,SIGNAL(axisClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),this,SLOT(slot_sAxies_drag_zoom(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)));
    connect(this,SIGNAL(mouseDoubleClick(QMouseEvent*)),this,SLOT(slot_full_drag_zoom(QMouseEvent*)));
    connect(this,SIGNAL(selectionChangedByUser()),this,SLOT(slot_selectionChanged()));

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_contextMenuReq(QPoint)));

    // for points
    QLocale locale("en_EN.UTF-8");
    this->setLocale(locale);
    // сделать стили оформления + шрифты (возможно загрузка из конф. файла)

}

void InteractivePlot::slot_sAxies_drag_zoom(QCPAxis* sAxis,QCPAxis::SelectablePart part,QMouseEvent* event){
    this->axisRect()->setRangeDrag(sAxis->orientation());
    this->axisRect()->setRangeZoom(sAxis->orientation());
    return;
}

void InteractivePlot::slot_full_drag_zoom(QMouseEvent *mouseEvent){
    this->axisRect()->setRangeDrag(this->xAxis->orientation()|
                                   this->yAxis->orientation());
    this->axisRect()->setRangeZoom(this->xAxis->orientation()|
                                   this->yAxis->orientation());
    return;
}

void InteractivePlot::slot_selectionChanged(){
    if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
          this->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
      {
        this->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        this->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      }

      // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
      if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
          this->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
      {
        this->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        this->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      }

      // synchronize selection of graphs with selection of corresponding legend items:
      for (int i=0; i<this->graphCount(); ++i)
      {
        QCPGraph *graph = this->graph(i);
        QCPPlottableLegendItem *item = this->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected())
        {
          item->setSelected(true);
          graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
      }
    return;
}

void InteractivePlot::slot_contextMenuReq(QPoint p){
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QMenu *menu_export = new QMenu("Export");
    menu_export->setAttribute(Qt::WA_DeleteOnClose);
    menu->addMenu(menu_export);

    menu_export->addAction("bmp",this,SLOT(exportToBMP()));
    menu_export->addAction("jpg",this,SLOT(exportToJPG()));
    menu_export->addAction("pdf",this,SLOT(exportToPDF()));
    menu_export->addAction("png",this,SLOT(exportToPNG()));

    if(!x_log){
        menu->addAction("x log scale",this,SLOT(setXLog()));
    }else{
        menu->addAction("x linear scale",this,SLOT(setXLog()));
    }

    if(!y_log){
        menu->addAction("y log scale",this,SLOT(setYLog()));
    }else{
        menu->addAction("y linear scale",this,SLOT(setYLog()));
    }

    menu->popup(this->mapToGlobal(p));
}

void InteractivePlot::exportToBMP(){
    auto filename = QFileDialog::getSaveFileName(nullptr,"Save",".bmp","(*.bmp *.BMP)");
    if(filename!=nullptr) this->saveBmp(filename);
}

void InteractivePlot::exportToJPG(){
    auto filename = QFileDialog::getSaveFileName(nullptr,"Save",".jpg","(*.jpg *.JPG)");
    if(filename!=nullptr) this->saveJpg(filename);
}

void InteractivePlot::exportToPDF(){
    auto filename = QFileDialog::getSaveFileName(nullptr,"Save",".pdf","(*.pdf *.PDF)");
    if(filename!=nullptr) this->savePdf(filename);
}

void InteractivePlot::exportToPNG(){
    auto filename = QFileDialog::getSaveFileName(nullptr,"Save",".png","(*.png *.PNG)");
    if(filename!=nullptr) this->savePng(filename);
}

void InteractivePlot::setYLog(){
    y_log = !y_log;
    if(y_log){
        this->yAxis->setScaleType(QCPAxis::stLogarithmic);
        this->yAxis2->setScaleType(QCPAxis::stLogarithmic);
    }else{
        this->yAxis->setScaleType(QCPAxis::stLinear);
        this->yAxis2->setScaleType(QCPAxis::stLinear);
    }
    this->replot();
}

void InteractivePlot::setXLog(){
    x_log = !x_log;
    if(x_log){
        this->xAxis->setScaleType(QCPAxis::stLogarithmic);
        this->xAxis2->setScaleType(QCPAxis::stLogarithmic);
    }else{
        this->xAxis->setScaleType(QCPAxis::stLinear);
        this->xAxis2->setScaleType(QCPAxis::stLinear);
    }
    this->replot();
}

InteractivePlot::~InteractivePlot(){

}


/* Central Widget */
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent){
    auto main_layout = new QHBoxLayout(this);
    auto right_widget = new QWidget();
    auto right_layout = new QVBoxLayout(right_widget);
    auto sample_position_layout = new QFormLayout();

    auto splitter = new QSplitter(Qt::Horizontal);

    button_calculate = new QPushButton("Calculate");

    spinbox_detector_position = new QDoubleSpinBox();
    spinbox_detector_position->setSuffix(" m");

    spinbox_sample_position = new QDoubleSpinBox();
    spinbox_sample_position->setSuffix(" m");
    sample_position_layout->addRow("Detector position: ",spinbox_detector_position);
    sample_position_layout->addRow("Sample position: ",spinbox_sample_position);

    main_layout->setMargin(0);
    main_layout->addWidget(splitter);

    // default values for rnage
    time_range.max = 500.0;
    time_range.min = 0;
    distance_range.max = 40.0;
    distance_range.min = 0;

    for(int i=0;i<4;i++) text_chooper[i] = nullptr;

    plot = new InteractivePlot();
    plot->xAxis->setLabel("time, ms");
    plot->yAxis->setLabel("Distance, m");
    PlotRescaleAxis();

    chooper_widget1 = new ChooperWidget("Chooper #1");
    chooper_widget2 = new ChooperWidget("Chooper #2");
    chooper_widget3 = new ChooperWidget("Chooper #3");
    chooper_widget4 = new ChooperWidget("Chooper #4");

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
    spinbox_sample_position->setValue(25.0);
    spinbox_detector_position->setValue(30.0);

    chooper_widget1->spinbox_distance->setValue(5);
    chooper_widget1->spinbox_period->setValue(50);
    chooper_widget1->spinbox_phase->setValue(10);
    chooper_widget1->spinbox_duty->setValue(10);

    chooper_widget2->spinbox_distance->setValue(4);
    chooper_widget2->spinbox_period->setValue(20);
    chooper_widget2->spinbox_phase->setValue(10);
    chooper_widget2->spinbox_duty->setValue(50);

    chooper_widget3->spinbox_distance->setValue(2);
    chooper_widget3->spinbox_period->setValue(30);
    chooper_widget3->spinbox_phase->setValue(10);
    chooper_widget3->spinbox_duty->setValue(30);

    chooper_widget4->spinbox_distance->setValue(1);
    chooper_widget4->spinbox_period->setValue(30);
    chooper_widget4->spinbox_phase->setValue(15);
    chooper_widget4->spinbox_duty->setValue(30);

    rd = new ResultDialog();
    connect(this,SIGNAL(sendPercentLiveNeutrons(double)),rd,SLOT(showPercentNeutron(double)));
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

    //int N_lines = (plot->xAxis->range().upper - plot->xAxis->range().lower)/period;
    int N_lines = (time_range.max - time_range.min)/period;
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

s_windows CentralWidget::getWindowsFromChooper(double distance, double phase, double period, double duty){
    s_windows windows;
    s_window window;
    int N_lines = (time_range.max - time_range.min)/period;

    windows.distance = distance;

    for(int i=0;i<N_lines;i++){
        window.max = phase+i*period;
        window.min = phase+i*period - period*duty/100;
        windows.windows.append(window);
    }

    return windows;
}


void CentralWidget::ReleaseCalculatrion(){
    plot->clearGraphs();
    int all_neutrons = 0,live_neutrons = 0;
    double neutron_percent;

    int time_count = 0;
    int lambda_count = 0;

    double time,lambda;

    double time_min, time_max, time_step;
    double lambda_min,lambda_max,lambda_step;
    lambda_min = 1;
    lambda_max = 12;
    lambda_step = 0.5;

    time_min = time_range.min;
    time_max = time_range.max;
    time_step = 1;



    s_windows wins1 = getWindowsFromChooper(chooper_widget1->spinbox_distance->value(),
                                           chooper_widget1->spinbox_phase->value(),
                                           chooper_widget1->spinbox_period->value(),
                                           chooper_widget1->spinbox_duty->value());
    s_windows wins2 = getWindowsFromChooper(chooper_widget2->spinbox_distance->value(),
                                           chooper_widget2->spinbox_phase->value(),
                                           chooper_widget2->spinbox_period->value(),
                                           chooper_widget2->spinbox_duty->value());
    s_windows wins3 = getWindowsFromChooper(chooper_widget3->spinbox_distance->value(),
                                           chooper_widget3->spinbox_phase->value(),
                                           chooper_widget3->spinbox_period->value(),
                                           chooper_widget3->spinbox_duty->value());
    s_windows wins4 = getWindowsFromChooper(chooper_widget4->spinbox_distance->value(),
                                           chooper_widget4->spinbox_phase->value(),
                                           chooper_widget4->spinbox_period->value(),
                                           chooper_widget4->spinbox_duty->value());


    time = time_min;
    while(time <= time_max){
        lambda = lambda_min;
        lambda_count = 0;
        while(lambda <= lambda_max){
            Neutron neutron = time;
            neutron.setWavelength(lambda);
            neutron.setSampleDistance(spinbox_sample_position->value());

            neutron.addChopper(wins4);
            neutron.addChopper(wins3);
            neutron.addChopper(wins2);
            neutron.addChopper(wins1);

            neutron.trace(500,100);

            if(neutron.isLive()){
                plot->addGraph();
                plot->graph()->setName(QString::number(lambda));
                plot->graph()->setData(neutron.v_time,neutron.v_distance);
                plot->replot();

                live_neutrons ++;
            }

            lambda += lambda_step;
            lambda_count ++;

            all_neutrons ++;
        }

        time_count++;
        time += time_step;
    }

    neutron_percent = 100.0*live_neutrons/(double)all_neutrons;
    emit sendPercentLiveNeutrons(neutron_percent);
    rd->show();

}

/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#ifndef CALCULATE_H
#define CALCULATE_H

#include <math.h>
#include <QVector>
#include <QThread>


#define C_PLANK 1.054571817e-34
#define C_MASS_NEUTRON 1.67492749804e-27

// For MSVS2017
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct s_window{
    double min,max;
};

class windows{
private:
    double ds;
    double time_range_max, time_range_min;
public:
    void setWindows(double distance, double phase, double period, double duty){
        s_window window;
        int N_lines = (time_range_max - time_range_min)/period;
        ds = distance;
        for(int i=0;i<N_lines;i++){
            window.max = phase+i*period;
            window.min = phase+i*period - period*duty/100;
            wins.append(window);
        }
    }

    QVector<s_window> wins;
    void setDistance(double distance){
        ds = distance;
    }
    double getDistance(void){return ds;}
    void setTimeRange(double lower_time, double upper_time){
        time_range_max = upper_time;
        time_range_min = lower_time;
    }

};

class Neutron{
public:
    Neutron(double st = 0);            //start_time

    void setWavelength(double wavelenght){lambda = wavelenght;}
    double getWavelenght(void){return lambda;}
    void setStartTime(double st){start_time = st;}

    bool isLive(void){return live;}
    void kill(void){live = false;}

    // time at ms
    double getDistance(double time);
    double getTime(double distance);
    void setSampleDistance(double distance){
        sample_time = this->getTime(distance);
    }
    double getSampleTime(void){return sample_time;}
    double getStartTime(void){return start_time;}
    void trace(double end_time=500.0,int points=5);

    void addChopper(windows *chopper){
        choppers.append(chopper);
    }

    QVector<double> v_time;
    QVector<double> v_distance;

private:
    double lambda;
    bool live = false;
    double start_time;
    double sample_time = 0;

    QVector<windows *> choppers;

};


class CalculateThread : public QThread
{
    Q_OBJECT

    void run() override;

private:
    double v_time = 500.0;
    int trace_point = 2;

    int neutron_from, neutron_to;
    QVector<Neutron *> *ns;

public slots:
    void setVisibleTimeAndPoint(double time,int point){
        v_time = time;
        trace_point = point;
    }
    void setNeutrons(QVector<Neutron *> *neutrons,int from, int to){
        ns = neutrons;
        neutron_from = from;
        neutron_to = to;
    }

signals:
    void PlotNeutron(Neutron *);
    void end(CalculateThread *ct);

};

#endif // CALCULATE_H

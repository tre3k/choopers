/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#ifndef CALCULATE_H
#define CALCULATE_H

#include <math.h>
#include <QVector>

#include <QDebug>

#define C_PLANK 1.054571817e-34
#define C_MASS_NEUTRON 1.67492749804e-27

struct s_window{
    double min,max;
};

struct s_windows{
    double distance;
    QVector<s_window> windows;
};

class Neutron{
public:
    Neutron(double st = 0);            //start_time

    void setWavelength(double wavelenght){lambda = wavelenght;}
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
    void trace(double end_time=500.0,double step=1.0);

    void addChopper(s_windows chopper){
        choppers.append(chopper);
    }

    QVector<double> v_time;
    QVector<double> v_distance;

private:
    double lambda;
    bool live = true;
    double start_time;
    double sample_time = 0;

    QVector<s_windows> choppers;


};

#endif // CALCULATE_H

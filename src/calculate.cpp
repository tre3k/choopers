/*
 * Under license GNU GLP (c) 2020
 * Autor: Kirill Pshenichnyi
 *
 */

#include "calculate.h"

Neutron::Neutron(double st){
    choppers.clear();

    setStartTime(st);
}

double Neutron::getDistance(double time){
    double L,vel;
    vel = 2*M_PI*C_PLANK/lambda/1e-10/C_MASS_NEUTRON;
    L = vel*time*1e-3 - vel*start_time*1e-3;
    return L;
}

double Neutron::getTime(double distance){
    double T,vel;
    vel = 2*M_PI*C_PLANK/lambda/1e-10/C_MASS_NEUTRON;
    T = start_time + 1e+3*distance/vel;
    return T;
}

void Neutron::trace(double end_time,double step){
    double chopper_distance,time_value_at_chopper;
    double t_min,t_max;

    for(int i=0;i<choppers.size();i++){
        live = false;
        chopper_distance = choppers.at(i).distance;
        time_value_at_chopper = getTime(chopper_distance);
        //qDebug() << "Distance: " << chopper_distance;
        for(int j=0;j<choppers.at(i).windows.size();j++){
            t_min = choppers.at(i).windows.at(j).min;
            t_max = choppers.at(i).windows.at(j).max;
            //qDebug() << t_min << " " << t_max << " time value:" << time_value_at_chopper;

            if(time_value_at_chopper >= t_min && time_value_at_chopper <= t_max) live = true;
        }
        if(!isLive()) return;
    }


    v_time.clear(); v_distance.clear();
    for(double time = start_time; time <= end_time; time+=step){
        v_time.append(time);
        v_distance.append(getDistance(time));
    }
}

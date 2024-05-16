//
// Created by toor on 5/16/24.
//

#include "pid.h"

PID::PID(double Kp, double Kd, double Ki) :
        _Kp(Kp),
        _Kd(Kd),
        _Ki(Ki),
        _pre_error(0),
        _integral(0) {}

double PID::calculate(double setpoint, double pv, double deltaTime, double max, double min) {
    double error = setpoint - pv;
    double Pout = _Kp * error;
    _integral += error * deltaTime;
    double Iout = _Ki * _integral;
    double derivative = (error - _pre_error) / deltaTime;
    double Dout = _Kd * derivative;
    double output = Pout + Iout + Dout;
    if (output > max)
        output = max;
    else if (output < min)
        output = min;
    _pre_error = error;
    return output;
}

void PID::Set(double p, double i, double d) {
    _Kp = p;
    _Ki = i;
    _Kd = d;
}

PID::~PID() {}

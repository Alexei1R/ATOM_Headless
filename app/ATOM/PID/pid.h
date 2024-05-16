//
// Created by toor on 5/16/24.
//

#ifndef PID_H
#define PID_H

class PID {
public:
    PID(double Kp, double Kd, double Ki);
    double calculate(double setpoint, double pv, double deltaTime, double max, double min);
    void Set(double p, double i, double d);
    ~PID();

private:
    double _Kp;
    double _Kd;
    double _Ki;
    double _pre_error;
    double _integral;
};

#endif // PID_H

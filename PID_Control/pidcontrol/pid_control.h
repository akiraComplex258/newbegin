#ifndef __PID_CONTROL_H
#define __PID_CONTROL_H

#include "main.h"

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float pre_error;
    float integral;
    float integral_limit;
    float error_limit;
} PID_Controller; //pid结构体的定义

void PID_Init_speed(PID_Controller *pid, float Kp, float Ki, float Kd);//pid初始化函数,输入参数为pid结构体指针和pid参数
void PID_Init_torque(PID_Controller *pid, float Kp, float Ki, float Kd);
float PID_Compute_speed(PID_Controller *pid, float setpoint, float measured_value,float integral_limit,float error_limit);//pid计算函数,输入参数为pid结构体指针、设定值和测量值
float PID_Compute_torque(PID_Controller *pid, float setpoint, float measured_value,float integral_limit,float error_limit);

#endif /* __PID_CONTROL_H */
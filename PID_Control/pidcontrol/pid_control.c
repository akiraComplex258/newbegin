#include "pid_control.h"

void PID_Init_speed(PID_Controller *pid, float Kp, float Ki, float Kd)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->pre_error = 0;
    pid->integral = 0;
}//速度环初始化
void PID_Init_torque(PID_Controller *pid, float Kp, float Ki, float Kd)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->pre_error = 0;
    pid->integral = 0;
} //电流环初始化
//速度环
float PID_Compute_speed(PID_Controller *pid, float setpoint, float measured_value,float integral_limit,float error_limit)
{
    float error = setpoint - measured_value;//计算误差

    pid->integral += error;//积分项
    if (error > error_limit) {
        pid->integral = 0;
    } else if (error < -error_limit) {
        pid->integral = 0;
    }//积分分离
    if (pid->integral > integral_limit) {
        pid->integral = integral_limit;
    } else if (pid->integral < -integral_limit) {
        pid->integral = -integral_limit;
    }//积分限幅
    float derivative = error - pid->pre_error;//微分项
    pid->pre_error = error;//更新前一个误差
    return pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;//计算PID输出
}//pid计算函数,输入参数为pid结构体指针、设定值和测量值,返回pid输出
//电流环
float PID_Compute_torque(PID_Controller *pid, float setpoint, float measured_value,float integral_limit,float error_limit)
{
    float error = setpoint - measured_value;//计算误差

    pid->integral += error;//积分项
    if (error > error_limit) {
        pid->integral = 0;
    } else if (error < -error_limit) {
        pid->integral = 0;
    }//积分分离
    if (pid->integral > integral_limit) {
        pid->integral = integral_limit;
    } else if (pid->integral < -integral_limit) {
        pid->integral = -integral_limit;
    }//积分限幅
    float derivative = error - pid->pre_error;//微分项
    pid->pre_error = error;//更新前一个误差
    return pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;//计算PID输出
}//pid计算函数,输入参数为pid结构体指针、设定值和测量值,返回pid输出

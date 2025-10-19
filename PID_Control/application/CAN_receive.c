#include "CAN_receive.h"
#include "main.h"
#include "stm32f407xx.h"
#include "pid_control.h"
#include "stm32f4xx_hal_uart.h"
#include <stdint.h>



extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;//
extern UART_HandleTypeDef huart6;//
extern DMA_HandleTypeDef hdma_usart6_tx;
//motor data read
#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
    }// 读取电机数据宏定义, 输入参数为电机数据结构体指针和CAN接收数据数组
/*
电机数据, 0:底盘电机1 3508;1:底盘电机2 3508;2:底盘电机3 3508;3:底盘电机4 3508;
4:yaw云台电机 6020;5:pitch云台电机 6020;6:触发电机 2006;*/
static motor_measure_t motor_chassis[7];// 底盘电机数据结构体数组
static motor_serialplot_tx_t motor_serialplot_tx[4];// 电机serialplot发送数据结构体数组

static CAN_TxHeaderTypeDef  gimbal_tx_message;
static uint8_t              gimbal_can_send_data[8];// 云台CAN发送数据数组
static CAN_TxHeaderTypeDef  chassis_tx_message;
static uint8_t              chassis_can_send_data[8];// 底盘CAN发送数据数组
int8_t              UART_TX_DATA[8]={0,0,0,0,0,0,0,0};// serialplot发送数据数组

/**
  * @brief          hal CAN fifo call back, receive motor data
  * @param[in]      hcan, the point to CAN handle
  * @retval         none
  */
/**
  * @brief          hal��CAN�ص�����,���յ������
  * @param[in]      hcan:CAN���ָ��
  * @retval         none
  */




/**
  * @brief          send control current of motor (0x205, 0x206, 0x207, 0x208)
  * @param[in]      yaw: (0x205) 6020 motor control current, range [-30000,30000] 
  * @param[in]      pitch: (0x206) 6020 motor control current, range [-30000,30000]
  * @param[in]      shoot: (0x207) 2006 motor control current, range [-10000,10000]
  * @param[in]      rev: (0x208) reserve motor control current
  * @retval         none
  */
/**
  * @brief          ���͵�����Ƶ���(0x205,0x206,0x207,0x208)
  * @param[in]      yaw: (0x205) 6020������Ƶ���, ��Χ [-30000,30000]
  * @param[in]      pitch: (0x206) 6020������Ƶ���, ��Χ [-30000,30000]
  * @param[in]      shoot: (0x207) 2006������Ƶ���, ��Χ [-10000,10000]
  * @param[in]      rev: (0x208) ������������Ƶ���
  * @retval         none
  */
void CAN_cmd_gimbal(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev)
{
    uint32_t send_mail_box;
    gimbal_tx_message.StdId = CAN_GIMBAL_ALL_ID;
    gimbal_tx_message.IDE = CAN_ID_STD;
    gimbal_tx_message.RTR = CAN_RTR_DATA;
    gimbal_tx_message.DLC = 0x08;
    // 云台CAN发送数据
    gimbal_can_send_data[0] = (yaw >> 8);
    gimbal_can_send_data[1] = yaw;
    gimbal_can_send_data[2] = (pitch >> 8);
    gimbal_can_send_data[3] = pitch;
    gimbal_can_send_data[4] = (shoot >> 8);
    gimbal_can_send_data[5] = shoot;
    gimbal_can_send_data[6] = (rev >> 8);
    gimbal_can_send_data[7] = rev;
    HAL_CAN_AddTxMessage(&GIMBAL_CAN, &gimbal_tx_message, gimbal_can_send_data, &send_mail_box);
    // 发送CAN报文
    //M3508电机提供的是相对角度（上电清零，每次上电后以初始位置为0点）。
    //GM6020电机提供的是绝对角度（即使掉电再上电，也能知道当前绝对位置，因为编码器是绝对式的）。
}

/**
  * @brief          send CAN packet of ID 0x700, it will set chassis motor 3508 to quick ID setting
  * @param[in]      none
  * @retval         none
  */
/**
  * @brief          ����IDΪ0x700��CAN��,��������3508��������������ID
  * @param[in]      none
  * @retval         none
  */
void CAN_cmd_chassis_reset_ID(void)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x700;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = 0;
    chassis_can_send_data[1] = 0;
    chassis_can_send_data[2] = 0;
    chassis_can_send_data[3] = 0;
    chassis_can_send_data[4] = 0;
    chassis_can_send_data[5] = 0;
    chassis_can_send_data[6] = 0;
    chassis_can_send_data[7] = 0;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}


/**
  * @brief          send control current of motor (0x201, 0x202, 0x203, 0x204)
  * @param[in]      motor1: (0x201) 3508 motor control current, range [-16384,16384] 
  * @param[in]      motor2: (0x202) 3508 motor control current, range [-16384,16384] 
  * @param[in]      motor3: (0x203) 3508 motor control current, range [-16384,16384] 
  * @param[in]      motor4: (0x204) 3508 motor control current, range [-16384,16384] 
  * @retval         none
  */
/**
  * @brief          ���͵�����Ƶ���(0x201,0x202,0x203,0x204)
  * @param[in]      motor1: (0x201) 3508������Ƶ���, ��Χ [-16384,16384]
  * @param[in]      motor2: (0x202) 3508������Ƶ���, ��Χ [-16384,16384]
  * @param[in]      motor3: (0x203) 3508������Ƶ���, ��Χ [-16384,16384]
  * @param[in]      motor4: (0x204) 3508������Ƶ���, ��Χ [-16384,16384]
  * @retval         none
  */
void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = CAN_CHASSIS_ALL_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8;
    chassis_can_send_data[1] = motor1;
    chassis_can_send_data[2] = motor2 >> 8;
    chassis_can_send_data[3] = motor2;
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

/**
  * @brief          return the yaw 6020 motor data point
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          ����yaw 6020�������ָ��
  * @param[in]      none
  * @retval         �������ָ��
  */
const motor_measure_t *get_yaw_gimbal_motor_measure_point(void)
{
    return &motor_chassis[4];
}

/**
  * @brief          return the pitch 6020 motor data point
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          ����pitch 6020�������ָ��
  * @param[in]      none
  * @retval         �������ָ��
  */
const motor_measure_t *get_pitch_gimbal_motor_measure_point(void)
{
    return &motor_chassis[5];
}


/**
  * @brief          return the trigger 2006 motor data point
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          ���ز������ 2006�������ָ��
  * @param[in]      none
  * @retval         �������ָ��
  */
const motor_measure_t *get_trigger_motor_measure_point(void)
{
    return &motor_chassis[6];
}


/**
  * @brief          return the chassis 3508 motor data point
  * @param[in]      i: motor number,range [0,3]
  * @retval         motor data point
  */
/**
  * @brief          ���ص��̵�� 3508�������ָ��
  * @param[in]      i: ������,��Χ[0,3]
  * @retval         �������ָ��
  */
const motor_measure_t *get_chassis_motor_measure_point(uint8_t i)
{
    return &motor_chassis[(i & 0x03)];
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

    switch (rx_header.StdId)
    {
        case CAN_3508_M1_ID:
        case CAN_3508_M2_ID:
        case CAN_3508_M3_ID:
        case CAN_3508_M4_ID:
        case CAN_YAW_MOTOR_ID:
        case CAN_PIT_MOTOR_ID:
        case CAN_TRIGGER_MOTOR_ID:
        {
            static uint8_t i = 0;
            //get motor id
            i = rx_header.StdId - CAN_3508_M1_ID;
            get_motor_measure(&motor_chassis[i], rx_data);
            break;
        }

        default:
        {
            break;
        }
    }
    switch (rx_header.StdId)
    {
        case CAN_3508_M1_ID:
        case CAN_3508_M2_ID:
        case CAN_3508_M3_ID:
        case CAN_3508_M4_ID:
        {
            static uint8_t j = 0;
            //get motor id
            j = rx_header.StdId - CAN_3508_M1_ID;
            motor_serialplot_tx[j].encoder = motor_chassis[j].ecd;
            motor_serialplot_tx[j].omega = motor_chassis[j].speed_rpm;
            motor_serialplot_tx[j].torque = motor_chassis[j].given_current;
            motor_serialplot_tx[j].temperature = motor_chassis[j].temperate;
        }
      }
      UART_TX_DATA[0] = 0xAA; // serialplot帧头
			UART_TX_DATA[1] = 0xBB; // serialplot帧头
			
    
      UART_TX_DATA[2 + 2 * 0 + 0] = (int8_t)(motor_serialplot_tx[0].encoder & 0xFF);
      UART_TX_DATA[2 + 2 * 0 + 1] = (int8_t)((motor_serialplot_tx[0].encoder >> 8) & 0xFF);

      UART_TX_DATA[2 + 2 * 1 + 0] = (int8_t)(motor_serialplot_tx[0].omega & 0xFF);
      UART_TX_DATA[2 + 2 * 1 + 1] = (int8_t)((motor_serialplot_tx[0].omega >> 8) & 0xFF);

      UART_TX_DATA[2 + 2 * 2 + 0] = (int8_t)(motor_serialplot_tx[0].torque & 0xFF);
      UART_TX_DATA[2 + 2 * 2 + 1] = (int8_t)((motor_serialplot_tx[0].torque >> 8) & 0xFF);     
           
//得到的UART_TX_DATA数组后续通过UART DMA发送传输给serialplot软件进行数据显示
    
// static PID_Controller pid1;
// PID_Init_speed(&pid1, 1.0f, 0.1f, 0.05f, 1000.0f, 50.0f);// 初始化PID控制器, 设置Kp, Ki, Kd, 积分限幅和误差限幅
// const motor_measure_t *motor = get_chassis_motor_measure_point(0);
// uint16_t measured_value = motor->speed_rpm; // 获取实际速度
// uint16_t output = PID_Compute_speed(&pid1, 100, (float)measured_value, 1000.0f, 50.0f); // 100为目标速度
//     }
    }
    int8_t* get_UART_TX_DATA_ptr(void)
{
    return UART_TX_DATA;
}

uint16_t get_UART_TX_DATA_size(void)
{
    return (uint16_t)sizeof(UART_TX_DATA);
}

#ifndef __UART_DMA_H__
#define __UART_DMA_H__

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_tx;

#define FRAME_HEADER_0 0xAA
#define FRAME_HEADER_1 0x55
#define MOTOR_DATA_CHANNELS 3 // 3个通道
//电机数据结构体
typedef struct {
    uint8_t header[2];
    float data[MOTOR_DATA_CHANNELS];
} motor_data_frame_t;
void UART_DMA_Init(void);
void send_motor_data_dma(float speed, float position, float current);
#endif // __UART_DMA_H__
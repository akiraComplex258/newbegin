#include "serialplot.h"
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart6;

volatile uint8_t serialplot_dma_busy = 0;

// void UART_DMA_Init(void) {
//     // 如需开启 UART 空闲中断之类的在这里做；TX 使用 DMA 不需要额外配置
// }

 void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
     if (huart->Instance == USART6) {
         serialplot_dma_busy = 0; // DMA 发送完成，清忙标志
     }
 }
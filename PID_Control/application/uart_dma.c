// #include "uart_dma.h"
// #include "stm32f4xx_hal.h"

// extern UART_HandleTypeDef huart6;
// extern DMA_HandleTypeDef hdma_usart6_tx;
// motor_data_frame_t tx_frame;
// void UART_DMA_Init(void) {
//     __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE); // 使能空闲中断
// }      
// void send_motor_data_dma(float speed, float position, float current){
//     tx_frame.header[0] = FRAME_HEADER_0;
//     tx_frame.header[1] = FRAME_HEADER_1;
//     tx_frame.data[0] = speed;//0号频道
//     tx_frame.data[1] = position;//1号频道
//     tx_frame.data[2] = current;//2号频道

//     HAL_UART_Transmit_DMA(&huart6, (uint8_t*)&tx_frame, sizeof(tx_frame));
// }
// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
//     if (huart->Instance == USART6) {
//         // 发送完成，可以设置标志位，通知主程序可以发送下一帧
//     }
// }
// void USART6_IRQHandler(void) {
//     HAL_UART_IRQHandler6(&huart6);
//     // 用户代码：检测空闲中断
//     if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_IDLE) != RESET) {
//         __HAL_UART_CLEAR_IDLEFLAG(&huart6); // 清除空闲中断标志
//         // 处理接收到的数据（如果有接收需求）
//         // 这里可以调用接收完成回调函数
//     }
// }
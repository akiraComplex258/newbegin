#ifndef SERIALPLOT_H
#define SERIALPLOT_H

#include "stm32f4xx_hal.h"
#include "CAN_receive.h"

uint8_t* get_UART_TX_DATA_ptr(void);
uint16_t get_UART_TX_DATA_size(void);
extern volatile uint8_t serialplot_dma_busy;

#endif // SERIALPLOT_H
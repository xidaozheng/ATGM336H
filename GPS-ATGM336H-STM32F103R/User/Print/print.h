#ifndef __PRINT_H
#define __PRINT_H

#include "stm32f10x.h"
#include <stdio.h>

#define PRINT_TX_CLK		RCC_APB2Periph_GPIOA
#define PRINT_RX_CLK		RCC_APB2Periph_GPIOA

#define PRINT_TX_PORT		GPIOA
#define PRINT_RX_PORT		GPIOA

#define PRINT_TX_PIN		GPIO_Pin_9
#define PRINT_RX_PIN		GPIO_Pin_10

#define PRINT_TX_MODE		GPIO_Mode_AF_PP
#define PRINT_RX_MODE		GPIO_Mode_IN_FLOATING

#define PRINT_SPEED			GPIO_Speed_50MHz


#define PRINT_USART			USART1
#define PRINT_USART_CLK		RCC_APB2Periph_USART1


extern void printInit(uint32_t baud);

#endif


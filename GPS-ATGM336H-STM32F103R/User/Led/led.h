#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

enum en_ledStatus
{
	enLedOff = 0,
	enLedOn  = 1,
};

#define LED1_CLK		RCC_APB2Periph_GPIOB
#define LED2_CLK		RCC_APB2Periph_GPIOB
#define LED3_CLK		RCC_APB2Periph_GPIOB
#define LED4_CLK		RCC_APB2Periph_GPIOB
	
#define LED1_PORT		GPIOB
#define LED2_PORT		GPIOB
#define LED3_PORT		GPIOB
#define LED4_PORT		GPIOB
	
#define LED1_PIN		GPIO_Pin_6
#define LED2_PIN		GPIO_Pin_7
#define LED3_PIN		GPIO_Pin_8
#define LED4_PIN		GPIO_Pin_9


#define LED_IO_MODE		GPIO_Mode_Out_PP
#define LED_IO_SPEED	GPIO_Speed_10MHz


#define LED1_ON			do{GPIO_SetBits(LED1_PORT, LED1_PIN);} 		while(0)
#define LED1_OFF		do{GPIO_ResetBits(LED1_PORT, LED1_PIN);} 	while(0)

#define LED2_ON			do{GPIO_SetBits(LED2_PORT, LED2_PIN);} 		while(0)
#define LED2_OFF		do{GPIO_ResetBits(LED2_PORT, LED2_PIN);} 	while(0)

#define LED3_ON			do{GPIO_SetBits(LED3_PORT, LED3_PIN);} 		while(0)
#define LED3_OFF		do{GPIO_ResetBits(LED3_PORT, LED3_PIN);} 	while(0)

#define LED4_ON			do{GPIO_SetBits(LED4_PORT, LED4_PIN);} 		while(0)
#define LED4_OFF		do{GPIO_ResetBits(LED4_PORT, LED4_PIN);} 	while(0)


extern void ledInit(void);

#endif

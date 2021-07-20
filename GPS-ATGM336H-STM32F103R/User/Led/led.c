#include "led.h"

/********************************
**	LED1	PB6		High-level **
**	LED2	PB7		High-level **	
**	LED3	PB8		High-level **
**	LED4	PB9		High-level **
*********************************/


/**
  * @brief  LED1234初始化函数
  * @param  无
  * @retval 无
  */
void ledInit(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED1_CLK|LED2_CLK|LED3_CLK|LED4_CLK, ENABLE);
	
	//LED1
	GPIO_InitStructure.GPIO_Mode	= LED_IO_MODE;
	GPIO_InitStructure.GPIO_Pin		= LED1_PIN;
	GPIO_InitStructure.GPIO_Speed	= LED_IO_SPEED;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	
	//LED2
	GPIO_InitStructure.GPIO_Pin		= LED2_PIN;
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);	
	
	//LED3
	GPIO_InitStructure.GPIO_Pin		= LED3_PIN;
	GPIO_Init(LED3_PORT, &GPIO_InitStructure);	
	
	//LED4
	GPIO_InitStructure.GPIO_Pin		= LED4_PIN;
	GPIO_Init(LED4_PORT, &GPIO_InitStructure);

	//Set init staute: Low-level, and led1234 is off
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}




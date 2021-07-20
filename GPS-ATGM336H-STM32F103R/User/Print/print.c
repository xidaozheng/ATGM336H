#include "print.h"


static void GpioInit(void);
static void UsartInit(uint32_t baud);


/**
  * @brief  打印串口的初始化函数
  * @param  无
  * @retval 无
  */
void printInit(uint32_t baud)
{
	UsartInit(baud);	
	GpioInit();			//后 初始化端口，否则第一个字符是乱码
}

/**
  * @brief  打印串口的端口初始化函数
  * @param  无
  * @retval 无
  */
static void GpioInit(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(PRINT_TX_CLK|PRINT_RX_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//必须打开复用时钟，否则复用推挽输出无效
	
	GPIO_InitStructure.GPIO_Mode	= PRINT_TX_MODE;
	GPIO_InitStructure.GPIO_Pin		= PRINT_TX_PIN;
	GPIO_InitStructure.GPIO_Speed	= PRINT_SPEED;
	GPIO_Init(PRINT_TX_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode	= PRINT_RX_MODE;
	GPIO_InitStructure.GPIO_Pin		= PRINT_RX_PIN;
	GPIO_Init(PRINT_RX_PORT, &GPIO_InitStructure);	
}

/**
  * @brief  打印串口的端口初始化函数
  * @param  baud:波特率
  * @retval 无
  */
static void UsartInit(uint32_t baud)
{
	USART_InitTypeDef	USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(PRINT_USART_CLK, ENABLE);
	
	USART_InitStructure.USART_BaudRate				= baud;								//自定义波特率
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	//禁止硬件流控
	USART_InitStructure.USART_Mode					= USART_Mode_Rx|USART_Mode_Tx;		//全双工
	USART_InitStructure.USART_Parity				= USART_Parity_No;					//无校验	
	USART_InitStructure.USART_StopBits				= USART_StopBits_1;					//一个停止位
	USART_InitStructure.USART_WordLength			= USART_WordLength_8b;				//8位
	USART_Init(PRINT_USART, &USART_InitStructure);
	
	USART_Cmd(PRINT_USART, ENABLE);														//打开串口的时钟入口
}

int fputc(int c, FILE * stream)
{
	USART_SendData(PRINT_USART, (uint8_t)c);
	while(USART_GetFlagStatus(PRINT_USART, USART_FLAG_TXE) == RESET);
	
	return c;
}


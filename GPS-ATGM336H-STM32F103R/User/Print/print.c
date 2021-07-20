#include "print.h"


static void GpioInit(void);
static void UsartInit(uint32_t baud);


/**
  * @brief  ��ӡ���ڵĳ�ʼ������
  * @param  ��
  * @retval ��
  */
void printInit(uint32_t baud)
{
	UsartInit(baud);	
	GpioInit();			//�� ��ʼ���˿ڣ������һ���ַ�������
}

/**
  * @brief  ��ӡ���ڵĶ˿ڳ�ʼ������
  * @param  ��
  * @retval ��
  */
static void GpioInit(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(PRINT_TX_CLK|PRINT_RX_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//����򿪸���ʱ�ӣ����������������Ч
	
	GPIO_InitStructure.GPIO_Mode	= PRINT_TX_MODE;
	GPIO_InitStructure.GPIO_Pin		= PRINT_TX_PIN;
	GPIO_InitStructure.GPIO_Speed	= PRINT_SPEED;
	GPIO_Init(PRINT_TX_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode	= PRINT_RX_MODE;
	GPIO_InitStructure.GPIO_Pin		= PRINT_RX_PIN;
	GPIO_Init(PRINT_RX_PORT, &GPIO_InitStructure);	
}

/**
  * @brief  ��ӡ���ڵĶ˿ڳ�ʼ������
  * @param  baud:������
  * @retval ��
  */
static void UsartInit(uint32_t baud)
{
	USART_InitTypeDef	USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(PRINT_USART_CLK, ENABLE);
	
	USART_InitStructure.USART_BaudRate				= baud;								//�Զ��岨����
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	//��ֹӲ������
	USART_InitStructure.USART_Mode					= USART_Mode_Rx|USART_Mode_Tx;		//ȫ˫��
	USART_InitStructure.USART_Parity				= USART_Parity_No;					//��У��	
	USART_InitStructure.USART_StopBits				= USART_StopBits_1;					//һ��ֹͣλ
	USART_InitStructure.USART_WordLength			= USART_WordLength_8b;				//8λ
	USART_Init(PRINT_USART, &USART_InitStructure);
	
	USART_Cmd(PRINT_USART, ENABLE);														//�򿪴��ڵ�ʱ�����
}

int fputc(int c, FILE * stream)
{
	USART_SendData(PRINT_USART, (uint8_t)c);
	while(USART_GetFlagStatus(PRINT_USART, USART_FLAG_TXE) == RESET);
	
	return c;
}


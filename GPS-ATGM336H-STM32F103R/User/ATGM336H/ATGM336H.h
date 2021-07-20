#ifndef __ATGM336H_H
#define __ATGM336H_H

#include "stm32f10x.h"


#define ATGM336H_USART_TX_CLK		RCC_APB2Periph_GPIOC
#define ATGM336H_USART_RX_CLK		RCC_APB2Periph_GPIOC
#define ATGM336H_USART_CLK			RCC_APB1Periph_UART4

#define ATGM336H_USART				UART4

#define ATGM336H_USART_TX_PORT		GPIOC	
#define ATGM336H_USART_RX_PORT		GPIOC

#define ATGM336H_USART_TX_PIN		GPIO_Pin_10
#define ATGM336H_USART_RX_PIN		GPIO_Pin_11

#define ATGM336H_USART_TX_MODE		GPIO_Mode_AF_PP
#define ATGM336H_USART_RX_MODE		GPIO_Mode_IN_FLOATING

#define ATGM336H_USART_SPEED		GPIO_Speed_50MHz


#define ATGM336_USART_REC_MAX_LEN	200  					//�����������ֽ��� 200

//�������鳤��
#define ATGM336_BUFF_LEN 			80
#define UTC_TIME_LEN 				11
#define LATITUDE_LEN	 			11
#define N_S_LEN 					2
#define LONGITUDE_LEN	 			12
#define E_W_LEN 					2 

typedef struct stcSaveData 
{
	char GPS_Buffer[ATGM336_BUFF_LEN];
	char isGetData;							//�Ƿ��ȡ��GPS����
	char isParseData;						//�Ƿ�������
	char UTCTime[UTC_TIME_LEN];			//UTCʱ��
	char latitude[LATITUDE_LEN];			//γ��
	char N_S[N_S_LEN];					//N/S
	char longitude[LONGITUDE_LEN];		//����
	char E_W[E_W_LEN];					//E/W
	char isUsefull;							//��λ��Ϣ�Ƿ���Ч
}stcSaveData_t, *pstcSaveData_t;



extern void ATGM336H_Init(void);
extern void ATGM336H_clrStruct(void);
extern void ATGM336H_parseGpsBuffer(void);
extern void ATGM336H_printGpsBuffer(void);


#endif

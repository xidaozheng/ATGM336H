#include "ATGM336H.h"
#include <stdio.h>
#include <string.h>


#define false 0
#define true 1


static stcSaveData_t	stcSaveData;
static char 			recBuffer[ATGM336_USART_REC_MAX_LEN];     
static uint16_t 		point1 = 0;

static void clrBuffer(void);
static void recvDataCallback(uint8_t d);


void ATGM336H_Init(void)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(ATGM336H_USART_TX_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(ATGM336H_USART_RX_CLK, ENABLE);	
	RCC_APB1PeriphClockCmd(ATGM336H_USART_CLK, ENABLE);

	//TX
    GPIO_InitStructure.GPIO_Pin 	= ATGM336H_USART_TX_PIN; 
    GPIO_InitStructure.GPIO_Speed 	= ATGM336H_USART_SPEED;
    GPIO_InitStructure.GPIO_Mode 	= ATGM336H_USART_TX_MODE;	
    GPIO_Init(ATGM336H_USART_TX_PORT, &GPIO_InitStructure);
   
	//RX
    GPIO_InitStructure.GPIO_Pin 	= ATGM336H_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode 	= ATGM336H_USART_RX_MODE;
    GPIO_Init(ATGM336H_USART_RX_PORT, &GPIO_InitStructure);  


    NVIC_InitStructure.NVIC_IRQChannel 						= UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 3;			//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);											//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = 9600;								//����������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;				//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;					//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;						//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
																			//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			//�շ�ģʽ

    USART_Init(ATGM336H_USART, &USART_InitStructure); 						//��ʼ������
    USART_ITConfig(ATGM336H_USART, USART_IT_RXNE, ENABLE);					//�����ж�
    USART_Cmd(ATGM336H_USART, ENABLE);                   	 				//ʹ�ܴ��� 


	clrBuffer();																//��ջ���	
}

static void clrBuffer(void)
{
	memset(recBuffer, 0, ATGM336_USART_REC_MAX_LEN);      //���
	point1 = 0;                    	
}

static void recvDataCallback(uint8_t d)
{
	if(d == '$')		//���ݵĿ�ʼ��ʶ
		point1 = 0;		//������0
	
	recBuffer[point1++] = d;
	
	if(recBuffer[0] == '$' && recBuffer[4] == 'M' && recBuffer[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
	{
		if(d == '\n')									   
		{
			memset(stcSaveData.GPS_Buffer, 0, ATGM336_BUFF_LEN);      //���
			memcpy(stcSaveData.GPS_Buffer, recBuffer, point1); 	//��������
			stcSaveData.isGetData = true;
			point1 = 0;
			memset(recBuffer, 0, ATGM336_USART_REC_MAX_LEN);      //���				
		}	
				
	}
	
	if(point1 >= ATGM336_USART_REC_MAX_LEN)
		point1 = ATGM336_USART_REC_MAX_LEN;
}

void UART4_IRQHandler(void)                									//����4�жϷ������
{
	uint8_t recvData;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) 
	{
		recvData = USART_ReceiveData(UART4);								//��ȡ���յ�������
		recvDataCallback(recvData);
   } 
}


void ATGM336H_clrStruct(void)
{
	stcSaveData.isGetData 	= false;
	stcSaveData.isParseData = false;
	stcSaveData.isUsefull 	= false;
	
	memset(stcSaveData.GPS_Buffer, 	0, ATGM336_BUFF_LEN);      //���
	memset(stcSaveData.UTCTime, 	0, UTC_TIME_LEN);
	memset(stcSaveData.latitude, 	0, LATITUDE_LEN);
	memset(stcSaveData.N_S, 		0, N_S_LEN);
	memset(stcSaveData.longitude, 	0, LONGITUDE_LEN);
	memset(stcSaveData.E_W, 		0, E_W_LEN);
}

static void errorLog(int num)
{
	
	while (1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}

void ATGM336H_parseGpsBuffer(void)
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (stcSaveData.isGetData){
		stcSaveData.isGetData = false;
		printf("**************\r\n");
		printf(stcSaveData.GPS_Buffer);
		
		for (i = 0 ; i <= 6 ; i++){
			if (i == 0){
				if ((subString = strstr(stcSaveData.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//��������
			}
			else{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL){
					char usefullBuffer[2]; 
					switch(i){
						case 1:
							memcpy(stcSaveData.UTCTime, subString, subStringNext - subString);
						break;	//��ȡUTCʱ��
						case 2:
							memcpy(usefullBuffer, subString, subStringNext - subString);
						break;	//��ȡUTCʱ��
						case 3:
							memcpy(stcSaveData.latitude, subString, subStringNext - subString);
						break;	//��ȡγ����Ϣ
						case 4:
							memcpy(stcSaveData.N_S, subString, subStringNext - subString);
						break;	//��ȡN/S
						case 5:
							memcpy(stcSaveData.longitude, subString, subStringNext - subString);
						break;	//��ȡ������Ϣ
						case 6:
							memcpy(stcSaveData.E_W, subString, subStringNext - subString);
						break;	//��ȡE/W

						default:break;
					}

					subString = subStringNext;
					stcSaveData.isParseData = true;
					if(usefullBuffer[0] == 'A')
						stcSaveData.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						stcSaveData.isUsefull = false;

				}
				else{
					errorLog(2);	//��������
				}
			}
		}
	}
}

void ATGM336H_printGpsBuffer(void)
{
	if (stcSaveData.isParseData)
	{
		stcSaveData.isParseData = false;
		
		printf("Save_Data.UTCTime = ");			//��λ���UTCʱ��
		printf(stcSaveData.UTCTime);
		printf("\r\n");

		if(stcSaveData.isUsefull)
		{
			stcSaveData.isUsefull = false;
			printf("Save_Data.latitude = ");	//γ��
			printf(stcSaveData.latitude);			
			printf("\r\n");


			printf("Save_Data.N_S = ");			//γ����
			printf(stcSaveData.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");	//����
			printf(stcSaveData.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");			//��/����
			printf(stcSaveData.E_W);
			printf("\r\n");
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}

void ATGM336H_uploadProtocol(void)
{
	
}





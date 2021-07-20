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
    //GPIO端口设置
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 3;			//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);											//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = 9600;								//波特率设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;				//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;					//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;						//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
																			//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			//收发模式

    USART_Init(ATGM336H_USART, &USART_InitStructure); 						//初始化串口
    USART_ITConfig(ATGM336H_USART, USART_IT_RXNE, ENABLE);					//开启中断
    USART_Cmd(ATGM336H_USART, ENABLE);                   	 				//使能串口 


	clrBuffer();																//清空缓存	
}

static void clrBuffer(void)
{
	memset(recBuffer, 0, ATGM336_USART_REC_MAX_LEN);      //清空
	point1 = 0;                    	
}

static void recvDataCallback(uint8_t d)
{
	if(d == '$')		//数据的开始标识
		point1 = 0;		//长度清0
	
	recBuffer[point1++] = d;
	
	if(recBuffer[0] == '$' && recBuffer[4] == 'M' && recBuffer[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
	{
		if(d == '\n')									   
		{
			memset(stcSaveData.GPS_Buffer, 0, ATGM336_BUFF_LEN);      //清空
			memcpy(stcSaveData.GPS_Buffer, recBuffer, point1); 	//保存数据
			stcSaveData.isGetData = true;
			point1 = 0;
			memset(recBuffer, 0, ATGM336_USART_REC_MAX_LEN);      //清空				
		}	
				
	}
	
	if(point1 >= ATGM336_USART_REC_MAX_LEN)
		point1 = ATGM336_USART_REC_MAX_LEN;
}

void UART4_IRQHandler(void)                									//串口4中断服务程序
{
	uint8_t recvData;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) 
	{
		recvData = USART_ReceiveData(UART4);								//读取接收到的数据
		recvDataCallback(recvData);
   } 
}


void ATGM336H_clrStruct(void)
{
	stcSaveData.isGetData 	= false;
	stcSaveData.isParseData = false;
	stcSaveData.isUsefull 	= false;
	
	memset(stcSaveData.GPS_Buffer, 	0, ATGM336_BUFF_LEN);      //清空
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
					errorLog(1);	//解析错误
			}
			else{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL){
					char usefullBuffer[2]; 
					switch(i){
						case 1:
							memcpy(stcSaveData.UTCTime, subString, subStringNext - subString);
						break;	//获取UTC时间
						case 2:
							memcpy(usefullBuffer, subString, subStringNext - subString);
						break;	//获取UTC时间
						case 3:
							memcpy(stcSaveData.latitude, subString, subStringNext - subString);
						break;	//获取纬度信息
						case 4:
							memcpy(stcSaveData.N_S, subString, subStringNext - subString);
						break;	//获取N/S
						case 5:
							memcpy(stcSaveData.longitude, subString, subStringNext - subString);
						break;	//获取经度信息
						case 6:
							memcpy(stcSaveData.E_W, subString, subStringNext - subString);
						break;	//获取E/W

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
					errorLog(2);	//解析错误
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
		
		printf("Save_Data.UTCTime = ");			//定位点的UTC时间
		printf(stcSaveData.UTCTime);
		printf("\r\n");

		if(stcSaveData.isUsefull)
		{
			stcSaveData.isUsefull = false;
			printf("Save_Data.latitude = ");	//纬度
			printf(stcSaveData.latitude);			
			printf("\r\n");


			printf("Save_Data.N_S = ");			//纬度区
			printf(stcSaveData.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");	//经度
			printf(stcSaveData.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");			//东/西区
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





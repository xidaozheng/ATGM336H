#include "led.h"
#include "print.h"
#include "ATGM336H.h"


/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	
    ledInit();
	printInit(115200);
	ATGM336H_Init();			//UART4 波特率9600
	
	ATGM336H_clrStruct();
	
	printf("******************************\n");
	printf("     ==      ==    ======     \n");
	printf("      ==    ==       ==       \n");
	printf("       ==  ==        ==       \n");
	printf("        ====         ==       \n");
	printf("       ==  ==        ==       \n");
	printf("      ==    ==       ==       \n");
	printf("    ==       ==    ======     \n");
	printf("******************************\n");
	
	while(1)
	{
		ATGM336H_parseGpsBuffer();
		ATGM336H_printGpsBuffer();		
	}
}


/********************************END OF FILE****************************/

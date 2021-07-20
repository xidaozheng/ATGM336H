#include "led.h"
#include "print.h"
#include "ATGM336H.h"


/*
*************************************************************************
*                             main ����
*************************************************************************
*/
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	
    ledInit();
	printInit(115200);
	ATGM336H_Init();			//UART4 ������9600
	
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

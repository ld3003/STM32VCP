#include <stm32f10x_usart.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <misc.h>

#include "BSP_Uart.h"
#include "atcmd.h"

CirQueue UartRingQueue;

void BSP_UART1Config(int baud)
{
	NVIC_InitTypeDef		NVIC_InitStructure;
	USART_InitTypeDef		USART_InitStructure;
	GPIO_InitTypeDef		GPIO_InitStructure;
	
	//��ʼ�����λ���
	InitQueue(&UartRingQueue);

	//RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//??UASRT???
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//??GPIOA???,?????,??????,????,???????????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	//GPIO
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz; //?????,IO???????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //??????;??????????GPIO_Mode_Out_PP????????,??????,?????????;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //???????,????????????IO?????
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//USART1
	USART_InitStructure.USART_BaudRate= baud;     //?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;      //???
	USART_InitStructure.USART_Parity= USART_Parity_No ;         //?????
	 
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None; //???
	 
	USART_InitStructure.USART_Mode =USART_Mode_Rx |USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	 
	USART_Cmd(USART1, ENABLE);
	
	//NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);      //????2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  //??USART????
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //?????
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //????
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //??????
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    //????????	 
	USART_Cmd(USART1, ENABLE);

}

void BSP_UART2Config(int baud)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 ,ENABLE);
  /*
  *  USART1_TX -> PA2 , USART1_RX ->	PA3
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStructure); 
  USART_Cmd(USART2, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
 
	USART_Cmd(USART2, ENABLE);//�1?�'??�1


	
	//
}


void BSP_UART2ConfigOff(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 ,ENABLE);
	
	USART_Cmd(USART2, DISABLE);//�1?�'??�1
  /*
  *  USART1_TX -> PA2 , USART1_RX ->	PA3
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);



	
	//
}

void UART1_Write_buffer(unsigned char * buffer, int length)
{
	int i=0;
	for(i=0;i<length;i++)
	{
		
		USART_SendData(USART1, buffer[i]);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}
		
		//
	}
	//
}


void UART2_Write_buffer(unsigned char * buffer, int length)
{
	int i=0;
	
	extern unsigned int SysTickCnt;
	extern unsigned int __UART_SEND_LED_TIM;
	__UART_SEND_LED_TIM = SysTickCnt + 100;
	for(i=0;i<length;i++)
	{
		
		USART_SendData(USART2, buffer[i]);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    {}
		
		//
	}
	//
}


#include <stm32f10x_usart.h>
void USART1_IRQHandler(void)
{
	unsigned char tmp = 0x0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		/* clear interrupt */
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		tmp = USART_ReceiveData(USART1);
		//recv
		//EnQueue(&UartRingQueue,tmp); //�������
		
	}
	if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		/* clear interrupt */
		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}

	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}

	if (USART_GetITStatus(USART1, USART_IT_CTS) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_CTS);
	}

	if (USART_GetITStatus(USART1, USART_IT_LBD) != RESET)
	{  
		USART_ClearITPendingBit(USART1, USART_IT_LBD);
	}

	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)    //??????
	{
		(u16)(USART1->DR & (u16)0x01FF);   
	}

}

void USART2_IRQHandler(void)
{
	unsigned char tmp = 0x0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		/* clear interrupt */
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		tmp = USART_ReceiveData(USART2);
		EnQueue(&UartRingQueue,tmp); //�������
		
	}
	if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		/* clear interrupt */
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}

	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);
	}

	if (USART_GetITStatus(USART2, USART_IT_CTS) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_CTS);
	}

	if (USART_GetITStatus(USART2, USART_IT_LBD) != RESET)
	{  
		USART_ClearITPendingBit(USART2, USART_IT_LBD);
	}

	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)    //??????
	{
		(u16)(USART2->DR & (u16)0x01FF);   
	}

}

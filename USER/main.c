/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The USART application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-10-30
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "ov2640api.h"
#include "BC95/neul_bc95.h"
#include "BASE64/cbase64.h"

static void takephoto(void);

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
int main(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	SysTick_Config(SystemCoreClock / 100);
	
	init_uart1();
	init_uart2();
	RTC_Init();
	SET_BOOTLOADER_STATUS(2);
	WKUP_Pin_Init();
	
	
	init_utimer();
	init_task();
	mainloop_init();
	init_mem();
	init_uart2_buffer();
	
	//neul_bc95_reboot();																	//初始化模块
	while(neul_bc95_get_netstat()<0);										//等待连接上网络
	
	{
		#define RECV_BUF_LEN 1024
		char *recvbuf = malloc(RECV_BUF_LEN);
		
		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("AT\r\n", strlen("AT\r\n"), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
		
		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("AE0\r\n", strlen("AE0\r\n"), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
		
		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("AT+NCDP=180.101.147.115\r\n", strlen("AT+NCDP=180.101.147.115\r\n"), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
		
		memset(recvbuf,0x0,RECV_BUF_LEN);
		
		#define SEND_STR "AT+NMGS=257,0001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506\r\n"
		uart_data_write(SEND_STR, strlen(SEND_STR), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 5000);
		
	}
	
	//
	printf("等待5000ms 进入 Standby 模式 \r\n");
	utimer_sleep(5000);
	//进入休眠
	Sys_Enter_Standby();
	
	
	
	
	for(;;){feed_watchdog();mainloop();}
	
	return 0;
}





#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	
  USART_SendData(USART1, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


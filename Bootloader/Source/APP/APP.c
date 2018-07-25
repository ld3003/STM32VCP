#include <string.h>
#include <stdio.h>
#include "APP.h"
#include "BSP_tim.h"
#include "BSP_Uart.h"
#include "HW_Config.h"
#include "atcmd.h"
#include "bsp.h"
#include "mem.h"
#include "flash.h"
#include "rtc.h"
#include "bkpreg.h"
#include "common.h"
#include "standby.h"
#include "ledctl.h"

#define SERIALPORT		fputcmod = 0;
#define USBPORT				fputcmod = 1;


unsigned int __UART_SEND_LED_TIM = 0;


static void process_atcmd(void);
static void process_usbdata(void);
static void app(void);


/*******************************************************************************
* Function Name : int main(void)
* Description   : 主程序入口
* Input         : 
* Output        : 
* Other         : 
* Date          : 2013.07.21
*******************************************************************************/
int main(void)
{
	
	RCC_ClocksTypeDef rccClk = {0};
	unsigned char runapp_status = 0; 
	
	
	//关闭 SWD 调试
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/*RCC SYSTICK RTC 初始化*/
	RCC_GetClocksFreq(&rccClk);
	SysTick_Config(rccClk.HCLK_Frequency / 100);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	
	WKUP_Pin_Init();
	
	BSP_UART1Config(115200);
	BSP_UART2Config(115200);
	
	
	printf("Wakeup Bootloader 状态 %d \r\n",GET_BOOTLOADER_STATUS);
	
	app();
	
}

static void app(void)
{
	
	//led0_on();
	extern unsigned int SysTickCnt;
	unsigned int SysTickCntRecord;
	unsigned int tmpCnt;
	int i=0;
	
	init_mem();
	
	/*Uart1 INIT*/
	BSP_UART1Config(115200);
	
	usben_on();
	USB_Config();
	init_led();
	modem_poweron();
	while (1)
	{
		if (SysTickCnt > __UART_SEND_LED_TIM)
		{
			LED_TYPEA;
		}else{
			LED_TYPEB;
		}
		
		//喂狗操作
		feed_watchdog();
		//处理USB 数据
		process_usbdata();
		//处理串口AT数据
		process_atcmd();	
	}
}

static void process_atcmd(void)
{
	//处理AT指令
	switch(status_recv)
	{
		case STATUS_RECV_FINISH:
			break;
		case STATUS_RECV_ERROR:
			atcmd_recv_reset(); //发生错误了要复位
			break;
		default:
			break;
	}
	//
}

static void process_usbdata(void)
{
	uint32_t len = 0;
	static uint8_t buf[256] = {0};
	
	//将串口2发送过来的额数据，通过USB转发出去
	if (QueueIsEmpty(&UartRingQueue) == 0)
	{
		uint8_t tmp = (uint8_t)DeQueue(&UartRingQueue);
		USB_TxWrite(&tmp, 1);
	}
	
	len = USB_RxRead(buf, sizeof(buf));
	
	if (len > 0)
	{
		unsigned short i=0;
		
		UART1_Write_buffer(buf,len);
		UART2_Write_buffer(buf,len);
	}
	
}

int fputc(int ch, FILE *f)
{
	
	USART_SendData(USART1, ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	return (ch);
}


void write_usb_buffer(unsigned char*buf , int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		EnQueue(&UartRingQueue,buf[i]);
	}
}





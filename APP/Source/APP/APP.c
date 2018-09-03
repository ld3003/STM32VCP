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
* Description   : ���������
* Input         : 
* Output        : 
* Other         : 
* Date          : 2013.07.21
*******************************************************************************/
int main(void)
{
	
	RCC_ClocksTypeDef rccClk = {0};
	unsigned char runapp_status = 0; 
	
	
	//�ر� SWD ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/*RCC SYSTICK RTC ��ʼ��*/
	RCC_GetClocksFreq(&rccClk);
	SysTick_Config(rccClk.HCLK_Frequency / 100);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	
	WKUP_Pin_Init();
	
	BSP_UART1Config(115200);
	BSP_UART2Config(115200);
	
	
	printf("Wakeup Bootloader ״̬ %d \r\n",GET_BOOTLOADER_STATUS);
	
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
		
		//ι������
		feed_watchdog();
		//����USB ����
		process_usbdata();
		//������AT����
		process_atcmd();	
	}
}

static void process_atcmd(void)
{
	//����ATָ��
	switch(status_recv)
	{
		case STATUS_RECV_FINISH:
			break;
		case STATUS_RECV_ERROR:
			atcmd_recv_reset(); //����������Ҫ��λ
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
	
	//������2���͹����Ķ����ݣ�ͨ��USBת����ȥ
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





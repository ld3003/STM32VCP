
#include "main.h"
#include "ov2640api.h"
#include "BC95/neul_bc95.h"
#include "BASE64/cbase64.h"
#include "JSON/cjson.h"

static int make_send_str(char *outstr , int outbuf_len ,char * instr)
{
	int i=0;
	extern int encode_base64(unsigned char * input , int inlen , char * output);
	extern int decode_base64(const char * input , char * output , int * outlen);
	extern int conv_hex_2_string(unsigned char *data , int datalen , char *str);
	
	memset(outstr,0x0,outbuf_len);
	
	char *b64buf = malloc(512);
	char *hexbuf = malloc(512);
	
	//数据包长度为256
	if (strlen(instr) <= 256)
	{
		for(i=strlen(instr);i<256;i++)
		{
			instr[i] = ' ';
			//
		}
		instr[256] = 0x0;
	}else{
		instr[256] = 0x0;
	}
	
	//encode_base64(inbuf,inbuf_len,b64buf);
	//printf("BASE64: %s \r\n",b64buf);
	snprintf((char *)outstr,outbuf_len,"AT+NMGS=%d,",257);
	conv_hex_2_string((unsigned char*)instr,strlen(instr),hexbuf);
	//printf("HEX BUF : %s ",hexbuf);
	
	strcat((char*)outstr,"00");
	strcat((char*)outstr,hexbuf);
	strcat((char*)outstr,"\r\n");
	
	free(b64buf);
	free(hexbuf);
	
	
	return strlen(outstr);
}

static int make_json_data(char *oustr)
{
	
	char * p = 0;
	cJSON * pJsonRoot = NULL;
	char tmpstr[32];
 

	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot){return -1;}
	
	cJSON_AddNumberToObject(pJsonRoot, "version", 1);
	cJSON_AddStringToObject(pJsonRoot, "ipaddress", "202.99.96.68");
	cJSON_AddStringToObject(pJsonRoot, "ipport", "3000");
	
	p = cJSON_Print(pJsonRoot);
	
	if(NULL == p)
	{
		cJSON_Delete(pJsonRoot);
		return -1;
	}
	cJSON_Delete(pJsonRoot);
	
	sprintf(oustr,"%s",p);
	
	printf("JSON:%s\r\n",oustr);
	
	free(p);
	return 0;
}

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
	init_led();
	SysTick_Config(SystemCoreClock / 100);
	
	init_uart1();
	init_uart2();
	RTC_Init();
	SET_BOOTLOADER_STATUS(2);
	WKUP_Pin_Init();
	
	init_utimer();
	init_task();
	init_mem();
	init_uart2_buffer();
	
	for(;;){};
	
	while(neul_bc95_get_netstat()<0){};										//等待连接上网络
	
	{
		
		/*
		 * 分配内存
		 */
		#define RECV_BUF_LEN 1024
		char *recvbuf = malloc(RECV_BUF_LEN);
		char *atbuf = malloc(1024);
		char *jsonbuf = malloc(512);
		
		/*
		 * 发送AT指令
		 */
		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("AT\r\n", strlen("AT\r\n"), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
		
		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("ATE0\r\n", strlen("ATE0\r\n"), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
		
		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("AT+CSQ\r\n", strlen("AT+CSQ\r\n"), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
		
		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("AT+NCDP=180.101.147.115\r\n", strlen("AT+NCDP=180.101.147.115\r\n"), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
		
		memset(recvbuf,0x0,RECV_BUF_LEN);
		
		
		make_json_data(jsonbuf);
		make_send_str(atbuf,1024,jsonbuf);
		uart_data_write(atbuf, strlen(atbuf), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 5000);
		
		/*
		释放内存
		*/
		free(recvbuf);
		free(atbuf);
		free(jsonbuf);
	}
	
	printf("Sys_Enter_Standby CurrentTim %d\r\n",RTC_GetCounter());
	
	RTC_SetAlarm(RTC_GetCounter() + 10);
	utimer_sleep(1000);
	//进入休眠
	Sys_Enter_Standby();

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


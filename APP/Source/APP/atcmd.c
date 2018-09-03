#include "atcmd.h"
#include <stdio.h>
#include <BSP_Uart.h>
#include <APP.h>

#include "flash.h"
#include "common.h"

#include <string.h>
#include <mem.h>
#include "common.h"

unsigned char recvbuf[UART_RECV_BUFFER_LEN];
int recvbuffer_length = 0;
unsigned char status_recv = STATUS_RECV_INIT;

void atcmd_recv_reset(void)
{
	recvbuffer_length = 0;
	status_recv = STATUS_RECV_INIT;
}

void uart_recv_handle(unsigned char data)
{
	
	switch(data)
	{
		case '+':
		{
			recvbuffer_length = 0;
			status_recv = STATUS_RECV_PLUS;
			break;
		}
		
		case 0x0D:
		{
			recvbuf[recvbuffer_length++] = 0x0;
			status_recv = STATUS_RECV_FINISH;
			break;
		}
		
	}

	switch(status_recv)
	{
		
		case STATUS_RECV_PLUS:
		{
			if (recvbuffer_length < UART_RECV_BUFFER_LEN)
				recvbuf[recvbuffer_length++] = data;
			break;
		}
		
		case STATUS_RECV_FINISH:
		{
			int i=0;
			
			printf("���ݳ��� %d\r\n",recvbuffer_length);
			printf("�������� %s\r\n",recvbuf);
			
			process_cmddata();
			
			status_recv = STATUS_RECV_INIT;
			
			break;
		}
		
		case STATUS_RECV_INIT:
			break;
		
	}
	//
}

#include <string.h>
void process_cmddata(void)
{
	int retcode = -1;
	
	if      (strstr((char*)recvbuf,"+1234567890123") == (char*)recvbuf)
	{
		retcode = 0;
		//
	}
	else if (strstr((char*)recvbuf,"+STARTBURN") == (char*)recvbuf)
	{
		//+STARTBURN=1230342348,1024\r\n;
		char *p1,*p2,*p3,*p4,*p5,*p6;
		unsigned int address,size;
		//
		p1 = strstr((char*)recvbuf,"=");
		p2 = strstr(p1+1,",");
		p3 = strstr(p2+1,"\r");
		
		p1[0] = 0x0;
		p2[0] = 0x0;
		
		printf("[%s] [%s] [%s] \r\n",p1+1,p2+1,p3+1);
		
		printf("@@ address [%s] size [%s] \r\n",p1+1,p2+1);
		
		sscanf(p1+1,"%d",&address);
		sscanf(p2+1,"%d",&size);
		
		
		printf("## address [%d:%d] size [%d:%d] \r\n",APPLICATION_ADDRESS,address,APPLICATION_SIZE,size);
		
		//��ʼ��¼
		//FLASH_ProgramStart(APPLICATION_ADDRESS,APPLICATION_SIZE);
		FLASH_ProgramStart(address,size);
		retcode = 0;
		//
	}
	else if (strstr((char*)recvbuf,"+ENDBURN") == (char*)recvbuf)
	{
		//������¼
		FLASH_AppendEnd();
		FLASH_ProgramDone();
		retcode = 0;
		//
	}
	else if (strstr((char*)recvbuf,"+BURNHEX=") == (char*)recvbuf)
	{
		unsigned int len;
		unsigned char *buf = (unsigned char*)alloc_mem(__FILE__,__LINE__,512);
		//printf("(char*)recvbuf+9 : %d \r\n",strlen((char*)recvbuf+9));
		len = conv_string_2_hex((char*)recvbuf+9,buf);
		printf("BURN LEN %d \r\n",len);
		//debug_buf("BURN DATA",buf,len);
		FLASH_AppendBuffer(buf,len);
		free_mem(__FILE__,__LINE__,buf);
		
		
		
		retcode = 0;
	}
	else if (strstr((char*)recvbuf,"+APP") == (char*)recvbuf)
	{
		//gotoApp();
		retcode = 0;
		//
	}
	else if (strstr((char*)recvbuf,"+RFLASH=") == (char*)recvbuf)
	{
		char *p1,*p2,*p3,*p4,*p5,*p6;
		unsigned int address,size;
		//
		p1 = strstr((char*)recvbuf,"=");
		p2 = strstr(p1+1,",");
		p3 = strstr(p2+1,"\r");
		
		p1[0] = 0x0;
		p2[0] = 0x0;
		
		printf("[%s] [%s] [%s] \r\n",p1+1,p2+1,p3+1);
		
		printf("@@ address [%s] size [%s] \r\n",p1+1,p2+1);
		
		sscanf(p1+1,"%d",&address);
		sscanf(p2+1,"%d",&size);
		
		//printf("DATA : %s \r\n",(unsigned char*)address);
		write_usb_buffer((unsigned char*)address,size);
		
		retcode = 10;
		//
	}
	else if (strstr((char*)recvbuf,"+AT") == (char*)recvbuf)
	{
		retcode = 0;
		//
	}
	
	switch(retcode)
	{
		case 0:
		{
			char tmpbuf[32];
			snprintf(tmpbuf,32,"OK:%d\r\n",retcode);
			write_usb_buffer((unsigned char*)tmpbuf,strlen(tmpbuf));
			break;
		}
		
		case 10:
		{
			char tmpbuf[32];
			snprintf(tmpbuf,32,"OK:%d\r\n",retcode);
			write_usb_buffer((unsigned char*)tmpbuf,strlen(tmpbuf));
			break;
		}
		
		default:
		{
			char tmpbuf[32];
			snprintf(tmpbuf,32,"ERROR:%d\r\n",retcode);
			write_usb_buffer((unsigned char*)tmpbuf,strlen(tmpbuf));
		}
	}
	
	//
}


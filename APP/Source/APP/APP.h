/*
********************************************************************************
*
*                                 APP.h
*
* File          : APP.h
* Version       : V1.0
* Author        : whq
* Mode          : Thumb2
* Toolchain     : 
* Description   : ������ͷ�ļ�
*                
* History       :
* Date          : 2013.07.21
*******************************************************************************/



#ifndef _APP_H_
#define _APP_H_

extern unsigned int __UART_SEND_LED_TIM;
void write_usb_buffer(unsigned char*buf , int len);
void gotoApp(void);

#endif


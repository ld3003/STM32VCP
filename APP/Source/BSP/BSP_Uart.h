#ifndef __BSP_UART__
#define __BSP_UART__

#include "RingBuffer.h"

extern CirQueue UartRingQueue;
void BSP_UART1Config(int baud);
void UART1_Write_buffer(unsigned char * buffer, int length);

void BSP_UART2Config(int baud);
void BSP_UART2ConfigOff(void);
void UART2_Write_buffer(unsigned char * buffer, int length);

#endif


#ifndef __AT_CMD__
#define __AT_CMD__

#define UART_RECV_BUFFER_LEN	128

/**
ATָ���ʹ�÷���

ͨ��״̬���������ȡ��ǰ��������״̬
����������״̬Ϊ STATUS_RECV_FINISH ��ʱ����������Ѿ�����һ���Ϸ�֡
��ԺϷ�֡����Ӧ��������������ֵ״̬��

atcmd_recv_reset ���ô˺����󣬻��������������µ�֡

while

*/


enum {
	STATUS_RECV_INIT,				/// ���ճ�ʼ��
	STATUS_RECV_PLUS,				/// �յ� +
	STATUS_RECV_A,					
	STATUS_RECV_T,
	STATUS_RECV_0D,					/// �յ�0X0D
	STATUS_RECV_0A,					/// �յ�0X0A
	STATUS_RECV_FINISH,			/// �յ���Ч����
	STATUS_RECV_ERROR,			/// ���մ���
};
extern int recvbuffer_length;
extern unsigned char recvbuf[UART_RECV_BUFFER_LEN];
extern unsigned char status_recv;

void uart_recv_handle(unsigned char data);		/// �����жϵ��ô˺��������ڴ�����յ��Ĵ�������
void atcmd_recv_reset(void);									/// ��������λ����
void process_cmddata(void);

#endif

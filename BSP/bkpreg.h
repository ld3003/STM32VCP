#ifndef __bkpreg_h__
#define __bkpreg_h__

/*
#define BKP_DR1			RTC ��ʼ����¼
#define BKP_DR2			ϵͳ����������
#define BKP_DR3			
#define BKP_DR4			���һ�α���ʱ�䣬32λ

#define BKP_DR5			ϵͳ״̬                           ((uint16_t)0x0014)
#define BKP_DR6			                           ((uint16_t)0x0018)
#define BKP_DR7			6,7�洢��һ������ʱ��                           ((uint16_t)0x001C)
#define BKP_DR8			��ֹ��ʱ״̬                           ((uint16_t)0x0020)
#define BKP_DR9                           ((uint16_t)0x0024)
#define BKP_DR10                          ((uint16_t)0x0028)
#define BKP_DR11                          ((uint16_t)0x0040)
#define BKP_DR12                          ((uint16_t)0x0044)
#define BKP_DR13                          ((uint16_t)0x0048)
#define BKP_DR14                          ((uint16_t)0x004C)
#define BKP_DR15                          ((uint16_t)0x0050)
#define BKP_DR16                          ((uint16_t)0x0054)
#define BKP_DR17                          ((uint16_t)0x0058)
#define BKP_DR18                          ((uint16_t)0x005C)
#define BKP_DR19                          ((uint16_t)0x0060)
#define BKP_DR20                          ((uint16_t)0x0064)
#define BKP_DR21                          ((uint16_t)0x0068)
#define BKP_DR22                          ((uint16_t)0x006C)
#define BKP_DR23                          ((uint16_t)0x0070)
#define BKP_DR24                          ((uint16_t)0x0074)
#define BKP_DR25                          ((uint16_t)0x0078)
#define BKP_DR26                          ((uint16_t)0x007C)
#define BKP_DR27                          ((uint16_t)0x0080)
#define BKP_DR28                          ((uint16_t)0x0084)
#define BKP_DR29                          ((uint16_t)0x0088)
#define BKP_DR30                          ((uint16_t)0x008C)
#define BKP_DR31                          ((uint16_t)0x0090)
#define BKP_DR32                          ((uint16_t)0x0094)
#define BKP_DR33                          ((uint16_t)0x0098)
#define BKP_DR34                          ((uint16_t)0x009C)
#define BKP_DR35                          ((uint16_t)0x00A0)
#define BKP_DR36                          ((uint16_t)0x00A4)
#define BKP_DR37                          ((uint16_t)0x00A8)
#define BKP_DR38                          ((uint16_t)0x00AC)
#define BKP_DR39                          ((uint16_t)0x00B0)
#define BKP_DR40                          ((uint16_t)0x00B4)
#define BKP_DR41                          ((uint16_t)0x00B8)
#define BKP_DR42                          ((uint16_t)0x00BC)
*/


unsigned int bkp32bit_read(unsigned short a , unsigned short b);
void bkp32bit_write(unsigned int val , unsigned short *a , unsigned short *b);
void __set_last_alarm_time(void);
unsigned int __get_last_alarm_time(void);
void __set_next_wakeup_time(unsigned int time);
unsigned int __get_next_wakeup_time(void);


#define GET_SYSTEM_COUNTER  BKP_ReadBackupRegister(BKP_DR2)
#define SET_SYSTEM_COUNTER	BKP_WriteBackupRegister(BKP_DR2,GET_SYSTEM_COUNTER+1)

#define SET_LAST_ALARM_TIME	__set_last_alarm_time()
#define GET_LAST_ALARM_TIME	__get_last_alarm_time()

#define GET_SYSTEM_STATUS 	BKP_ReadBackupRegister(BKP_DR5)
#define SET_SYSTEM_STATUS(X)		BKP_WriteBackupRegister(BKP_DR5,X)

#define SET_NEXT_WAKEUP_TIME(X)	__set_next_wakeup_time(X)
#define GET_NEXT_WAKEUP_TIME	__get_next_wakeup_time()

#define GET_MOTIONLESS_STATUS	BKP_ReadBackupRegister(BKP_DR8)
#define SET_MOTIONLESS_STATUS(X)	BKP_WriteBackupRegister(BKP_DR8,X)


#pragma pack(push)
#pragma  pack(1)
struct BKP_REG {
	unsigned char SYSTEM_RESET_COUNTE;
	unsigned int LAST_ALARM_TIME;
	unsigned int WAKE_UP_TIME;
	unsigned char SYSTEM_STATUS;
};
#pragma pack(pop)

void read_bkp_buffer(unsigned char*buf);
void write_bkp_buffer(unsigned char*buf);

#endif

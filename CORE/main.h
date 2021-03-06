#ifndef __main_h__
#define __main_h__

#include "stm32f10x.h"
#include "bsp.h"
#include <stdio.h>
#include <stdlib.h>
#include "utimer.h"
#include "task.h"
#include "mem.h"
#include "serialport.h"
#include "mainloop.h"
#include "bsp.h"
#include "config.h"

enum SYSTEM_STATUS {
	SYSTEM_STATUS_INIT = 0,
	SYSTEM_STATUS_WAIT_WAKEUP,
	SYSTEM_STATUS_DEEPSLEEP,		//深度睡眠，只有当下次启动时才会被唤醒
	SYSTEM_STATUS_TAKEPHOTH,
	SYSTEM_STATUS_RUN,
	SYSTEM_STATUS_CHECKALARM,
};



#endif

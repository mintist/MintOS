#ifndef _BSP_H
#define _BSP_H

#include "usart1.h"
#include "usart2.h"
#include "led.h"
#include "m_os.h"


void bsp_init(void);
//ϵͳʱ�����ú���
void systick_config(void);

#endif

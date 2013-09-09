#ifndef __USART2_H
#define	__USART2_H

#include "stm32f10x.h"
#include <stdio.h>

void USART2_Configuration(u32 baudrate);
void USART2_printf (char *fmt, ...);

#endif /* __USART_H */


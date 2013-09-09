#include "bsp.h"
#include "m_os.h"

void bsp_init(void)
{
	LED_Configuration();
	USART1_Configuration(115200);
	USART2_Configuration(115200);
}


//ϵͳʱ�����ã����1ms����һ���ж�
void systick_config(void)
{
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	
	SysTick_Config(rcc_clocks.HCLK_Frequency/M_TICKS_PER_SEC);
}


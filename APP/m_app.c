#include "m_os.h"

/* 定义任务变量 */
#define GLOBAL_APP
#include "m_app.h"
#include "bsp.h"



void StartTask(void *pdata)
{
	systick_config();
	
	m_create_task(Task0,&TASK0_STK[TASK0_TCB_SIZE-1],Task0_Prio);
	m_create_task(Task1,&TASK1_STK[TASK1_TCB_SIZE-1],Task1_Prio);
	
	/* 建立其他任务之后删除自己，但是其实空间还是被占据的 */
	m_delete_task(Start_Prio);
}

void Task0(void *pdata)
{
	while(1)
	{
		uint32_t Task0SP = __current_sp();
		printf("Task0's SP is %X \r\n",Task0SP);
		
		m_delay_ticks(Task0_Prio,M_TICKS_PER_SEC);
//		m_delay_dhms(Task0_Prio,0,0,0,1);
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);	
		m_delay_ticks(Task0_Prio,M_TICKS_PER_SEC);
//		m_delay_dhms(Task0_Prio,0,0,0,1);
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		
//		USART2_printf("%s \r\n","USART2_printf");
	}
}

void Task1(void *pdata)
{
	while(1)
	{
		uint32_t Task1SP = __current_sp();
		printf("Task1's SP is %X \r\n",Task1SP);
		
		m_delay_ticks(Task1_Prio,M_TICKS_PER_SEC);
//		m_delay_dhms(Task1_Prio,0,0,0,1);
		GPIO_SetBits(GPIOD, GPIO_Pin_2);	
		m_delay_ticks(Task1_Prio,M_TICKS_PER_SEC);
//		m_delay_dhms(Task1_Prio,0,0,0,1);
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);	

	}
}











/**
  ******************************************************************************
  * @file    m_core.c
  * @author  Ted Lin
  * @version V0.0.0
  * @date    2013/8/29
  * @brief   This file provides all the miscellaneous firmware functions (add-on
  *          to CMSIS functions).
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  * 
  * 
  * 
  *
  * <h2><center>&copy; COPYLEFT 2013 Ted Lin</center></h2>
  ******************************************************************************
  */


/** 
  * @brief	将Mint OS的全局变量定义在此文件中
  */
#define GLOBAL_VAR

/* Includes ------------------------------------------------------------------*/
#include "m_os.h"

/** @defgroup Mint_OS_Core	1. 内核调度
  * @brief Mint OS的内核调度函数和变量。
  */

/**
  * @brief  系统初始化函数，一般位于bsp_init()之后，空闲任务在
  *		此函数被创建。
  * @param  无
  * @retval 无
  */
void m_init(void)
{
	m_create_task(m_idle_task,&m_idle_stk[IDLE_STK_SIZE-1],IDLE_PRIO);
}

/*
	1. 获取创建的最高任务的优先级
	2. 设置一些全局变量
	3. 最高优先级任务，走你
*/
/**
  * @brief  系统初始启动函数，一般位于TaskStart（任务启动任务）被
  *		创建之后调用，简要步骤如下：
  *		1. 获取最高优先级任务
  *		2. 设置最该优先级以及当前优先级栈地址
  *		3. 调用m_start_high启动Mint OS
  * @param  无
  * @retval 无
  */
void m_start(void)
{
	m_get_high_prio();
	
	m_cur_prio = m_high_prio;
	m_cur_tcb = &tcb[m_cur_prio];
	m_high_tcb = &tcb[m_high_prio];
	
	m_start_high();
}

/**
  * @brief  系统最核心的函数之一：任务调度函数，总是去调度就绪
  *		任务表中优先级最高的任务。简要步骤如下：
  *		1. 寻找就续表中的最高优先级
  *			2. 如果比当前优先级更高，则
  *				2.1 设置最该优先级以及当前优先级栈地址
  *				2.2 切换任务
  * @param  无
  * @retval 无
  */
void m_sched(void)
{
	m_get_high_prio();
	m_enter_critical();
	if(m_high_prio != m_cur_prio)
	{
		m_cur_tcb = &tcb[m_cur_prio];
		m_high_tcb = &tcb[m_high_prio];
		m_cur_prio = m_high_prio;
		m_exit_critical();
		m_switch_task();
	}
	else
	{
		m_exit_critical();
	}
}

/**
  * @brief  进入中断之后可调用，但如若调用，需要与m_exit_int
  *		配对使用。
  * @param  无
  * @retval 无
  */
void m_enter_int(void)
{
//	m_enter_critical();
	m_int_nest++;
	tcb[m_cur_prio].status = ISR;
}

/**
  * @brief  中断退出函数，与m_enter_int配对使用
  * @param  无
  * @retval 无
  */
void m_exit_int(void)
{
	m_int_nest--;
	m_sched();
//	m_exit_critical();
}

/**
  * @}
  */



/******************* (C) COPYLEFT 2013 Ted Lin *****END OF FILE****/


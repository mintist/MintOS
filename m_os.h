/**
  ******************************************************************************
  * @file    m_os.h
  * @author  Ted Lin
  * @version V0.0.0
  * @date    2013/8/29
  * @brief   此文件包含：关于Mint OS的所有全局变量定义，对内API和对外API 的声明
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _M_OS_H
#define _M_OS_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stm32f10x.h>

#include "m_port.h"
#include "m_os_cfg.h"

  
/** 
  * @brief	预防全局变量重定义宏，将头文件写成如下这样，然后再程序中的某一个
  *		地方使用#define GLOBAL_VAR，并且包含这个头文件，就能保证定义只存在于
  *		一个地方，其他地方都是extern。
  */
#ifdef GLOBAL_VAR
#define GLOBAL
#else
#define GLOBAL extern
#endif






/** @addtogroup Mint_OS_Task
  * @{
  */
  
#define USER_TASKS		200				/*!<  用户定义自己需要用的任务数目 */

/** 
  * @brief	任务的函数指针类型
  */
typedef void (*m_task)(void* pdata);
/**
  * @}
  */



/** @addtogroup Mint_OS_Core
  * @{
  */
/** 
  * @brief	任务控制块（RCB）的属性之一：任务的状态，目前还没有用上，
  * 	留作以后扩展用。
  */
typedef enum{
	SLEEP 	= 	0,				/*!<  休眠态 */
	READY 	= 	1,				/*!<  准备态 */
	RUNNING =	2,				/*!<  运行态 */
	DELAYED =	3,				/*!<  延迟态 */
	ISR		=	4,				/*!<  中断态 */
	BLOCKED =	5				/*!<  阻塞态 */
}m_status;


/** 
  * @brief  Mint OS 任务控制块（TCB）类型定义 
  */
typedef struct m_tcb{
	uint32_t* 	sp;				 /*!< 私有堆栈指针
                                        @note: 此变量地址放在第一位置的好处是与tcb地址相同，移植时直接用sp地址表示tcb地址 */

#if TICK_TIME_SCHED_DLIST
	struct m_tcb* prev;			/*!<  延时链表采用双链表 */
#endif
#if TICK_TIME_SCHED_DLIST || TICK_TIME_SCHED_LIST || TICK_TIME_SCHED_IM_LIST
	struct m_tcb* next;			/*!<  延时链表采用单链表或双链表 */
#endif
	
	uint32_t 	ticks;			/*!<  延时滴答次数 */
	uint32_t	prio;			/*!<  任务优先级 */
	m_status	status;			/*!<  当前任务状态 */
}m_tcb;

/** 
  * @brief	延时/空闲/阻塞任务链表的表头
  */
typedef struct{
	m_tcb*		head;			/*!<  表头 */
	uint32_t	size;			/*!<  被延时的任务个数 */
}m_list;


/**
  * @}
  */


/** @addtogroup Mint_OS_Core
  * @{
  */

#define SYS_TASKS 	2			/*<	 系统任务数目，最高优先级-0和最低优先级任务作为系统保留 */
#define N_TASKS		(USER_TASKS+SYS_TASKS)	/*!<  总的任务数目，由系统任务和用户任务两部分构成 */
GLOBAL m_tcb tcb[N_TASKS];					/*!<  所有任务的任务控制块 */

GLOBAL m_tcb*	m_cur_tcb;					/*!<  当前任务tcb指针 */
GLOBAL m_tcb*	m_high_tcb;					/*!<  最高任务tcb指针，在上下文切换时，与前者不同 */

GLOBAL uint32_t	m_cur_prio;					/*!<  当前任务优先级 */
GLOBAL uint32_t	m_high_prio;				/*!<  最高任务优先级 */

GLOBAL uint32_t m_int_nest;					/*!<  中断嵌套层数 */

GLOBAL uint32_t m_time;						/*!<  系统运行“滴答”数 */

#define IDLE_PRIO (N_TASKS-1)				/*!<  空闲任务，即最低优先级任务，用于空转或低功耗设置 */
#define IDLE_STK_SIZE	100					/*!<  空闲任务私有堆栈大小 */
GLOBAL uint32_t m_idle_stk[IDLE_STK_SIZE];	/*!<  空闲任务的私有堆栈空间 */
void m_idle_task(void* pdata);				/*!<  空闲任务的任务代码 */

#if TICK_TIME_SCHED_LIST
GLOBAL m_list 	m_delayed_list;		/*!< 延时任务链表 */
GLOBAL m_list 	m_free_list;		/*!< 空闲任务链表 */
GLOBAL m_list 	m_ready_list;		/*!< 就绪任务链表 */
#endif

#if TICK_TIME_SCHED_DLIST
GLOBAL m_list 	m_delayed_dlist;	/*!< 延时任务链表 */
GLOBAL m_list 	m_free_dlist;		/*!< 空闲任务链表 */
GLOBAL m_list 	m_ready_dlist;		/*!< 就绪任务链表 */
#endif

#if TICK_TIME_SCHED_IM_LIST
GLOBAL m_list 	m_delayed_imlist;	/*!< 延时任务链表 */
#endif

/**
  * @}
  */

#ifdef __cplusplus
extern "C"{
#endif

/** 
  * @brief	对内服务API
  */

/** @addtogroup Mint_OS_Core
  * @{
  */

void m_sched(void);
/**
  * @}
  */
	
/** @addtogroup Mint_OS_Port
  * @{
  */
#define M_TICKS_PER_SEC	1000	 		/*!<  设置每秒的节拍数目 */
uint32_t* m_init_sp(m_task task, uint32_t* sp);
/**
  * @}
  */
/** @addtogroup Mint_OS_Task
  * @{
  */
void m_set_prio(uint32_t prio);
void m_reset_prio(uint32_t prio);
void m_get_high_prio(void);
/**
  * @}
  */
/** @addtogroup Mint_OS_Time
  * @{
  */ 
#if TICK_TIME_SCHED_LIST
void m_add_list(m_list* list,m_tcb* tcb);
void m_rmv_list(m_list* list,m_tcb* prev);
void m_rvs_list(m_list* list);
#endif

#if TICK_TIME_SCHED_DLIST
void m_add_dlist(m_list* dlist,m_tcb* tcb);
void m_rmv_dlist(m_list* dlist,m_tcb* cur);
void m_rvs_dlist(m_list* dlist);
#endif

#if TICK_TIME_SCHED_IM_LIST
void m_add_imlist(m_list* list,m_tcb* tcb);
void m_rvs_imlist(m_list* list);
#endif

/**
  * @}
  */

/**********************************************/


/** 
  * @brief	对外服务API
  */

/** @addtogroup Mint_OS_Core
  * @{
  */
void m_init(void);
void m_start(void);

void m_enter_int(void);
void m_exit_int(void);
/**
  * @}
  */

/** @addtogroup Mint_OS_Task
  * @{
  */
void m_create_task(m_task task, uint32_t*  sp, uint32_t prio);
void m_delete_task(uint32_t prio);
/**
  * @}
  */
/** @addtogroup Mint_OS_Time
  * @{
  */
void m_delay_ticks(uint32_t prio, uint32_t ticks);
void m_delay_dhms(uint32_t prio, uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
void m_tick_time(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif	/*!< END OF M_OS_H */






/******************* (C) COPYLEFT 2013 Ted Lin *****END OF FILE****/

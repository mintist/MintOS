#ifndef _MINT_OS_H
#define _MINT_OS_H

#include <stdint.h>
#include <stdio.h>
#include <stm32f10x.h>

#include "m_port.h"
#include "m_os_cfg.h"


#ifdef GLOBAL_VAR
#define GLOBAL
#else
#define GLOBAL extern
#endif

/* 任务的函数指针类型 */
typedef void (*m_task)(void* pdata);


/* 系统类型定义 */
typedef struct m_tcb{			//任务控制块
	uint32_t* 	sp;
	uint32_t 	ticks;
}m_tcb;



#define USER_TASKS	300
#define SYS_TASKS 	2
#define N_TASKS		(USER_TASKS+SYS_TASKS)

GLOBAL m_tcb tcb[N_TASKS];

/* 系统变量全局 */
GLOBAL uint32_t	m_prio_tbl[N_TASKS/32+1];

GLOBAL m_tcb*	m_cur_tcb;
GLOBAL m_tcb*	m_high_tcb;

GLOBAL uint32_t	m_cur_prio;
GLOBAL uint32_t	m_high_prio;

GLOBAL uint32_t m_int_nest;

GLOBAL uint32_t m_time;

#define IDLE_PRIO (N_TASKS-1)
#define IDLE_STK_SIZE	100
GLOBAL uint32_t m_idle_stk[IDLE_STK_SIZE];
void m_idle_task(void* pdata);


/* 系统对内服务 */

void m_sched(void);
uint32_t* m_init_sp(m_task task, uint32_t* sp);

void m_set_prio(uint32_t prio);
void m_reset_prio(uint32_t prio);
void m_get_high_prio(void);
	
/* 系统对外服务 */
void m_enter_int(void);
void m_tick_time(void);
void m_exit_int(void);

void m_init(void);
void m_start(void);

void m_create_task(m_task task, uint32_t*  sp, uint32_t prio);
void m_delete_task(uint32_t prio);

void m_delay_time(uint32_t prio, uint32_t ticks);

#endif

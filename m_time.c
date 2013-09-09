/**
  ******************************************************************************
  * @file    m_time.c
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
  
/* Includes ------------------------------------------------------------------*/ 
#include "m_os.h"

/** @defgroup Mint_OS_Time 3. 时间管理
  * @brief Mint OS时间相关函数
  */
  

  
/** @addtogroup Mint_OS_Time
  * @{
  */


/**
  * @brief  延时指定任务优先级延时ticks次“滴答”时钟
  * @param  prio: 指定的任务优先级
  *   此参数可能为以下取值:
  *     @arg 非休眠态的任务优先级
  * @param  ticks: 延时的“滴答”数目
  * @retval None
  */
void m_delay_ticks(uint32_t prio, uint32_t ticks)
{
	if(ticks == 0)
		return;
	
	m_enter_critical();
	tcb[prio].ticks = ticks;
	m_exit_critical();
	
	m_reset_prio(prio);
#if TICK_TIME_SCHED_LIST
	m_add_list(&m_delayed_list,&tcb[prio]);
#endif
	
#if TICK_TIME_SCHED_DLIST
	m_add_dlist(&m_delayed_dlist,&tcb[prio]);
#endif
	
#if TICK_TIME_SCHED_IM_LIST
	m_add_imlist(&m_delayed_imlist,&tcb[prio]);
#endif

	m_sched();
}

/**
  * @brief  
  * @param  prio: 指定的任务优先级
  *   此参数可能为以下取值:
  *     @arg 非休眠态的任务优先级
  * @param  days: 延时的天数，若延时太久会加重TimeTick的负担。
  *     @arg 0~355
  * @param  hours: 延时小时数
  *     @arg 0~23
  * @param  minutes: 延时分钟数
  *     @arg 0~59
  * @param  seconds: 实验秒数，如要延时小于1s，请调用m_delay_ticks，可到1ms。
  *     @arg 0~59
  * @retval 无
  */
void m_delay_dhms(uint32_t prio,uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	uint32_t ticks;
	ticks = seconds + minutes * 60u + hours * 3600u + days * 86400u;
	ticks *= M_TICKS_PER_SEC;
	m_delay_ticks(prio, ticks);
}

/*
	1. 记录系统运行时间+1
	2. 遍历TCB对delay参数减一
	3. 再看看有没有任务延时到期
	4. 如果有，则
		4.1 设置任务状态
*/
/**
  * @brief  系统“滴答”函数，在Cortex-M3中，位于系统时钟SysTick_Handler内，
  *				此函数每被调用一次，所有被延时的任务的ticks减1，并将从非0到0的任务设置为就绪。
  * 	@note 此函数的实现由多种方式：数组，单链表，双链表还有一种巧妙的方法（保留）
  * @param  无 
  * @retval 无
  */
void m_tick_time(void)
{
#if TICK_TIME_SCHED_ARRAY
	uint32_t i;
	for(i = 0;i < N_TASKS;i++)		//数组循环（省空间） or 链表遍历（省时间）
	{
		if(tcb[i].ticks > 0)
		{
			tcb[i].ticks--;
			if(tcb[i].ticks == 0)
			{
				m_set_prio(i);
			}
		}
	}
#endif
	m_time++;
#if TICK_TIME_SCHED_LIST
	m_rvs_list(&m_delayed_list);
#endif

#if TICK_TIME_SCHED_DLIST
	m_rvs_dlist(&m_delayed_dlist);
#endif

#if TICK_TIME_SCHED_IM_LIST
	m_rvs_imlist(&m_delayed_imlist);
#endif
}

#if TICK_TIME_SCHED_LIST
/**
  * @brief  将延时的任务加入到延时链表中，此函数在延时事件发生时被调用
  * @param  list: 延时链表
  * @param  tcb: 被延时任务的tcb
  * @retval 无
  */
void m_add_list(m_list* list,m_tcb* tcb)
{
	tcb->next = list->head;
	list->head = tcb;

	list->size++;
}
/**
  * @brief  如果某任务中延时ticks降到0，则此函数被调用，删除单链表中的相对应的tcb，
  * 	@note 由于是使用单链表实现，所以这里只能通过删除prev的next来实现。
  * @param  list: 延时链表
  * @param  prev: 将被删除的tcb位于prev的next
  * @retval 无
  */
void m_rmv_list(m_list* list,m_tcb* prev)
{	
	m_tcb* old;
	old = prev->next;
	if(old == NULL)
	{
		list->head = NULL;
		list->size = 0;
		return;
	}

	if(old->next != NULL)
		prev->next = old->next;
	else
		prev->next = NULL;
	
	list->size--;
	
}
/**
  * @brief  遍历延时链表，此处由于采用的是单链表，所以需要一个临时变量-prev来记录cur的前一个位置。
  * @param  list: 延时链表
  * @retval 无
  */
void m_rvs_list(m_list* list)
{
	m_tcb* cur,*prev;
	if(list->size == 0)
		return;

	for(cur = list->head,prev = list->head;cur != NULL;prev = cur,cur =cur->next)
	{
		if(cur->ticks >0)
			cur->ticks--;
		if(cur->ticks == 0)
		{
			m_rmv_list(list,prev);
			m_set_prio(cur->prio);
		}
	}
}
#endif


#if TICK_TIME_SCHED_DLIST
/**
  * @brief  将延时的任务加入到延时链表中，此函数在延时事件发生时被调用
  * 	@note 但是由于此处采用双向链表实现，故需要维护prev和next两个指针
  * @param  dlist: 延时链表
  * @param  tcb: 被延时任务的tcb
  * @retval 无
  */
void m_add_dlist(m_list* dlist,m_tcb* tcb)
{
	if (dlist->head == NULL)
	{
		dlist->head = tcb;
		tcb->next = NULL;
		tcb->prev = NULL;
	} 
	else		//添加至head的前面，再将head往前移动一个
	{
		tcb->next = dlist->head;
		dlist->head->prev = tcb;
		tcb->prev = NULL;
		dlist->head = tcb;
	}
	dlist->size++;
}
/**
  * @brief  如果某任务中延时ticks降到0，则此函数被调用，删除双向链表中的相对应的tcb。
  * @param  list: 延时链表
  * @param  cur: 将被删除的tcb指针
  * @retval 无
  */
void m_rmv_dlist(m_list* dlist,m_tcb* cur)
{
	if (dlist->size == 1)		//删除最后一个节点
	{
		cur->next = NULL;
		cur->prev = NULL;
		dlist->head = NULL;
		dlist->size = 0;
		return;
	} 

	if (cur->next == NULL)
	{
		cur->prev->next = NULL;
	} 
	else
	{
		if (cur->prev == NULL)
		{
			cur->next->prev = NULL;
			if(cur == dlist->head)
				dlist->head = cur->next;
		} 
		else
		{
			cur->prev = cur->next;
			cur->next = cur->prev;
		}
	}
	dlist->size--;
}
/**
  * @brief  遍历延时链表，此处由于采用的是双向链表，所以遍历起来比较容易。
  * @param  dlist: 延时链表
  * @retval 无
  */
void m_rvs_dlist(m_list* dlist)
{
	m_tcb* cur;
	if(dlist->size == 0)
		return;
	for (cur = dlist->head;cur != NULL;cur = cur->next)
	{
		if (cur->ticks > 0)
			cur->ticks--;
		if(cur->ticks == 0)
		{
			m_rmv_dlist(dlist,cur);
			m_set_prio(cur->prio);
		}
	}
}
#endif


#if TICK_TIME_SCHED_IM_LIST
/**
  * @brief  将延时的任务加入到延时链表中，此函数在延时事件发生时被调用
  * @param  list: 延时链表
  * @param  tcb: 被延时任务的tcb
  * @retval 无
  */
void m_add_imlist(m_list* list,m_tcb* tcb)
{
	uint32_t t_ticks=0;
	m_tcb* insert;
	if(list->size == 0)
	{
		list->head = tcb;
		tcb->next = NULL;
		list->size = 1;
		return ;
	}
	if(tcb->ticks < list->head->ticks)
	{
		tcb->next = list->head;
		list->head->ticks -= tcb->ticks;
		list->head = tcb;
	}
	else
	{
		/* 找到插入的位置，即insert的下一个位置 */
		t_ticks = list->head->ticks;
		insert = list->head;
		while (insert->next != NULL && (t_ticks += insert->next->ticks) <= tcb->ticks)
		{
			insert = insert->next;
		}
		tcb->next = insert->next;
		insert->next = tcb;
		tcb->ticks -= t_ticks;
	}
	
	list->size++;
}
/**
  * @brief  遍历延时链表，此处由于采用的是单链表，所以需要一个临时变量-prev来记录cur的前一个位置。
  * @param  list: 延时链表
  * @retval 无
  */
void m_rvs_imlist(m_list* list)
{
	if(list->size == 0)
		return;
	list->head->ticks--;
	if( list->head->ticks!= 0)
	{
		return;
	}
	else
	{
		m_set_prio(list->head->prio);
		list->head = list->head->next;
		list->size--;
	}
}
#endif

/**
  * @}
  */


/******************* (C) COPYLEFT 2013 Ted Lin *****END OF FILE****/


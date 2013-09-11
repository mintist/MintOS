#include "m_os.h"

void m_set_prio(uint32_t prio)
{
	m_prio_tbl[prio >> 5] |= (0x01 << (prio % 32 ));
}

void m_reset_prio(uint32_t prio)
{
	m_prio_tbl[prio >> 5] &= ~(0x01 << (prio % 32 ));
}

void m_get_high_prio(void)
{
	uint32_t prio = 0;
	uint32_t grp = 0;
	while(m_prio_tbl[grp] == 0)
		grp++;
	
	while((m_prio_tbl[grp] & (0x01 << prio)) == 0)
		prio++;
	m_high_prio = (grp << 5) + prio;
}

void m_create_task(m_task task, uint32_t* sp, uint32_t prio)
{
	tcb[prio].sp = m_init_sp(task,sp);
	tcb[prio].ticks = 0;
	m_set_prio(prio);
}
/* 
	任务删除函数可以删除任意状态的任务，
	只有当其删除自身（即RUNNING状态），会引起调度。
*/
void m_delete_task(uint32_t prio)
{
	m_reset_prio(prio);
	m_get_high_prio();
	if( prio == m_cur_prio)
		m_sched();
}

void m_idle_task(void* pdata)
{
	static uint32_t counter;
	while(1)
	{
		counter++;
	}
}








/**
  ******************************************************************************
  * @file    m_task.c
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

/** @defgroup Mint_OS_Task 2. 任务管理
  * @brief Mint OS任务相关函数
  */


/** @addtogroup Mint_OS_Task
  * @{
  */

#if TASK_SCHED_ARRAY
uint32_t	m_prio_tbl[N_TASKS/32+1];	/*!< 采用ucosiii的查找表调度算法，最好采用CLZ计算前导零的方式来实现，用硬件来缩短调度时间 */
#endif

#if TASK_SCHED_UCOS64				/*!< 采用ucos的查找表调度算法，最大支持64个任务 */

uint8_t            OSRdyGrp;
uint8_t            OSRdyTbl[N_TASKS/8+1];

/*!< OSMapTbl[]：就绪任务表；对应OSRdyGrp和OSRbyTbl[i]的位值(0~7) */
uint8_t  const  	OSMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
#endif

#if TASK_SCHED_UCOS128				/*!< 采用ucos的查找表调度算法，最大支持128个任务 */

uint16_t           OSRdyGrp;
uint8_t            OSRdyTbl[N_TASKS/8+1];

/*!< OSMapTbl[]：就绪任务表；对应OSRdyGrp和OSRbyTbl[i]的位值(0~7) */
uint16_t  const  	OSMapTbl[]   = { 0x0001, 0x0002, 0x0004, 0x0008,
                                0x0010, 0x0020, 0x0040, 0x0080,
                                0x0100, 0x0200, 0x0400, 0x0800,
                                0x1000, 0x2000, 0x4000, 0x8000};
#endif

#if TASK_SCHED_UCOS256									/*!< 采用ucos的查找表调度算法，最大支持256个任务 */
uint16_t            OSRdyGrp;                        	/*!<  Ready list group                         */
uint16_t            OSRdyTbl[N_TASKS/16+1];       		/*!< Table of tasks which are ready to run    */

/*!< OSMapTbl[]：就绪任务表；对应OSRdyGrp和OSRbyTbl[i]的位值(0~7) */
uint16_t  const  	OSMapTbl[]   = { 0x0001, 0x0002, 0x0004, 0x0008,
                                0x0010, 0x0020, 0x0040, 0x0080,
                                0x0100, 0x0200, 0x0400, 0x0800,
                                0x1000, 0x2000, 0x4000, 0x8000};
#endif

#if TASK_SCHED_UCOS512				/*!< 采用ucos的查找表调度算法，最大支持512个任务 */
uint8_t             OSRdyGrpX;
uint8_t             OSRdyGrpY[8]; 
uint8_t             OSRdyTbl[8][8]; 

/*!< OSMapTbl[]：就绪任务表；对应OSRdyGrp和OSRbyTbl[i]的位值(0~7) */
uint8_t  const  OSMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

#define m_x (prio >> 6)
#define m_y ((prio >> 3) & 0x07)
#define m_z (prio & 0x07)

#endif


#if TASK_SCHED_UCOS64 || TASK_SCHED_UCOS128 || TASK_SCHED_UCOS256 || TASK_SCHED_UCOS512
/*!< ******************************************************************************************************* */
/*!< 最高优先级任务查找表(PRIORITY RESOLUTION TABLE) */
/*!< 注意: 变址索引表是位模式，找出就绪态最高优先级别任务，给定值应符合高优先级位位值(0~7) */

/*!< OSUnMapTbl[]：最高优先级任务查找表；对应OSRdyGrp和OSRbyTbl[i]的位值(0~7) */
uint8_t  const  OSUnMapTbl[] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x00 to 0x0F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x10 to 0x1F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x20 to 0x2F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x30 to 0x3F                             */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x40 to 0x4F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x50 to 0x5F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x60 to 0x6F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x70 to 0x7F                             */
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x80 to 0x8F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x90 to 0x9F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xA0 to 0xAF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xB0 to 0xBF                             */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xC0 to 0xCF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xD0 to 0xDF                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xE0 to 0xEF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0        /* 0xF0 to 0xFF                             */
};
#endif

/**
  * @}
  */
  

/** @addtogroup Mint_OS_Task
  * @{
  */

/**
  * @brief  使能任务就绪表
  * @param  prio: 依据你所选择的任务调度算法指定任务优先级
  *   可选的任务调度算法:
  *     @arg TASK_SCHED_ARRAY 	1~任意，只要你内存够大
  *     @arg TASK_SCHED_UCOS64	1~62
  *     @arg TASK_SCHED_UCOS128	1~126
  *     @arg TASK_SCHED_UCOS256	1~254
  *     @arg TASK_SCHED_UCOS512	1~510
  * @retval 无
  */
void m_set_prio(uint32_t prio)
{
#if TASK_SCHED_ARRAY
	m_prio_tbl[prio >> 5] |= (0x01 << (prio % 32 ));
#endif
	
#if TASK_SCHED_UCOS64 || TASK_SCHED_UCOS128
	OSRdyGrp |= OSMapTbl[prio >> 3];
	OSRdyTbl[prio >> 3] |= OSMapTbl[prio & 0x07];
#endif
	
#if TASK_SCHED_UCOS256
	OSRdyGrp |= OSMapTbl[prio >> 4];
    OSRdyTbl[prio >>4] |= OSMapTbl[prio & 0xf];
#endif
	
#if TASK_SCHED_UCOS512	
	OSRdyGrpX |= OSMapTbl[m_x];
    OSRdyGrpY[m_x] |= OSMapTbl[m_y];
    OSRdyTbl[m_x][m_y] |= OSMapTbl[m_z];
#endif
	tcb[prio].status = READY;
}
/**
  * @brief  除能任务就续表
  * @param  prio: 你已经创建的任务的任务优先级
  * @retval 无
  */
void m_reset_prio(uint32_t prio)
{
#if TASK_SCHED_ARRAY
	m_prio_tbl[prio >> 5] &= ~(0x01 << (prio % 32 ));
#endif
	
#if TASK_SCHED_UCOS64 || TASK_SCHED_UCOS128
	if((OSRdyTbl[prio >> 3] &= ~OSMapTbl[prio & 0x07]) == 0)
		OSRdyGrp &= ~OSMapTbl[prio >> 3];
#endif
	
#if TASK_SCHED_UCOS256
	if((OSRdyTbl[prio >> 4] &= ~OSMapTbl[prio & 0x0f]) == 0)
		OSRdyGrp &= ~OSMapTbl[prio >> 4];
#endif

#if TASK_SCHED_UCOS512	
	if((OSRdyTbl[m_x][m_y] &= ~OSMapTbl[m_z]) == 0)
    {
        OSRdyGrpY[m_y] &= ~OSMapTbl[m_y];
        if((OSRdyGrpY[m_x] &= ~OSMapTbl[m_y]) == 0)
            OSRdyGrpX &= ~OSMapTbl[m_x];
    }
#endif
	tcb[prio].status = DELAYED;
}

/**
  * @brief  获取最高优先级的任务的优先级，并将其放在m_high_prio中
  * @param  无
  * @retval 无
  */
void m_get_high_prio(void)
{
#if TASK_SCHED_ARRAY
	uint32_t prio = 0;
	uint32_t grp = 0;
	while(m_prio_tbl[grp] == 0)
		grp++;
	
	while((m_prio_tbl[grp] & (0x01 << prio)) == 0)
		prio++;
	m_high_prio = (grp << 5) + prio;
#endif
	
#if TASK_SCHED_UCOS64
	uint8_t x,y;
	y = OSUnMapTbl[OSRdyGrp];
	x = OSUnMapTbl[OSRdyTbl[y]];
	m_high_prio = (y << 3) + x;
#endif

#if TASK_SCHED_UCOS128
	uint8_t x,y;
	if((OSRdyGrp & 0xff) != 0)
    {
        y    = OSUnMapTbl[OSRdyGrp & 0xff];
        x    = OSUnMapTbl[OSRdyTbl[y]];
        m_high_prio = (y << 3) + x;
    }
    else
    {
        y    = OSUnMapTbl[OSRdyGrp >> 8];
        x    = OSUnMapTbl[OSRdyTbl[y + 8]];
        m_high_prio = 64 + (y << 3) + x;
    }
#endif
	
#if TASK_SCHED_UCOS256
	uint8_t x,y;
    uint16_t* ptbl;
	if((OSRdyGrp & 0xff) != 0)
    {
        y    = OSUnMapTbl[OSRdyGrp & 0xff];
    }
    else
    {
        y    = OSUnMapTbl[OSRdyGrp >> 8] + 8;
    }

    ptbl = &OSRdyTbl[y];

    if((*ptbl & 0xff) != 0)
    {
        x = OSUnMapTbl[(*ptbl & 0xff)];
        m_high_prio = (y << 4) + x;
    }
    else
    {
        x = OSUnMapTbl[(*ptbl >> 8) & 0xff];
        m_high_prio = (y << 4) + x + 8;
    }
#endif
	
#if TASK_SCHED_UCOS512	
	uint8_t x,y,z;
	z = OSUnMapTbl[OSRdyGrpX];
    y = OSUnMapTbl[OSRdyGrpY[z]];
    x = OSUnMapTbl[OSRdyTbl[z][y]];
    m_high_prio = (z<<6)+(y<<3) + x;
#endif
	
	tcb[m_high_prio].status = RUNNING;
}
/**
  * @brief  任务创建函数
  * @param  task: 任务实体代码
  * @param  sp: 私有堆栈指针
  * @param  prio: 指定的任务优先级
  * @retval 无
  */
void m_create_task(m_task task, uint32_t* sp, uint32_t prio)
{
	tcb[prio].sp = m_init_sp(task,sp);
	tcb[prio].next = NULL;
	tcb[prio].prio = prio;
	tcb[prio].ticks = 0;
	m_set_prio(prio);
}

/**
  * @brief  任务删除函数可以删除任意状态的任务，只有当其删除自身（即RUNNING状态），会引起调度。
  * @param  prio: 将被删除的任务所具有的优先级
  * @retval 无
  */
void m_delete_task(uint32_t prio)
{
	m_reset_prio(prio);
	m_get_high_prio();
	if( prio == m_cur_prio)
		m_sched();
}
/**
  * @brief  空闲任务实体代码
  * 	@note 在这里面，可以自行设定低功耗，但是不能被删除
  * @param  pdata: 可选参数
  * @retval 无
  */
void m_idle_task(void* pdata)
{
	static uint32_t counter;
	while(1)
	{
		counter++;
	}
}


/**
  * @}
  */


/******************* (C) COPYLEFT 2013 Ted Lin *****END OF FILE****/






/**
  ******************************************************************************
  * @file    m_os_cfg.h
  * @author  Ted Lin
  * @version V0.0.0
  * @date    2013/8/29
  * @brief   此文件包含：关于Mint OS算法选择的所有宏配置选项 
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYLEFT 2013 Ted Lin</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MINT_OS_CFG_H
#define _MINT_OS_CFG_H

/* Includes ------------------------------------------------------------------*/


/** @addtogroup Mint_OS_Task
  * @brief 任务调度算法-数据结构宏选择 
  * @{
  */

#define TASK_SCHED_ARRAY		1		/*!<  采用数组作为任务就续表，支持无限个数的任务 */

#define TASK_SCHED_UCOS64		0		/*!<  ucosii查表法，最大支持64个任务 */
#define TASK_SCHED_UCOS128		0		/*!<  ucosii查表法，最大支持128个任务 */
#define TASK_SCHED_UCOS256		0		/*!<  ucosii查表法，最大支持256个任务 */
#define TASK_SCHED_UCOS512		0		/*!<  ucosii查表法，最大支持512个任务 */

#define TASK_SCHED_LIST
#define TASK_SCHED_HEAP

/**
  * @}
  */


/** @addtogroup Mint_OS_Time
  * @brief 延时链表 m_tick_time 数据结构宏选择
  * @{
  */
  
#define TICK_TIME_SCHED_ARRAY		0	/*!<  采用数组实现m_tick_time */

#define TICK_TIME_SCHED_LIST  		0	/*!<  采用单链表实现m_tick_time */
#define TICK_TIME_SCHED_DLIST  		0	/*!<  采用双向链表实现m_tick_time */
#define TICK_TIME_SCHED_IM_LIST		1	/*!<  采用改进的单链表实现m_tick_time */

#define TICK_TIME_SCHED_HASH_TABLE

/**
  * @}
  */
#endif	//END OF _M_OS_CFG_H



/******************* (C) COPYLEFT 2013 Ted Lin *****END OF FILE****/


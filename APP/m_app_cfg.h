#ifndef _M_APP_CFG_H
#define _M_APP_CFG_H

#ifdef GLOBAL_APP
#define APP
#else
#define APP extern
#endif


//�����ջ��С����
#define TASK_START_SIZE	100
#define TASK0_TCB_SIZE 	100
#define TASK1_TCB_SIZE 	100

//�����ջ����
APP uint32_t START_STK[TASK_START_SIZE];
APP uint32_t TASK0_STK[TASK0_TCB_SIZE];
APP uint32_t TASK1_STK[TASK1_TCB_SIZE];
//�������ȼ�����
#define Start_Prio 2
#define Task0_Prio 200
#define Task1_Prio 128
//������
void StartTask(void *pdata);
void Task0(void *pdata);
void Task1(void *pdata);







#endif


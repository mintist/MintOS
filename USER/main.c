#include "m_os.h"
#include "bsp.h"
#include "m_app.h"


int main(void)
{
	bsp_init();
	
	m_init();

	m_create_task(StartTask,&START_STK[TASK_START_SIZE-1],Start_Prio);
	
	m_start();
	
	return 0;
}





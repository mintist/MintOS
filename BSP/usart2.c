#include "usart2.h"
#include <stdarg.h>
/*
	stm32f10x_gpio
	stm32f10x_usart
*/

/*
 * ��������USART2_Configuration
 * ����  ��USART2 GPIO ����,����ģʽ���á�������Ϊ115200��8������λ��1��ֹͣλ����Ӳ��������
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USART2_Configuration(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;	
	
	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.2) as alternate function push-pull����������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART2 Rx (PA.3) as input floating ��������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//�ٶ���������PA.09ʱ���ã�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
										//USART_WordLength_8b; //8 λ����
										//USART_WordLength_9b; //9 λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	                                       //USART_StopBits_1  ;//��֡��β���� 1 ��ֹͣλ
										//USART_StopBits_0.5;//��֡��β���� 0.5 ��ֹͣλ
										//USART_StopBits_2  ;//��֡��β���� 2 ��ֹͣλ
										//USART_StopBits_1.5;//��֡��β���� 1.5 ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
										//USART_Parity_No  ;//��żʧ��
										//USART_Parity_Even;//żģʽ
										//USART_Parity_Odd ;//��ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
										//USART_HardwareFlowControl_None;   //Ӳ��������ʧ��
										//USART_HardwareFlowControl_RTS;    //�������� RTSʹ��
										//USART_HardwareFlowControl_CTS;    //������� CTSʹ��
										//USART_HardwareFlowControl_RTS_CTS;//RTS�� CTSʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
										//USART_Mode_Tx;//����ʹ��
										//USART_Mode_Rx;//����ʹ��
	USART_Init(USART2, &USART_InitStructure); 
	/*
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_IRQn_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  

	NVIC_Init(&NVIC_InitStructure);	
	*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
						//USART_IT_PE  //��ż�����ж�
                        //USART_IT_TXE //�����ж�
                        //USART_IT_TC  //��������ж�
                        //USART_IT_RXNE//�����ж�
                        //USART_IT_IDLE//���������ж�
                        //USART_IT_LBD //LIN�жϼ���ж�
                        //USART_IT_CTS //CTS�ж�
                        //USART_IT_ERR //�����ж�
	USART_Cmd(USART2, ENABLE);
	                    //ENABLE-DISABLE//ʹ�ܻ���ʧ�� USART ����

}

#define CMD_BUFFER_LEN 50
void USART2_printf (char *fmt, ...)
{
        char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN�����Լ������
        u8 i = 0;
        
        va_list arg_ptr;
        va_start(arg_ptr, fmt);  
        vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr);
        while ((i < CMD_BUFFER_LEN) && buffer)
        {
                USART_SendData(USART2, (u8) buffer[i++]);
                while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
        }
        va_end(arg_ptr);
}

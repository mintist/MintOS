#include "usart2.h"
#include <stdarg.h>
/*
	stm32f10x_gpio
	stm32f10x_usart
*/

/*
 * 函数名：USART2_Configuration
 * 描述  ：USART2 GPIO 配置,工作模式配置。波特率为115200，8个数据位，1个停止位，无硬件流控制
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
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
	/* Configure USART2 Tx (PA.2) as alternate function push-pull复用推挽输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART2 Rx (PA.3) as input floating 浮空输入*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//速度已在设置PA.09时设置，为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
										//USART_WordLength_8b; //8 位数据
										//USART_WordLength_9b; //9 位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	                                       //USART_StopBits_1  ;//在帧结尾传输 1 个停止位
										//USART_StopBits_0.5;//在帧结尾传输 0.5 个停止位
										//USART_StopBits_2  ;//在帧结尾传输 2 个停止位
										//USART_StopBits_1.5;//在帧结尾传输 1.5 个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
										//USART_Parity_No  ;//奇偶失能
										//USART_Parity_Even;//偶模式
										//USART_Parity_Odd ;//奇模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
										//USART_HardwareFlowControl_None;   //硬件流控制失能
										//USART_HardwareFlowControl_RTS;    //发送请求 RTS使能
										//USART_HardwareFlowControl_CTS;    //清除发送 CTS使能
										//USART_HardwareFlowControl_RTS_CTS;//RTS和 CTS使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
										//USART_Mode_Tx;//发送使能
										//USART_Mode_Rx;//接收使能
	USART_Init(USART2, &USART_InitStructure); 
	/*
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_IRQn_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  

	NVIC_Init(&NVIC_InitStructure);	
	*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
						//USART_IT_PE  //奇偶错误中断
                        //USART_IT_TXE //发送中断
                        //USART_IT_TC  //传输完成中断
                        //USART_IT_RXNE//接收中断
                        //USART_IT_IDLE//空闲总线中断
                        //USART_IT_LBD //LIN中断检测中断
                        //USART_IT_CTS //CTS中断
                        //USART_IT_ERR //错误中断
	USART_Cmd(USART2, ENABLE);
	                    //ENABLE-DISABLE//使能或者失能 USART 外设

}

#define CMD_BUFFER_LEN 50
void USART2_printf (char *fmt, ...)
{
        char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN长度自己定义吧
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

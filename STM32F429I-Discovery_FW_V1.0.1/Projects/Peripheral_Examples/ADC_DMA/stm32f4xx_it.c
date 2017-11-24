
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"
#include <stdio.h>
#include "string.h"
#include "stdlib.h"

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup ADC_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
char  *data;
/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

//Get short low level time(>2.5u)
void TIM2_IRQHandler(void){
	y=y+1;
	TIM_ClearITPendingBit(TIM2 , TIM_IT_CC2);
	TIM_ClearFlag(TIM2,TIM_FLAG_CC2);
	t2=TIM_GetCapture2(TIM2);
	LowLevel_Time1=LowLevel_Time1+t2;
}

//Get short low level time(>1u)
void TIM4_IRQHandler(void){
		z=z+1;
		TIM_ClearITPendingBit(TIM4 , TIM_IT_CC2);
		TIM_ClearFlag(TIM4,TIM_FLAG_CC2);
		t4=TIM_GetCapture2(TIM4);
		temp2=t4;
		LowLevel_Time2=LowLevel_Time2+t4;
}

//Count 30sec and display
void TIM3_IRQHandler(void){
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	x++;
	
    if(x==30)	Get_PM();		
	
		Control_Window();
}	

//Receive data & User	Control MOTOR
void USART3_IRQHandler(){
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
			char mes = USART_ReceiveData(USART3);
				if(ready==1 && mes != '\r' && mes != '\n'){
					sprintf (buff, "%s%c", buff,mes); 
				}
				else{
					memset(buff, 0, strlen(buff));					
				}
		}
		// If program set lock to true user's usart command do not work
		if(lock!=true)		Control_Motor(buff);
}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).  */
/******************************************************************************/

/**
  * @}
  */ 

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    ADC_DMA/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

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
int y,z;
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

//Get short low level time
void TIM2_IRQHandler(void){
	y=y+1;
	//z=TIM_GetCounter(TIM2);
	TIM_ClearITPendingBit(TIM2 , TIM_IT_CC1);
	TIM_ClearFlag(TIM2,TIM_FLAG_CC1);
	t1=TIM_GetCapture1(TIM2);
	t2=TIM_GetCapture2(TIM2);
	if((temp-(t2-t1))!=0){
		temp=t2-t1;
		LowLevel_Time=LowLevel_Time+t2;
	}
}

//Count 30sec and display
void TIM3_IRQHandler(void){
	char buffer[10];
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	x++;
    if(x==30){
			if (IOE_Config() == IOE_OK){
				LCD_Clear(LCD_COLOR_WHITE);
				DutyCycle=LowLevel_Time/(1000*30);
				sprintf(buffer,"1.Duty Cycle:%d%%",DutyCycle);
				LCD_DisplayStringLine(LINE(0), (uint8_t*)"Dust Detection:");
				LCD_DisplayStringLine(LINE(1), (uint8_t*)buffer);
				if(DutyCycle<=4)	LCD_DisplayStringLine(LINE(2), (uint8_t*)"2.Class:Well");
				else if(DutyCycle<=7)	LCD_DisplayStringLine(LINE(2), (uint8_t*)"2.Class:Medium");
				else if(DutyCycle<=12)	LCD_DisplayStringLine(LINE(2), (uint8_t*)"2.Class:Bad");
				else LCD_DisplayStringLine(LINE(2), (uint8_t*)"2.\"Out Of Range\"");
				x=0;
				LowLevel_Time=0;
			}
	  }
}	

void USART3_IRQHandler(){
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_SendData(USART3, USART_ReceiveData(USART3));
        while(USART_GetFlagStatus(USART3, USART_IT_TXE)==RESET);
    }
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

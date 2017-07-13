#include "main.h"
#include "stm32f429i_discovery.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f429i_discovery_ioe.h"
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "misc.h"


static void GPIOC_Config(void);
static void USART3_Config(void);

int main(void){
	GPIOC_Config();
	USART3_Config();

	while(1){
	
	}
		
}
/*/Set GPIOA for Alernate Function(AF=>PA0 Timer2_CH1)
static void GPIOC_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_0;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_AF;
//GPIO_Structure.GPIO_OType=GPIO_OType_PP;
GPIO_Structure.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOA,&GPIO_Structure);
GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
}
*/

//Set GPIOC for USART3(PC10->Tx,PC11->Rx)
static void GPIOC_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_10	|	GPIO_Pin_11;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_AF;
GPIO_Structure.GPIO_OType=GPIO_OType_PP;
GPIO_Structure.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOC,&GPIO_Structure);
GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
}


static void USART3_Config(){
	USART_InitTypeDef USART3_Structure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART3_Structure.USART_BaudRate=9600;
	USART3_Structure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART3_Structure.USART_Mode=USART_Mode_Rx	| USART_Mode_Tx;
	USART3_Structure.USART_Parity=USART_Parity_No;
	USART3_Structure.USART_StopBits=USART_StopBits_1;
	USART3_Structure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3,&USART3_Structure);
	
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
	
	USART_Cmd(USART3,ENABLE);
}

/***************************/


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

static void TIM2_Config(void);
static void TIM3_Config(void);
static void GPIOA_Config(void);
static void GPIOB_Config(void);
static void TP_Config(void);
static void USART3_Config(void);

char * Usart3_ReadLine(void);
void Usart3_Puts(char *string);

uint32_t temp,LowLevel_Time,	DutyCycle,	t1,	t2;
float average;
int x,i;
char buff [100] = "";
char t='x';	
int main(void){
	
	GPIOA_Config();
	GPIOB_Config();
	TP_Config();	//Show Data
	TIM2_Config();	//Timer2 for Dust Detection
	TIM3_Config();	//Timer3 for display each second
	USART3_Config();
	temp=0;
	LowLevel_Time=0;
	while(1){
	
	}
		
}
//Set GPIOA for Alernate Function(AF=>PA0 Timer2_CH1)
static void GPIOA_Config(void){
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
//Set GPIOB for USART3(PC10->Tx,PC11->Rx)
static void GPIOB_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_10	|	GPIO_Pin_11;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_AF;
GPIO_Structure.GPIO_OType=GPIO_OType_PP;
GPIO_Structure.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOB,&GPIO_Structure);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
}

//Timer2 for Dust Sensor
static void TIM2_Config(void){
TIM_TimeBaseInitTypeDef TIM2_Init;
TIM_ICInitTypeDef TIM_ICInitStruct;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
TIM2_Init.TIM_ClockDivision=TIM_CKD_DIV1;
TIM2_Init.TIM_Prescaler=3-1;
TIM2_Init.TIM_Period=60000-1;
TIM2_Init.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM2,&TIM2_Init);
	
TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Falling;
TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
TIM_ICInitStruct.TIM_ICFilter=0x00;
TIM_PWMIConfig(TIM2, &TIM_ICInitStruct); 
TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
TIM_ICInit(TIM2,&TIM_ICInitStruct);

NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);
TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
TIM_Cmd(TIM2,ENABLE);
}

//Interrupt each second
static void TIM3_Config(void){
TIM_TimeBaseInitTypeDef Tim_Basestructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
Tim_Basestructure.TIM_CounterMode= TIM_CounterMode_Up ;
Tim_Basestructure.TIM_ClockDivision= TIM_CKD_DIV1;
Tim_Basestructure.TIM_Period = 5000-1;
Tim_Basestructure.TIM_Prescaler = 18000-1;
Tim_Basestructure.TIM_RepetitionCounter = 0;
TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
TIM_InternalClockConfig(TIM3);
TIM_TimeBaseInit(TIM3,&Tim_Basestructure);
	
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
	
TIM_Cmd(TIM3,ENABLE);
}
//Initial LCD
static void TP_Config(void){
	LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetFont(&Font12x12);
}

static void USART3_Config(){
	USART_InitTypeDef USART3_Structure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART3_Structure.USART_BaudRate=115200;
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

/***************************
For USART3:Reading Data and Sending Data
***************************/

//Send data USART3
void Usart3_Puts(char *string){
    while(*string){
			while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){
      USART_SendData(USART3, *string);
			string++;
			}
		}
}

/*/Receive data  USART3
char * Usart3_ReadLine(){    
    while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
        t = USART_ReceiveData(USART3);
        if ((t == '\r')) {
            while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
            USART_SendData(USART3, t);
            t = '\n';
     	}
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        USART_SendData(USART3, t);
}
*/

/***************************/


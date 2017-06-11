#include "main.h"
#include "stm32f429i_discovery.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f429i_discovery_ioe.h"
#include <stdio.h>

static void TIM2_Config(void);
static void TIM4_Config(void);
static void TIM3_Config(void);
static void GPIOA_Config(void);
static void GPIOD_Config(void);
static void TP_Config(void);

uint32_t temp,	LowLevel_Time,	t1,t2;
int x;
	
int main(void){
	GPIOA_Config();
	GPIOD_Config();
	TP_Config();
	TIM2_Config();
	TIM4_Config();
	TIM3_Config();
	LowLevel_Time=0;
	
	while(1){
	
	}
		
}

static void GPIOA_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_0;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_AF;
GPIO_Structure.GPIO_OType=GPIO_OType_PP;
GPIO_Structure.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOA,&GPIO_Structure);
GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
}

static void GPIOD_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_12;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_AF;
GPIO_Structure.GPIO_OType=GPIO_OType_PP;
GPIO_Structure.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOD,&GPIO_Structure);
GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
}

static void TIM2_Config(void){

TIM_TimeBaseInitTypeDef TIM2_Init;
TIM_ICInitTypeDef TIM_ICInitStruct;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
TIM2_Init.TIM_ClockDivision=TIM_CKD_DIV1;
TIM2_Init.TIM_Prescaler=0;
TIM2_Init.TIM_Period=0xFFFF;
TIM2_Init.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM2,&TIM2_Init);
	
TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Falling;
TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
TIM_ICInitStruct.TIM_ICFilter=0x00;
//TIM_PWMIConfig(TIM2, &TIM_ICInitStruct); 
//TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
TIM_ICInit(TIM2,&TIM_ICInitStruct);

NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);
TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC3, ENABLE);
TIM_Cmd(TIM2,ENABLE);
}

static void TIM4_Config(void){

TIM_TimeBaseInitTypeDef TIM4_Init;
TIM_ICInitTypeDef TIM_ICInitStruct;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
TIM4_Init.TIM_ClockDivision=TIM_CKD_DIV1;
TIM4_Init.TIM_Prescaler=5000;
TIM4_Init.TIM_Period=0xFFFF;
TIM4_Init.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM4,&TIM4_Init);
	
TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
TIM_ICInitStruct.TIM_ICFilter=0x00;
//TIM_PWMIConfig(TIM3, &TIM_ICInitStruct); 
//TIM_SelectInputTrigger(TIM3, TIM_TS_TI2FP2);
TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);
TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
TIM_Cmd(TIM4,ENABLE);
}

static void TIM3_Config(void){
TIM_TimeBaseInitTypeDef Tim_Basestructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
Tim_Basestructure.TIM_CounterMode= TIM_CounterMode_Up ;
Tim_Basestructure.TIM_ClockDivision= TIM_CKD_DIV1;
Tim_Basestructure.TIM_Period = 5000-1;
Tim_Basestructure.TIM_Prescaler = 14400-1;
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
static void TP_Config(void)
{
	LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetFont(&Font16x24);
}


#include "main.h"
#include "stm32f429i_discovery.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f429i_discovery_ioe.h"
#include <stdio.h>

static void TIM1_Config(void);
static void TIM3_Config(void);
static void GPIOE_Config(void);
static void TP_Config(void);

uint32_t temp,	LowLevel_Time,	CCR1,	CCR2;
int x;
	
int main(void){
	GPIOE_Config();
	TP_Config();
	TIM1_Config();
	TIM3_Config();
	
	temp=0;
	x=0;
	LowLevel_Time=0;
	
	while(1){
	
	}
		
}

static void GPIOE_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_11;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_AF;
//GPIO_Structure.GPIO_OType=GPIO_OType_PP;
GPIO_Structure.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOE,&GPIO_Structure);
GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
}

static void TIM1_Config(void){

TIM_TimeBaseInitTypeDef TIM1_Init;
TIM_ICInitTypeDef TIM_ICInitStruct;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
TIM1_Init.TIM_ClockDivision=TIM_CKD_DIV1;
TIM1_Init.TIM_Prescaler=0;
TIM1_Init.TIM_Period=0xFFFF;
TIM1_Init.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM1,&TIM1_Init);
	
TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;
TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
TIM_ICInitStruct.TIM_ICFilter=0x00;
//TIM_PWMIConfig(TIM1, &TIM_ICInitStruct); 
//TIM_SelectInputTrigger(TIM1, TIM_TS_TI2FP2);
//TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);
//TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);
TIM_ICInit(TIM1,&TIM_ICInitStruct);

//TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

NVIC_InitStructure.NVIC_IRQChannel=TIM1_CC_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);

TIM_Cmd(TIM1,ENABLE);
TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
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
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
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


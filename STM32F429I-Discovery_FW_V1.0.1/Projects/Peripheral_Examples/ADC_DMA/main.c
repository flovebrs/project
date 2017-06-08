#include "main.h"
#include "stm32f429i_discovery.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
//static void USART_Config(void);
static void TIM2_Config(void);
//static void TIM3_Config(void);
//static void TP_Config(void);
static void GPIOA_Config(void);
static void GPIOG_Config(void);
static void EXTI0_Config(void);
//static void EXTI1_Config(void);
static void TP_Config(void);
uint32_t temp,t_flag;

int main(void){
	uint32_t LowLevel_Time;
	GPIOA_Config();
	GPIOG_Config();
	TP_Config();
	//USART_Config();
	EXTI0_Config();
	//EXTI1_Config();
	TIM2_Config();
	//TIM3_Config();
	STM_EVAL_LEDInit(LED3);
	temp=0;
	while(1){
	if(t_flag==1){
	LowLevel_Time=LowLevel_Time+temp;
	}
	}
}

/*
static void TP_Config(void)
{
	LCD_Init();
  LCD_LayerInit();  
  LTDC_Cmd(ENABLE);
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_Clear(LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);
}
*/

static void GPIOA_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_1;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_IN;
GPIO_Structure.GPIO_OType=GPIO_OType_PP;
GPIO_Structure.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOA,&GPIO_Structure);
}

static void GPIOG_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_OUT;
GPIO_Structure.GPIO_OType=GPIO_OType_PP;
GPIO_Structure.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOG,&GPIO_Structure);
}

/*
static void USART_Config(void){
	
USART_InitTypeDef	USART_Structure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//USART_Cmd(USART2,ENABLE);
USART_Structure.USART_BaudRate=115200;
USART_Structure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
USART_Structure.USART_Parity=USART_Parity_No;
USART_Structure.USART_StopBits=USART_StopBits_1;
USART_Structure.USART_WordLength=USART_WordLength_8b;
USART_Structure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
USART_Init(USART1,&USART_Structure);
USART_Cmd(USART1,ENABLE);
}
*/
/*
static void TIM3_Config(void){

TIM_TimeBaseInitTypeDef TIM3_Init;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
TIM3_Init.TIM_ClockDivision=TIM_CKD_DIV1;
TIM3_Init.TIM_Prescaler=18000-1;	
TIM3_Init.TIM_Period=5000;
TIM3_Init.TIM_CounterMode=TIM_CounterMode_Up;
//TIM_Cmd(TIM3, ENABLE);
TIM_TimeBaseInit(TIM3,&TIM3_Init);
TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
NVIC_Init(&NVIC_InitStructure);

TIM_Cmd(TIM3,ENABLE);
}
*/
static void TIM2_Config(void){

TIM_TimeBaseInitTypeDef TIM2_Init;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
TIM2_Init.TIM_ClockDivision=TIM_CKD_DIV1;
TIM2_Init.TIM_Prescaler=18000-1;
TIM2_Init.TIM_Period=2500;
TIM2_Init.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM2,&TIM2_Init);
TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
NVIC_Init(&NVIC_InitStructure);

TIM_Cmd(TIM2,ENABLE);
}
	

static void EXTI0_Config(void){
EXTI_InitTypeDef	EXTI_InitStructure;
NVIC_InitTypeDef	NVIC_InitStructure;
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,GPIO_PinSource0);
EXTI_InitStructure.EXTI_Line=EXTI_Line0;
EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd=ENABLE;
EXTI_Init(&EXTI_InitStructure);
	
NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);
}
/*
static void EXTI1_Config(void){
EXTI_InitTypeDef	EXTI_InitStructure;
NVIC_InitTypeDef	NVIC_InitStructure;
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,GPIO_PinSource0);
EXTI_InitStructure.EXTI_Line=EXTI_Line1;
EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd=ENABLE;
EXTI_Init(&EXTI_InitStructure);
	
NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);
}
*/
static void TP_Config(void)
{
	LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetFont(&Font16x24);
}


#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_ltdc.h"
#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"

static void CLK_config(void);
static void NVIC_config(void);
static void GPIO_config(void);
static void TP_config(void);
int i=-1;

int main(void){
	CLK_config();
	NVIC_config();
	GPIO_config();
  TP_config();
	
	
	while(1){
			
		}
   
}
	
	

static void CLK_config(void){
	TIM_TimeBaseInitTypeDef Tim_Basestructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	Tim_Basestructure.TIM_CounterMode= TIM_CounterMode_Up ;
	Tim_Basestructure.TIM_ClockDivision= TIM_CKD_DIV1;
	Tim_Basestructure.TIM_Period = 5000-1;
	Tim_Basestructure.TIM_Prescaler = 14400-1;
	Tim_Basestructure.TIM_RepetitionCounter = 0;
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInit(TIM2,&Tim_Basestructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

static void NVIC_config(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

static void GPIO_config(void){
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
}

static void TP_config(void){
	
	
  LCD_Init();
  
  LCD_LayerInit();
    
  LTDC_Cmd(ENABLE);
  
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  
	LCD_Clear(LCD_COLOR_WHITE);
	
}

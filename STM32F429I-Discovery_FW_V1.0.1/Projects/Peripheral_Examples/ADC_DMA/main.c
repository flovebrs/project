/***************************************
HARDWARE CONNECTION
Dust Dection
*Using Dust Sensor PPD42NJ.	The outputs are:
	P1:	Particle larger than 1um	->PA1
	P2:	Particle larger than 2.5um	->PD13
*Using Motor:
	GPIOE is setting for Timer9's AFIO
	Timer9 is setting for output PWM signal
*Part of transmission
	GPIOC is setting for USART3
	TX->PC10	RX->PC11
------------------------------------------------
*Part of calculate
	Timer3 interrupt per second
*Part of Sensor:
	Timer4 is setting for P1's calculate voltage low level time
	Timer2 is setting for P2's calculate voltage low level time
*Window position:
	Window_position=0 ->window closed
	Window_position=2 ->window opened(max)
****************************************/

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
#include "stdbool.h"

static void TIM2_Config(void);		//Timer2 for Dust Detection(>1u)
static void TIM3_Config(void);		//Timer3 for display each second
static void TIM4_Config(void);		//Timer4 for Dust Detection(>2.5u)
static void TIM9_Config(void);		//Timer9 for PWM output

static void GPIOA_Config(void);
static void GPIOC_Config(void);
static void GPIOD_Config(void);
static void GPIOE_Config(void);

//static void TP_Config(void);

static void USART3_Config(void);
void USART3_Send(char* string);

void SetPWM(int x,FunctionalState STATE);		//Setting spec of PWM
void Control_Motor(char* buff);
void Control_Window(void);
void Get_PM(void);

void Delay(void);

float  temp1, LowLevel_Time1, t1, t2;	//for >1u
float  temp2, LowLevel_Time2, t3, t4;	//for >2.5u
float DutyCycle, LowLevel_Time;	
int x,y,z,i,k,q;
int ready;
int window_pos;
int window_tar;
bool window_con,lock,shut;
char te[100];
char m[100];
char buff[100]="";
int times[4];

int main(void){
	GPIOC_Config();
	GPIOA_Config();
	GPIOD_Config();
	GPIOE_Config();
	//TP_Config();	//Show Data on LCD
	USART3_Config();
	TIM9_Config();
	TIM2_Config();
	TIM3_Config();
	TIM4_Config();
	
	//Initialize parameter
	ready=0;
	temp1=temp2=0;
	LowLevel_Time1=LowLevel_Time2=0;
	//Window closed deafault
	window_pos=0;
	q=0;
	USART3_Send("\r\n");
	Delay();
	USART3_Send("AT+CIPMUX=1\r\n");
	Delay();
	USART3_Send("AT+CIPSERVER=1\r\n");
	Delay();
	ready=1;
	while(1){}
}

//Set GPIOA for Alernate Function(PA1 Timer2_CH2)
static void GPIOA_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_AF;
GPIO_Structure.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOA,&GPIO_Structure);
GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
}

//Set GPIOD for Alernate Function(PD13 Timer4_CH2)
static void GPIOD_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_AF;
GPIO_Structure.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOD,&GPIO_Structure);
GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
}

//Set GPIOE for Alernate Function(PE5 Output PWM)
static void GPIOE_Config(void){
GPIO_InitTypeDef GPIO_Structure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
GPIO_Structure.GPIO_Pin=GPIO_Pin_5;
GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Structure.GPIO_Mode=GPIO_Mode_AF;
GPIO_Structure.GPIO_OType=GPIO_OType_PP;
GPIO_Structure.GPIO_PuPd= GPIO_PuPd_UP;
GPIO_Init(GPIOE,&GPIO_Structure);
GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);
}

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
GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
}

//Timer2 for Dust Sensor(>2.5u)->Falling Edge Trigger &	CC2 interrupt
static void TIM2_Config(void){
TIM_TimeBaseInitTypeDef TIM2_Init;
TIM_ICInitTypeDef TIM_ICInitStruct;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);	
	
TIM2_Init.TIM_ClockDivision=TIM_CKD_DIV1;
TIM2_Init.TIM_Prescaler=3-1;
TIM2_Init.TIM_Period=60000-1;
TIM2_Init.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM2,&TIM2_Init);
	
TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Falling;
TIM_ICInitStruct.TIM_Channel=TIM_Channel_1 | TIM_Channel_2;	//CH1->Rising(not used) , CH2->FALLING(used) 
TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
TIM_ICInitStruct.TIM_ICFilter=0x00;
TIM_PWMIConfig(TIM2, &TIM_ICInitStruct); 

TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
TIM_ICInit(TIM2,&TIM_ICInitStruct);

NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);
TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
TIM_Cmd(TIM2,ENABLE);
}

//Timer4 for Dust Sensor (>1u)->Falling Edge Trigger	 & 	CC2 interrupt
static void TIM4_Config(void){
TIM_TimeBaseInitTypeDef TIM4_Init;
TIM_ICInitTypeDef TIM_ICInitStruct;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);	

TIM4_Init.TIM_ClockDivision=TIM_CKD_DIV1;
TIM4_Init.TIM_Prescaler=3-1;
TIM4_Init.TIM_Period=60000-1;
TIM4_Init.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM4,&TIM4_Init);
	
TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Falling;
TIM_ICInitStruct.TIM_Channel=TIM_Channel_1 | TIM_Channel_2;	//CH1->Rising(not used) , CH2->FALLING(used) 
TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
TIM_ICInitStruct.TIM_ICFilter=0x00;
TIM_PWMIConfig(TIM4, &TIM_ICInitStruct); 	

TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);
TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
TIM_ICInit(TIM4,&TIM_ICInitStruct);

NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);
TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
TIM_Cmd(TIM4,ENABLE);
}

//Timer9 for generate PWM
static void TIM9_Config(void){
	TIM_TimeBaseInitTypeDef TIM9_Init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);
	TIM9_Init.TIM_CounterMode=TIM_CounterMode_Up;
	TIM9_Init.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM9_Init.TIM_Period=100-1;
	TIM9_Init.TIM_Prescaler=36000;
	TIM_TimeBaseInit(TIM9, &TIM9_Init);
	//TIM_CtrlPWMOutputs(TIM9, ENABLE);	use in tim1,tim8
	TIM_Cmd(TIM9, ENABLE);
}

//Timer3 for Interrupt each second
static void TIM3_Config(void){
TIM_TimeBaseInitTypeDef Tim_Basestructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
Tim_Basestructure.TIM_CounterMode= TIM_CounterMode_Up ;
Tim_Basestructure.TIM_ClockDivision= TIM_CKD_DIV1;
RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);
Tim_Basestructure.TIM_Period = 10000-1;
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

/*
//Initial LCD
static void TP_Config(void){
	LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetFont(&Font12x12);
}
*/

//Setting USART3
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
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	USART_Cmd(USART3,ENABLE);
	NVIC_EnableIRQ(USART3_IRQn);
}


//Calculate the density of pm2.5 &		Send
void Get_PM(){
		LowLevel_Time=LowLevel_Time2-LowLevel_Time1;
		DutyCycle=LowLevel_Time/(1000*42);
	sprintf(te,"PM:%.2f	>1u:%.2f	>2.5u:%.2f	Win_Pos:%d",DutyCycle,LowLevel_Time2/42,LowLevel_Time1/42,window_pos);	//78
		k=strlen(te);
		sprintf (m,"AT+CIPSEND=0,%d\r\n",k);
		USART3_Send(m);
		Delay();
		USART3_Send(te);
		Delay();
		memset(te, 0, strlen(te));
		memset(m, 0, strlen(m));			
		x=0;
		LowLevel_Time1=LowLevel_Time2=0;
		
		//Program priority is greater than user usart command
		if(DutyCycle>3)	times[k]=1;
		else if(DutyCycle<1)	times[k]=0;
		k++;	
		//If greater than standard 3 times in 2 minute close window
		if(k==4)	k=0;
		if((times[0]+times[1]+times[2]+times[3])>=3){
			lock=true;
			window_con=true;
			window_tar=2;
		}
		if((times[0]+times[1]+times[2]+times[3])<=1){
			lock=true;
			window_con=true;
			window_tar=0;
		}
}


//Setting spec of PWM
void SetPWM(int x, FunctionalState STATE){
	TIM_OCInitTypeDef Tim_OCInitStructure;
	if(STATE==RESET)	TIM_Cmd(TIM9, DISABLE);
	else{
	Tim_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  Tim_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	Tim_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	Tim_OCInitStructure.TIM_Pulse =x;	
	TIM_OC1Init(TIM9, &Tim_OCInitStructure);
  TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM9, ENABLE);
		
	TIM_Cmd(TIM9, ENABLE);
	}
}

//Control SERVO MOTOR
void Control_Motor(char* buff){
		//Close Window
		if(strcmp(buff,"+IPD,0,8:Window50")==0){
			window_con=true;
			window_tar=0;
		}
		if(strcmp(buff,"+IPD,0,8:Window75")==0){
			window_con=true;
			window_tar=1;
		}
		//Open Window
		if(strcmp(buff,"+IPD,0,9:Window100")==0){
			window_con=true;
			window_tar=2;
		}
		//Stop motor
		if(strcmp(buff,"+IPD,0,4:STOP")==0){
			SetPWM(7, DISABLE);
		}
		//Shut to forbid program control
		if(strcmp(buff,"+IPD,0,4:SHUT")==0){
			shut=true;
		}
		
		if(strcmp(buff,"+IPD,0,4:OPEN")==0){
			shut=false;
		}
}

//Control window position
void Control_Window(){
	if(window_con==true	&& shut!=true){
			if(window_tar!=window_pos){
				if(window_tar>window_pos){
					SetPWM(6,ENABLE);
					q++;
					//Motor run 5 seconds each window_position
					if(q%5==0)		window_pos++;
				}
				if(window_tar<window_pos){
					SetPWM(8,ENABLE);
					q++;
					if(q%5==0)		window_pos--;
				}
			}
			if(window_tar==window_pos){
				window_con=false;
				SetPWM(7,DISABLE);
				lock=false;
				q=0;
			}
	}
	else	{
		SetPWM(7,DISABLE);
		lock=false;
	}
}

//Send data USART3
void USART3_Send(char* string){
    while(*string){
        USART_SendData(USART3, *string++);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
}

void Delay(){
	for(i=0;i<100000000;i++);
}

/**************************************/



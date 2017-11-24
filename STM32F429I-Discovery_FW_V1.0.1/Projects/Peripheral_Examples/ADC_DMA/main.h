/**
  ******************************************************************************
  * @file    ADC_DMA/main.h 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
/* Private define ------------------------------------------------------------*/
/* used to display the ADC converted value on LCD */
#define USE_LCD
  /* if you are not using the LCD, you can monitor the converted values by adding
     the variable "ADC3ConvertedValue" to the debugger watch window */
     
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern float temp1, LowLevel_Time1, t1, t2;
extern float temp2, LowLevel_Time2, t3, t4;
extern float DutyCycle, LowLevel_Time;
extern int x,y,z,k,q;
extern int ready;
extern bool window_con,lock;
extern int window_pos;
extern int window_tar;
extern char buff[100];
extern char m[100];
extern char te[100];

/* Exported macro ------------------------------------------------------------*/ 
/* Exported functions ------------------------------------------------------- */
extern void USART3_Send(char* string);
extern void Get_PM(void);
extern void SetPWM(int x,FunctionalState STATE);
extern void	Control_Motor(char* buff);
void Control_Window(void);
extern void Delay(void);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

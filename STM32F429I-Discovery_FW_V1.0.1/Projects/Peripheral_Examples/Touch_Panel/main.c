#include "main.h"
int t1;
int main(void)
{
	t1=0;
    
  while (1){
		if(TIM_GetITStatus(TIM2,TIM_FLAG_Update)==SET)	t1=t1+1;
		if(EXTI_GetFlagStatus(EXTI_Line0)!=SET)	t1=0;
		
}

}

void assert_failed(uint8_t* file, uint32_t line)
{
  while (1)
  {
  }
}


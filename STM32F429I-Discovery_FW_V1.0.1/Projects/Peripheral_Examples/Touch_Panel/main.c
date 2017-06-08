
#include "main.h"

static void TP_Config(void);

int main(void)
{
	int t=0;
	char x[10];
  TP_Config();
    
  while (1)
  {
		t=t+1;
		sprintf(x,"%d",t);
		LCD_DisplayStringLine(LINE(0), (uint8_t *)x);
	}
}

static void TP_Config(void)
{
	LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetFont(&Font8x8);
}

void assert_failed(uint8_t* file, uint32_t line)
{
  while (1)
  {
  }
}



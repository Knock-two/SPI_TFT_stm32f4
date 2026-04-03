#include "stm32f4xx.h"
#include <stdbool.h>
#include "tft.h"
#include "led.h"
#include "delay.h"
#include "font.h"

int main()
{   
		TFT_Init();
		led_init();
		

		TFT_ClearScreen(0xFFFF);
		led_on(1);
		delay_ms(1000);
		led_off(1);
		delay_ms(1000);
		led_on(1);
	
		TFT_FillBlock(0,50,0,50,0x07E0);
		TFT_DrawChar_16x16(0, 0, Char16x16[0], 0x0000, 0xFFFF);
    
    while(1)
    {
    }

}



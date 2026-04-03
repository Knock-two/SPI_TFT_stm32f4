#include "stm32f4xx.h"

static uint32_t g_us_per_tick = 0;

void delay_us(uint32_t us)
{
		static uint8_t is_init = 0;
    if(is_init == 0)
    {
				g_us_per_tick = SystemCoreClock / 1000000;
				SysTick->LOAD = 0xFFFFFF;
        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk; 
        SysTick->CTRL |=  SysTick_CTRL_CLKSOURCE_Msk;
				SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
        is_init = 1;
    }
		
    if(us == 0) return;
    
    uint32_t start_tick = SysTick->VAL;
    uint32_t ticks_needed = us * g_us_per_tick;
    
    while((start_tick - SysTick->VAL) < ticks_needed)
    {
        if(SysTick->VAL > start_tick)
        {
            ticks_needed -= (start_tick + (0xFFFFFF - SysTick->VAL));
            start_tick = SysTick->VAL;
        }
    }
}


void delay_ms(uint32_t ms)
{
    while(ms--)
    {
        delay_us(1000); 
    }
}

void delay_s(uint32_t s)
{
	while(s--)
	{
		delay_ms(1000);
	}
} 


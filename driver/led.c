#include "stm32f4xx.h"
#include <stdbool.h>
#include "delay.h"

#define LED1_PORT GPIOE
#define LED1_PIN GPIO_Pin_15
#define LED2_PORT GPIOE
#define LED2_PIN GPIO_Pin_13
#define LED3_PORT GPIOE
#define LED3_PIN GPIO_Pin_11

void led_init(void)
{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_StructInit(&GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void led_set(uint8_t idx,bool or)
{
		switch(idx)
		{
				case 1:
						GPIO_WriteBit(LED1_PORT,LED1_PIN,or ? Bit_SET : Bit_RESET);
						break;
				case 2:
						GPIO_WriteBit(LED2_PORT,LED2_PIN,or ? Bit_SET : Bit_RESET);
						break;
				case 3:
						GPIO_WriteBit(LED3_PORT,LED3_PIN,or ? Bit_SET : Bit_RESET);
						break;
				default:
						break;
		}
}

void led_on(uint8_t idx)
{
		led_set(idx,true);
}

void led_off(uint8_t idx)
{
		led_set(idx,false);
}

void led_all_off(void)
{
		led_set(1,false);
		led_set(2,false);
		led_set(3,false);
}

void led_breath( uint32_t d , uint32_t period , int led_id )
{
				led_on(led_id);
				delay_ms(d);
				led_off(led_id);
				delay_ms(period - d);
}



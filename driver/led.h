#ifndef __LED_H
#define __LED_H
#include <stdbool.h>
#include <stdint.h>

void led_init(void);
void led_set(uint8_t idx,bool or);

void led_on(uint8_t idx);
void led_off(uint8_t idx);
void led_all_off(void);

void led_breath( uint32_t d , uint32_t period , int led_id );

#endif

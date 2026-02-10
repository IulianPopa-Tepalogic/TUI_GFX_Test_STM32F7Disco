/*
 * tos_clocks_init_stage.c
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#include "tos.h"
#include "stm32f746xx.h"
#include "stm32f7xx_hal.h"
#include "core_cm7.h"

extern void stm_setup_clocks(void);


void __tos_clocks_init_stage_hook(void)
{
	stm_setup_clocks();

    SysTick->CTRL = 0;
    SysTick->LOAD = 27000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 3; // ENABLE=1, TICKINT=1, CLKSOURCE=0 (HCLK/8)

    tos_set_ticks_per_second(1000);
    HAL_SetTickFreq(HAL_TICK_FREQ_1KHZ);
}


void __tos_clocks_tick_callback()
{
	HAL_IncTick();
}

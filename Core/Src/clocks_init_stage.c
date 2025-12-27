/*
 * tos_clocks_init_stage.c
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#include "tos.h"
#include "core_cm7.h"

extern stm_setup_clocks(void);

void __tos_clocks_init_stage_hook(void)
{
	stm_setup_clocks(void);

    SysTick->CTRL = 0;
    SysTick->LOAD = 2700000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 0; // ENABLE=1, TICKINT=1, CLKSOURCE=0 (HCLK/8)

    tos_set_ticks_per_second(1);
}

void __tos_clocks_tick_callback()
{
	/* NOTE:
	 * This functions does nothing for now!
	 */
}

/*
 * final_init_stage.c
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#include "tos.h"
#include "stm32f746xx.h"

void __tos_final_init_stage_hook()
{
	tos_enable_irq(LTDC_IRQn, 0xEE);
}




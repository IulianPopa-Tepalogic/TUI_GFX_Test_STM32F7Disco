/*
 * irqs_setup_stage.c
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#include "tos.h"

void __tos_irqs_setup_stage_hook()
{
	for (int i = 16; i < 256; ++i)
		tos_disable_irq(i);

	//Enable TIM6 IRQ with lowest priority because this time
	//it is used STM32 HAL functions.
	tos_enable_irq(54, 0xFF);
}

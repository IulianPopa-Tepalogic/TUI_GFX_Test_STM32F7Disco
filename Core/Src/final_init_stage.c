/*
 * final_init_stage.c
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

extern void stm_main(void);

void __tos_final_init_stage_hook()
{
	stm_main();
}




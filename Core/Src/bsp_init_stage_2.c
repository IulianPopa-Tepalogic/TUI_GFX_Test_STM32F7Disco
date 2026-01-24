/*
 * bsp_init_stage_1.c
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#include "stm32f746xx.h"
#include "stm32f7xx_hal.h"

extern SDRAM_HandleTypeDef hsdram1;

extern void stm_main(void);

static void MPU_Config_SDRAM_WBWA(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  HAL_MPU_Disable();

  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress      = 0xC0000000;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
  FMC_SDRAM_CommandTypeDef Command;

  /* Step 1: Clock enable command */
  Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;
  HAL_SDRAM_SendCommand(hsdram, &Command, HAL_MAX_DELAY);

  HAL_Delay(1); // >= 100us

  /* Step 2: Precharge all command */
  Command.CommandMode = FMC_SDRAM_CMD_PALL;
  Command.AutoRefreshNumber = 1;
  HAL_SDRAM_SendCommand(hsdram, &Command, HAL_MAX_DELAY);

  /* Step 3: Auto-refresh command */
  Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.AutoRefreshNumber = 8;
  HAL_SDRAM_SendCommand(hsdram, &Command, HAL_MAX_DELAY);

  /* Step 4: Load Mode Register */
  // Mode register fields depend on SDRAM:
  // burst length=1, burst type=sequential, CAS=3, operating mode=standard, write burst=single
  uint32_t modeReg =
      0
      | (0x0 << 0)   // Burst Length = 1
      | (0x0 << 3)   // Burst Type = Sequential
      | (0x3 << 4)   // CAS Latency = 3
      | (0x0 << 7)   // Operating Mode = Standard
      | (0x1 << 9);  // Write Burst = Single

  Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = modeReg;
  HAL_SDRAM_SendCommand(hsdram, &Command, HAL_MAX_DELAY);

  /* Step 5: Set the refresh rate counter */
  // RefreshRate = (SDCLK_Hz * 64ms / 4096) - margin
  // Margin is often ~20 cycles; board examples use a fixed value.
  // Compute based on your SDCLK!
  uint32_t refresh = 0; // <-- compute properly for your clock
  HAL_SDRAM_ProgramRefreshRate(hsdram, refresh);
}

void __tos_bsp_init_stage_2_hook()
{
	MPU_Config_SDRAM_WBWA();
	SCB_EnableICache();
	SCB_EnableDCache();

	stm_main();
	SDRAM_Initialization_Sequence(&hsdram1);
}




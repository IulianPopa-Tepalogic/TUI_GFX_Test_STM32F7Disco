#include <tui_gui_base.h>

#include <utility>
#include <cstring>

#include "tos.h"

#include "stm32f746xx.h"

static uint32_t SCREEN_MEM_SIZE = (SCREEN_HEIGHT_SIZE * SCREEN_WIDTH_SIZE * Pixel::BITS_PER_PIXEL / 8);
static uint8_t* _mainBuffer = reinterpret_cast<uint8_t*>(0xC0000000);
static uint8_t* _shadowBuffer = _mainBuffer + SCREEN_MEM_SIZE;

static constexpr uint32_t DCACHE_LINE_SIZE = 32;
static volatile int sgProcessToVSync = -1;

static void DCache_CleanByAddr32(const void *addr, uint32_t size)
{
  uintptr_t start = (uintptr_t)addr & ~(uintptr_t)(DCACHE_LINE_SIZE - 1U);
  uintptr_t end   = ((uintptr_t)addr + size + (DCACHE_LINE_SIZE - 1U)) & ~(uintptr_t)(DCACHE_LINE_SIZE - 1U);
  SCB_CleanDCache_by_Addr((uint32_t*)start, (int32_t)(end - start));
}

void* __tui_get_draw_buffer()
{
	return _mainBuffer;
}

void* __tui_get_shadow_buffer()
{
	return _shadowBuffer;
}

void __tui_commit_shadow_buffer()
{
	DCache_CleanByAddr32(_shadowBuffer,  SCREEN_MEM_SIZE);

	LTDC->ICR = LTDC_ICR_CRRIF;
	LTDC->IER |= LTDC_IER_RRIE | LTDC_IER_FUIE;

	const auto addr = reinterpret_cast<uint32_t>(_shadowBuffer);
	LTDC_Layer1->CFBAR = addr;
	__DSB();
	LTDC->SRCR = LTDC_SRCR_VBR;
    __DSB();

	sgProcessToVSync = tos_cpid();
	__DSB();
	tos_freeze_pid(sgProcessToVSync); //it will be waked up from IRQ

	std::swap(_mainBuffer, _shadowBuffer);
}

void __tui_warning(const char* s)
{
}

void __tui_error(const char* s)
{
	tos_critical_fault();
}


extern "C"
{

void tui_vsync_irq_handler()
{
    if (LTDC->ISR & LTDC_ISR_RRIF)
    {
    	if (LTDC->IER & LTDC_IER_RRIE)
		{
			LTDC->ICR = LTDC_ICR_CRRIF;

			if (sgProcessToVSync > 0)
			{
				tos_thaw_pid(sgProcessToVSync);
				sgProcessToVSync = -1;
				LTDC->IER &= ~LTDC_IER_RRIE;
			}
		}
    }

	tos_ack_irq(LTDC_IRQn);
}

}

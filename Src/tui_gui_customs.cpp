#include <tui_gui_base.h>

#include <utility>

#include "tos.h"

#include "stm32f746xx.h"

static uint32_t SCREEN_MEM_SIZE = (SCREEN_HEIGHT_SIZE * SCREEN_WIDTH_SIZE * Pixel::BITS_PER_PIXEL / 8);
static uint8_t* _mainBuffer = reinterpret_cast<uint8_t*>(0xC0000000);
static uint8_t* _shadowBuffer = _mainBuffer + SCREEN_MEM_SIZE;

static constexpr uint32_t DCACHE_LINE_SIZE = 32;

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

	const auto addr = reinterpret_cast<uint32_t>(_shadowBuffer);
	LTDC_Layer1->CFBAR = addr;
	LTDC->SRCR = LTDC_SRCR_VBR;

	std::swap(_mainBuffer, _shadowBuffer);
}

void __tui_warning(const char* s)
{

}

void __tui_error(const char* s)
{
	tos_critical_fault();
}

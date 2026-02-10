#pragma once

#include <cstdint>

#include "tos.h"

#if defined(USED_COLOR_ENCODING_RGB888) && (USED_COLOR_ENCODING_RGB888 == 1)
#define USED_COLOR_ENCODING RGB888

#elif defined(USED_COLOR_ENCODING_RGB565) && (USED_COLOR_ENCODING_RGB565 == 1)
	#define USED_COLOR_ENCODING RGB565

#elif(USED_COLOR_ENCODING_ARGB8888) && (USED_COLOR_ENCODING_ARGB8888 == 1)
  #define USED_COLOR_ENCODING ARGB8888
#endif

#ifndef USED_COLOR_ENCODING
	#error "Please define the color encoding to be used by defining the macro USED_COLOR_ENCODING to one of the following: ARGB8888, RGB888, RGB565"
#endif

enum PIXEL_COLOR_ENCODINGS {
	UNDEFINED = 0,
	ARGB8888,
	RGB888,
	RGB565,
};

struct alignas(4) Pixel_ARGB8888 {
	constexpr Pixel_ARGB8888()
		:  blue(0), green(0), red(0), alpha(0xFF)
	{}

	constexpr Pixel_ARGB8888 (uint8_t r, uint8_t g, uint8_t b)
		: blue(b), green(g), red(r), alpha(0xFF)
	{}

	constexpr Pixel_ARGB8888 (const Pixel_ARGB8888& p)
		: blue(p.blue), green(p.green), red(p.red), alpha(p.alpha)
	{}

	bool operator==(const Pixel_ARGB8888& p) const { return (blue == p.blue) & (green == p.green) & (red == p.red) & (alpha == p.alpha); }
	bool operator!=(const Pixel_ARGB8888& p) const { return !(*this == p); }

	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t alpha;

	static constexpr unsigned int BITS_PER_PIXEL = 32;
};


struct alignas(1) Pixel_RGB888 {
	constexpr Pixel_RGB888()
		: blue(0), green(0), red(0)
	{}

	constexpr Pixel_RGB888 (uint8_t r, uint8_t g, uint8_t b)
		: blue(b), green(g), red(r)
	{}

	constexpr Pixel_RGB888 (const Pixel_RGB888& p)
		: blue(p.blue), green(p.green), red(p.red)
	{}

	bool operator==(const Pixel_RGB888& p) const { return (blue == p.blue) & (green == p.green) & (red == p.red); }
	bool operator!=(const Pixel_RGB888& p) const { return !(*this == p); }

	uint8_t blue;
	uint8_t green;
	uint8_t red;

	static constexpr uint8_t alpha = 0xFF;
	static constexpr unsigned int BITS_PER_PIXEL = 24;
};

struct alignas(2) Pixel_RGB565 {
	constexpr Pixel_RGB565()
		: blue(0), green(0), red(0)
	{}

	constexpr Pixel_RGB565 (uint8_t r, uint8_t g, uint8_t b)
		: blue(b), green(g), red(r)
	{}

	bool operator==(const Pixel_RGB565& p) const { return *((uint16_t*)this) == *((uint16_t*)&p); }
	bool operator!=(const Pixel_RGB565& p) const { return !(*this == p); }

	uint16_t blue: 5;
	uint16_t green: 6;
	uint16_t red: 5;

	static constexpr uint8_t alpha = 0xFF;
	static constexpr unsigned int BITS_PER_PIXEL = 16;
};

static constexpr uint16_t SCREEN_WIDTH_SIZE  = 480;
static constexpr uint16_t SCREEN_HEIGHT_SIZE = 272;
static constexpr bool 	  SCREEN_USE_DOUBLE_BUFFERING = true;

static constexpr bool 	  USE_SIN_CACHE = false;
static constexpr bool 	  USE_COS_CACHE = false;

static constexpr PIXEL_COLOR_ENCODINGS HW_COLOR_ENCODE = USED_COLOR_ENCODING;

#define __PixelPaste(e) Pixel_ ## e
#define __PixelEval(e) __PixelPaste(e)
#define Pixel __PixelEval(USED_COLOR_ENCODING)

typedef char TUI_CHAR;

#define TUI_MUTEX TOS_MUTEX
#define TUI_MUTEX_INIT(m) tos_init_mutex(m)
#define TUI_MUTEX_LOCK(m) tos_lock_mutex(m)
#define TUI_MUTEX_TRY_LOCK(m, msTmo) tos_try_lock_mutex(m, tos_ms2ticks(msTmo))
#define TUI_MUTEX_UNLOCK(m) tos_unlock_mutex(m)

#define TUI_TICKS() tos_ticks()
#define TUI_TICKS_2_MS(t) tos_ticks2ms(t)
#define TUI_MS_2_TICKS(m) tos_ms2ticks(m)


#define _TUI_STRINGNIFY_2(x) #x
#define _TUI_STRINGNIFY(x) _TUI_STRINGNIFY_2(x)

#define TUI_DIE(x) __tui_error(x)

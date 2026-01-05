#include "tos_procs.h"
#include "tos.h"

#include <algorithm>

#include "Screen.h"

Screen lcd;

static constexpr int backDemoStartMs = 0;
static constexpr int backDemoLengthMs = 10 * 1000;
static constexpr int lines1DemoStart = backDemoStartMs + backDemoLengthMs;
static constexpr int lines1DemoLength = 10 * 1000;
static constexpr int demoLengthMs = lines1DemoStart + lines1DemoLength;

static constexpr Pixel white(255, 255, 255);

static void demo_background_update(uint32_t ms)
{
	static constexpr int animationLengthMs = 1000;
	static int color = 0;
	static int lastValue = -1;
	static bool toDark = false;

	ms %= animationLengthMs;
	int ctime = (ms % animationLengthMs);
	if (ctime == 0)
		toDark = !toDark;

	int value = white.red * ms / animationLengthMs;
	if (value < lastValue) {
		if (toDark) color = (color + 1) % 3;
		toDark = !toDark;
	}

	lastValue = value;

	if (toDark)
		value = white.red - value;

	Pixel p;
	if (color == 0) p = Pixel(value, 0, 0);
	else if (color == 1) p = Pixel(0, value, 0);
	else if (color == 2) p = Pixel(0, 0, value);

	lcd.fillColor(p);
}


static void demo_draw_lines(uint32_t ms)
{
	static constexpr uint32_t lineCount = 5;
	static constexpr uint32_t animationLengthMs = 2000;

	lcd.fillColor(Pixel());

	ms %= animationLengthMs;

	uint32_t hvalue = SCREEN_WIDTH_SIZE * ms / animationLengthMs;
	uint32_t vvalue = SCREEN_HEIGHT_SIZE * ms / animationLengthMs;

	const uint32_t vsize = 150;

	const auto c = std::max<uint32_t>(white.red * ms / animationLengthMs, 4);
	Pixel color(c, c, c);

	const auto lineWidth = std::max<uint32_t>(ms % 15, 1);
	for (uint32_t i = 0; i < lineCount; ++i)
		lcd.drawHLine(0, vvalue + i * vsize / 5, SCREEN_WIDTH_SIZE, lineWidth, color);

	for (uint32_t i = 0; i < lineCount; ++i)
		lcd.drawVLine(hvalue + i * vsize / 5, 0, SCREEN_HEIGHT_SIZE, lineWidth, color);
}


TOS_PROCESS(gfx_demo, 10240)
{
	int lastDrawTime = 0;
	lcd.m_showFps = true;
	uint32_t lastMs = 0;

	while (true)
	{
		const uint32_t ctick = tos_ticks();
		const uint32_t ctime = (tos_ticks2ms(ctick) % demoLengthMs);

		lcd.startFrameDrawing();

		if ((backDemoStartMs <= ctime) && (ctime <= backDemoLengthMs))
		{
			demo_background_update(ctime - backDemoStartMs);
		}
		else if ((lines1DemoStart <= ctime) && (ctime <= lines1DemoStart + lines1DemoLength))
		{
			demo_draw_lines(ctime - lines1DemoStart);
		}

		lcd.endFrameDrawing(true);
	}
}

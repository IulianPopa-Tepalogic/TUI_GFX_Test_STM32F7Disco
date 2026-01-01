#include "tos_procs.h"
#include "tos.h"

#include "Screen.h"

Screen lcd;

static const int backDemoStartMs = 0;
static const int backDemoLengthMs = 10 * 1000;
static const int demoLengthMs = backDemoStartMs + backDemoLengthMs;

static void demo_background_update(uint32_t ms)
{
	static const Pixel white(255, 255, 255);
	static const int animationLengthMs = 1000;
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


TOS_PROCESS(gfx_demo, 10240)
{
	int lastDrawTime = 0;
	lcd.m_showFps = true;

	while (true)
	{
		int ctime = (int)(tos_ticks2ms(tos_ticks()) % demoLengthMs);

		if ((backDemoStartMs <= ctime) && (ctime <= backDemoLengthMs))
		{
			lcd.startFrameDrawing();
			demo_background_update(ctime - backDemoStartMs);
			lcd.endFrameDrawing(true);
		}
	}
}

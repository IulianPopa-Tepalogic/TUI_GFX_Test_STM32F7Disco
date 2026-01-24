#include "tos_procs.h"
#include "tos.h"

#include <algorithm>

#include "Screen.h"

Screen lcd;

static constexpr int backDemoStartMs = 0;
static constexpr int backDemoLengthMs = 10 * 1000;
static constexpr int lines1DemoStart = backDemoStartMs + backDemoLengthMs;
static constexpr int lines1DemoLength = 10 * 1000;
static constexpr int lines1DdDemoStart = lines1DemoStart + lines1DemoLength;
static constexpr int lines1DdDemoLength = 10 * 1000;
static constexpr int lines1DdPpDemoStart = lines1DdDemoStart + lines1DdDemoLength;
static constexpr int lines1DdPpDemoLength = 10 * 1000;
static constexpr int lines2DemoStart = lines1DdPpDemoStart + lines1DdPpDemoLength;
static constexpr int lines2DemoLength = 10 * 1000;
static constexpr int lines2PPDemoStart = lines2DemoStart + lines2DemoLength;
static constexpr int lines2PPDemoLength = 10 * 1000;
static constexpr int lines2DDPPDemoStart = lines2PPDemoStart + lines2PPDemoLength;
static constexpr int lines2DDPPDemoLength = 10 * 1000;
static constexpr int circlesDemoStart =  lines2DDPPDemoStart + lines2DDPPDemoLength;
static constexpr int circlesDemoLength = 10 * 1000;

static constexpr int demoLengthMs = circlesDemoStart + circlesDemoLength;

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
		lcd.drawHLine(SCREEN_WIDTH_SIZE - ms, vvalue + i * vsize / 5, SCREEN_WIDTH_SIZE * 100, lineWidth, color);

	for (uint32_t i = 0; i < lineCount; ++i)
		lcd.drawVLine(hvalue + i * vsize / 5, SCREEN_HEIGHT_SIZE - ms, SCREEN_HEIGHT_SIZE * 100, lineWidth, color);
}

static void demo_draw_lines_dashes(uint32_t ms)
{
	static constexpr uint32_t lineCount = 5;
	static constexpr uint32_t animationLengthMs = 2000;

	lcd.fillColor(Pixel());

	ms %= animationLengthMs;

	uint32_t hvalue = SCREEN_WIDTH_SIZE * ms / animationLengthMs;
	uint32_t vvalue = SCREEN_HEIGHT_SIZE * ms / animationLengthMs;

	const uint32_t vsize = 150;

	const auto c = std::max<uint32_t>(white.red * ms / animationLengthMs, 4) + 10;
	Pixel color(c, c, c);

	int dashFill = ms % 20;
	int dashSkip = 20 - dashFill;

	const auto lineWidth = std::max<uint32_t>(ms % 15, 1);
	for (uint32_t i = 0; i < lineCount; ++i)
		lcd.drawHLine(SCREEN_WIDTH_SIZE - ms, vvalue + i * vsize / 5, SCREEN_WIDTH_SIZE * 100, lineWidth, color, dashFill, dashSkip);

	for (uint32_t i = 0; i < lineCount; ++i)
		lcd.drawVLine(hvalue + i * vsize / 5, SCREEN_HEIGHT_SIZE - ms, SCREEN_HEIGHT_SIZE * 100, lineWidth, color, dashFill, dashSkip);
}


static void demo_draw_lines_dashes_pixel_provider(uint32_t ms)
{
	static constexpr uint32_t lineCount = 5;
	static constexpr uint32_t animationLengthMs = 2000;

	lcd.fillColor(Pixel());

	ms %= animationLengthMs;

	uint32_t hvalue = SCREEN_WIDTH_SIZE * ms / animationLengthMs;
	uint32_t vvalue = SCREEN_HEIGHT_SIZE * ms / animationLengthMs;

	const uint32_t vsize = 150;

	const auto c = std::max<uint32_t>(white.red * ms / animationLengthMs, 4) + 30;
	Pixel color(c, c, c);

	int dashFill = ms % 20;
	int dashSkip = 20 - dashFill;

	const auto lineWidth = std::max<uint32_t>(ms % 15, 1);
	for (uint32_t i = 0; i < lineCount; ++i)
		lcd.drawHLine(SCREEN_WIDTH_SIZE - ms, vvalue + i * vsize / 5, SCREEN_WIDTH_SIZE * 100, lineWidth, Unicolor(color), dashFill, dashSkip);

	for (uint32_t i = 0; i < lineCount; ++i)
		lcd.drawVLine(hvalue + i * vsize / 5, SCREEN_HEIGHT_SIZE - ms, SCREEN_HEIGHT_SIZE * 100, lineWidth, Unicolor(color), dashFill, dashSkip);
}

static void demo_draw_lines2(uint32_t ms)
{
	static constexpr Pixel bgPixel = Pixel(1, 1, 1);

	static constexpr int animationLengthMs = 1000;
	ms %= animationLengthMs;
	if (ms ==0) ms = 1;

	float angle = 360.0f * ms / animationLengthMs;

	const int diff = ms % 50 + 1;

	lcd.fillColor(bgPixel);
#if 0
	lcd.drawLine(0, SCREEN_HEIGHT_SIZE / 2 - 30, SCREEN_WIDTH_SIZE - 1, SCREEN_HEIGHT_SIZE / 2 -30 - diff, Pixel(0xFF, 0xFF, 0xFF));

	lcd.drawLine(0, SCREEN_HEIGHT_SIZE / 2 + 30, SCREEN_WIDTH_SIZE - 1, SCREEN_HEIGHT_SIZE / 2 + 30 + diff, Pixel(0xFF, 0xFF, 0xFF));

	lcd.drawLine(SCREEN_WIDTH_SIZE / 2 - 30, 0, SCREEN_WIDTH_SIZE / 2 - 30 - diff, SCREEN_HEIGHT_SIZE - 1, Pixel(0xFF, 0xFF, 0xFF));

	lcd.drawLine(SCREEN_WIDTH_SIZE / 2 + 30, 0, SCREEN_WIDTH_SIZE / 2 + 30 + diff, SCREEN_HEIGHT_SIZE - 1, Pixel(0xFF, 0xFF, 0xFF));

#else
	for (int trailing = 1; trailing < 5; trailing++) {

		Pixel p = Canvas::combinePixels(Pixel(0xFF, 0, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(30, 30, 20, angle - trailing * 1.3f, p);

		p = Canvas::combinePixels(Pixel(0, 0xFF, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(400, 70, 60, -angle - 180 + trailing * 1.3f, p);

		p = Canvas::combinePixels(Pixel(0, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(50, 200, 90, angle - trailing * 1.3f, p);

		p = Canvas::combinePixels(Pixel(0xFF, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(380, 210, 80, -angle - 180 + trailing * 1.3f, p);

		p = Canvas::combinePixels(Pixel(0xFF, 0xFF, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE / 2 , SCREEN_HEIGHT_SIZE / 2, 1000, angle - 20 - trailing * 1.3f, p);


		p = Canvas::combinePixels(Pixel(0xFF, 0, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(-100 , -100, 1000, angle - 20 - trailing * 1.3f, p);

		p = Canvas::combinePixels(Pixel(0, 0xFF, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE / 2, -100,  1000, angle - 20 - trailing * 1.3f, p);

		p = Canvas::combinePixels(Pixel(0, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE + 100, -100 , 1000, angle - 20 - trailing * 1.3f, p);

		p = Canvas::combinePixels(Pixel(0xFF, 0xFF, 10), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(-100 , SCREEN_HEIGHT_SIZE + 100, 1000, angle - 20 - trailing * 1.3f, p);

		p = Canvas::combinePixels(Pixel(10, 0xFF, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE / 2, SCREEN_HEIGHT_SIZE + 100,  1000, angle - 20 - trailing * 1.3f, p);

		p = Canvas::combinePixels(Pixel(0XFF, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE + 100, SCREEN_HEIGHT_SIZE + 100, 1000, angle - 20 - trailing * 1.3f, p);
	}
#endif
}


static void demo_draw_lines2_pixel_provider(uint32_t ms)
{
	static constexpr Pixel bgPixel = Pixel(15, 10, 10);

	static constexpr int animationLengthMs = 1000;
	ms %= animationLengthMs;
	if (ms ==0) ms = 1;

	float angle = 360.0f * ms / animationLengthMs;

	lcd.fillColor(bgPixel);

	for (int trailing = 1; trailing < 5; trailing++) {

		Pixel p = Canvas::combinePixels(Pixel(0xFF, 0, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(30, 30, 20, angle - trailing * 1.3f, Unicolor(p));

		p = Canvas::combinePixels(Pixel(0, 0xFF, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(400, 70, 60, -angle - 180 + trailing * 1.3f, Unicolor(p));

		p = Canvas::combinePixels(Pixel(0, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(50, 200, 90, angle - trailing * 1.3f, Unicolor(p));

		p = Canvas::combinePixels(Pixel(0xFF, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(380, 210, 80, -angle - 180 + trailing * 1.3f, Unicolor(p));

		p = Canvas::combinePixels(Pixel(0xFF, 0xFF, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE / 2 , SCREEN_HEIGHT_SIZE / 2, 1000, angle - 20 - trailing * 1.3f, Unicolor(p));


		p = Canvas::combinePixels(Pixel(0xFF, 0, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(-100 , -100, 1000, angle - 20 - trailing * 1.3f, Unicolor(p));

		p = Canvas::combinePixels(Pixel(0, 0xFF, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE / 2, -100,  1000, angle - 20 - trailing * 1.3f, Unicolor(p));

		p = Canvas::combinePixels(Pixel(0, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE + 100, -100 , 1000, angle - 20 - trailing * 1.3f, Unicolor(p));

		p = Canvas::combinePixels(Pixel(0xFF, 0xFF, 10), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(-100 , SCREEN_HEIGHT_SIZE + 100, 1000, angle - 20 - trailing * 1.3f, Unicolor(p));

		p = Canvas::combinePixels(Pixel(10, 0xFF, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE / 2, SCREEN_HEIGHT_SIZE + 100,  1000, angle - 20 - trailing * 1.3f, Unicolor(p));

		p = Canvas::combinePixels(Pixel(0XFF, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE + 100, SCREEN_HEIGHT_SIZE + 100, 1000, angle - 20 - trailing * 1.3f, Unicolor(p));
	}
}

static void demo_draw_lines2_dashes_pixel_provider(uint32_t ms)
{
	static constexpr Pixel bgPixel = Pixel(1, 1, 1);

	static constexpr int animationLengthMs = 1000;
	ms %= animationLengthMs;
	if (ms ==0) ms = 1;

	int dashFill = ms % 20;
	int dashSkip = 20 - dashFill;

	float angle = 360.0f * ms / animationLengthMs;

	lcd.fillColor(bgPixel);

	for (int trailing = 1; trailing < 10; trailing++) {

		Pixel p = Canvas::combinePixels(Pixel(0xFF, 0, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(30, 30, 20, angle - trailing * 1.3f, Unicolor(p), dashFill, dashSkip);

		p = Canvas::combinePixels(Pixel(0, 0xFF, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(400, 70, 60, -angle - 180 + trailing * 1.3f, Unicolor(p), dashFill, dashSkip);

		p = Canvas::combinePixels(Pixel(0, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(50, 200, 90, angle - trailing * 1.3f, Unicolor(p), dashFill, dashSkip);

		p = Canvas::combinePixels(Pixel(0xFF, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(380, 210, 80, -angle - 180 + trailing * 1.3f, Unicolor(p), dashFill, dashSkip);

		p = Canvas::combinePixels(Pixel(0xFF, 0xFF, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE / 2 , SCREEN_HEIGHT_SIZE / 2, 1000, angle - 20 - trailing * 1.3f, Unicolor(p), dashFill, dashSkip);

		p = Canvas::combinePixels(Pixel(0xFF, 0, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(-100 , -100, 1000, angle - 20 - trailing * 1.3f, Unicolor(p), dashFill, dashSkip);

		p = Canvas::combinePixels(Pixel(0, 0xFF, 0), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE / 2, -100,  1000, angle - 20 - trailing * 1.3f, Unicolor(p), dashFill, dashSkip);

		p = Canvas::combinePixels(Pixel(0, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE + 100, -100 , 1000, angle - 20 - trailing * 1.3f, Unicolor(p), dashFill, dashSkip);

		p = Canvas::combinePixels(Pixel(0xFF, 0xFF, 10), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(-100 , SCREEN_HEIGHT_SIZE + 100, 1000, angle - 20 - trailing * 1.3f, Unicolor(p), dashFill, dashSkip);

		p = Canvas::combinePixels(Pixel(10, 0xFF, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE / 2, SCREEN_HEIGHT_SIZE + 100,  1000, angle - 20 - trailing * 1.3f, Unicolor(p), dashFill, dashSkip);

		p = Canvas::combinePixels(Pixel(0XFF, 0, 0xFF), bgPixel, 1.f - 1.f / trailing);
		lcd.drawSlopedLine(SCREEN_WIDTH_SIZE + 100, SCREEN_HEIGHT_SIZE + 100, 1000, angle - 20 - trailing * 1.3f, Unicolor(p), dashFill, dashSkip);
	}
}


static void demo_draw_circles(uint32_t ms)
{
	static constexpr Pixel bgPixel = Pixel(1, 1, 1);

	static constexpr int animationLengthMs = 3000;
	ms %= animationLengthMs;
	if (ms ==0) ms = 1;

	lcd.fillColor(bgPixel);


	lcd.drawCircle(SCREEN_WIDTH_SIZE / 2, SCREEN_HEIGHT_SIZE / 2, ms % 400, ms % 30, Pixel(0xFF, 0xFF, 0Xff));

	lcd.drawCircle(30 + ms % 100, 100 + ms % 100, ms % 50, ms % 40, Pixel(0xFF, 0x22, 0X11));

	lcd.drawCircle(-70 + ms % 100, 30 + ms % 100, ms % 60, ms % 15, Pixel(0xFF, 0x22, 0X11));
	lcd.drawCircle(81 - ms % 100, 300 - ms % 100, ms % 81, ms % 25, Pixel(0x22, 0x11, 0XFF));
	lcd.drawCircle(81 - ms % 100, 300 + ms % 100, ms % 51, ms % 7, Pixel(030, 0x20, 0X60));
	lcd.drawCircle(120 + ms % 100, 40 - ms % 100, ms % 68, ms % 8, Pixel(0x10, 0xFF, 0x40));

	lcd.drawCircle(-70 + ms % 100, 30 + ms % 100, ms % 60, 1, Pixel(0xFF, 0x22, 0X11));
	lcd.drawCircle(SCREEN_WIDTH_SIZE - 81 + ms % 100, -100 + ms % 100, ms % 81, 1, Pixel(0x22, 0x11, 0XFF));
	lcd.drawCircle(SCREEN_WIDTH_SIZE - 81 + ms % 100, 100 - ms % 100, ms % 51, 1, Pixel(030, 0x20, 0X60));
	lcd.drawCircle(SCREEN_WIDTH_SIZE - 120 - ms % 100, 200 - ms % 100, ms % 68, 1, Pixel(0x10, 0xFF, 0x40));
}


TOS_PROCESS(gfx_demo, 10240)
{
	lcd.m_showFps = true;

	while (true)
	{
#if 1
		uint32_t ctick = tos_ticks();
		uint32_t ctime = (tos_ticks2ms(ctick) % demoLengthMs);
#else
		uint32_t ctick = tos_ticks();
		uint32_t ctime = (tos_ticks2ms(ctick) % lines2DemoLength) + lines2DemoStart;
#endif

		lcd.startFrameDrawing();

		if ((backDemoStartMs <= ctime) && (ctime <= backDemoLengthMs))
		{
			demo_background_update(ctime - backDemoStartMs);
		}
		else if ((lines1DemoStart <= ctime) && (ctime <= lines1DemoStart + lines1DemoLength))
		{
			demo_draw_lines(ctime - lines1DemoStart);
		}
		else if ((lines1DdDemoStart <= ctime) && (ctime <= lines1DdDemoStart + lines1DdDemoLength))
		{
			demo_draw_lines_dashes(ctime - lines1DdDemoStart);
		}
		else if ((lines1DdPpDemoStart <= ctime) && (ctime <= lines1DdPpDemoStart + lines1DdPpDemoLength))
		{
			demo_draw_lines_dashes_pixel_provider(ctime - lines1DdPpDemoStart);
		}
		else if ((lines2DemoStart <= ctime) && (ctime <= lines2DemoStart + lines2DemoLength))
		{
			demo_draw_lines2(ctime - lines2DemoStart);
		}
		else if ((lines2PPDemoStart <= ctime) && (ctime <= lines2PPDemoStart + lines2PPDemoLength))
		{
			demo_draw_lines2_pixel_provider(ctime - lines2PPDemoStart);
		}
		else if ((lines2DDPPDemoStart <= ctime) && (ctime <= lines2DDPPDemoStart + lines2DDPPDemoLength))
		{
			demo_draw_lines2_dashes_pixel_provider(ctime - lines2DDPPDemoStart);
		}
		else if ((circlesDemoStart <= ctime) && (ctime <= circlesDemoStart + circlesDemoLength))
		{
			demo_draw_circles(ctime - circlesDemoStart);
		}

		lcd.endFrameDrawing(true);

		auto frameTimeMs = tos_ticks2ms(tos_ticks() - ctick);
		if (frameTimeMs < 37)
			tos_sleep(37 - frameTimeMs);
	}
}

#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "condition.h"
#include "guicast.h"
#include "thread.h"
#include "vframe.inc"

class ColorWindow;
class PaletteWheel;
class PaletteWheelValue;
class PaletteOutput;
class PaletteHue;
class PaletteSaturation;
class PaletteValue;
class PaletteRed;
class PaletteGreen;
class PaletteBlue;
class PaletteAlpha;

class ColorThread : public Thread
{
public:
	ColorThread(int do_alpha = 0, char *title = 0);
	~ColorThread();

	void run();
	void start_window(int output, int alpha);
	virtual int handle_event(int output) { return 0; };
	void update_gui(int output, int alpha);

	Condition completion;
	Mutex mutex;	// protects window
	ColorWindow *window;
// Starting color
	int output;
	int alpha;
	int do_alpha;
	char *title;
};

class ColorWindow : public BC_Window
{
public:
	ColorWindow(ColorThread *thread, int x, int y, char *title);

	void create_objects();
	int close_event();
	void update_display();
	void update_rgb();
	void change_values();	// does not update the display
	int handle_event();

	ColorThread *thread;

	PaletteWheel *wheel;
	PaletteWheelValue *wheel_value;
	PaletteOutput *output;
	PaletteHue *hue;
	PaletteSaturation *saturation;
	PaletteValue *value;
	PaletteRed *red;
	PaletteGreen *green;
	PaletteBlue *blue;
	PaletteAlpha *alpha;
	VFrame *value_bitmap;
	float h, s, v, r, g, b, a;
};


class PaletteWheel : public BC_SubWindow
{
public:
	PaletteWheel(ColorWindow *window, int x, int y);
	~PaletteWheel();
	int button_press_event();
	int cursor_motion_event();
	int button_release_event();

	int create_objects();
	int draw(float hue, float saturation);
	int get_angle(float x1, float y1, float x2, float y2);
	float torads(float angle);
	ColorWindow *window;
	float oldhue;
	float oldsaturation;
	int button_down;
};

class PaletteWheelValue : public BC_SubWindow
{
public:
	PaletteWheelValue(ColorWindow *window, int x, int y);
	~PaletteWheelValue();
	int create_objects();
	int button_press_event();
	int cursor_motion_event();
	int button_release_event();
	int draw(float hue, float saturation, float value);
	ColorWindow *window;
	int button_down;
// Gradient
	VFrame *frame;
};

class PaletteOutput : public BC_SubWindow
{
public:
	PaletteOutput(ColorWindow *window, int x, int y);
	~PaletteOutput();
	int create_objects();
	int handle_event();
	int draw();
	ColorWindow *window;
};

class PaletteHue : public BC_ISlider
{
public:
	PaletteHue(ColorWindow *window, int x, int y);
	~PaletteHue();
	int handle_event();
	ColorWindow *window;
};

class PaletteSaturation : public BC_FSlider
{
public:
	PaletteSaturation(ColorWindow *window, int x, int y);
	~PaletteSaturation();
	int handle_event();
	ColorWindow *window;
};

class PaletteValue : public BC_FSlider
{
public:
	PaletteValue(ColorWindow *window, int x, int y);
	~PaletteValue();
	int handle_event();
	ColorWindow *window;
};

class PaletteRed : public BC_FSlider
{
public:
	PaletteRed(ColorWindow *window, int x, int y);
	~PaletteRed();
	int handle_event();
	ColorWindow *window;
};

class PaletteGreen : public BC_FSlider
{
public:
	PaletteGreen(ColorWindow *window, int x, int y);
	~PaletteGreen();
	int handle_event();
	ColorWindow *window;
};

class PaletteBlue : public BC_FSlider
{
public:
	PaletteBlue(ColorWindow *window, int x, int y);
	~PaletteBlue();
	int handle_event();
	ColorWindow *window;
};

class PaletteAlpha : public BC_FSlider
{
public:
	PaletteAlpha(ColorWindow *window, int x, int y);
	~PaletteAlpha();
	int handle_event();
	ColorWindow *window;
};

#endif

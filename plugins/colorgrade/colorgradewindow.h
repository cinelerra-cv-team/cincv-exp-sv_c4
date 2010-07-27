
/*
 * CINELERRA
 * Copyright (C) 2008 Adam Williams <broadcast at earthling dot net>
 *               2010 Simeon Voelkel <simeon_voelkel@arcor.de> 
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 */

#ifndef COLORGRADEWINDOW_H
#define COLORGRADEWINDOW_H



#include "colorgrade.inc"
#include "colorgradewindow.inc"
#include "pluginvclient.h"



class ColorgradeSlider : public BC_SubWindow
{
public:
	ColorgradeSlider(ColorgradeMain *plugin, 
		ColorgradeWindow *gui,
		int x, 
		int y, 
		int w,
		int h,
		int is_input);

	void update();
	int button_press_event();
	int button_release_event();
	int cursor_motion_event();
	int input_to_pixel(float input);

	int operation;
	enum
	{
		NONE,
		DRAG_INPUT,
		DRAG_MIN_OUTPUT,
		DRAG_MAX_OUTPUT,
	};
	int is_input;
	ColorgradeMain *plugin;
	ColorgradeWindow *gui;
};

class ColorgradeAuto : public BC_CheckBox
{
public:
	ColorgradeAuto(ColorgradeMain *plugin, 
		int x, 
		int y);
	int handle_event();
	ColorgradeMain *plugin;
};

class ColorgradePlot : public BC_CheckBox
{
public:
	ColorgradePlot(ColorgradeMain *plugin, 
		int x, 
		int y);
	int handle_event();
	ColorgradeMain *plugin;
};

class ColorgradeSplit : public BC_CheckBox
{
public:
	ColorgradeSplit(ColorgradeMain *plugin, 
		int x, 
		int y);
	int handle_event();
	ColorgradeMain *plugin;
};

class ColorgradeMode : public BC_Radial
{
public:
	ColorgradeMode(ColorgradeMain *plugin, 
		int x, 
		int y,
		int value,
		char *text);
	int handle_event();
	ColorgradeMain *plugin;
	int value;
};

class ColorgradeReset : public BC_GenericButton
{
public:
	ColorgradeReset(ColorgradeMain *plugin, 
		int x,
		int y);
	int handle_event();
	ColorgradeMain *plugin;
};

class ColorgradeOutputText : public BC_TumbleTextBox
{
public:
	ColorgradeOutputText(ColorgradeMain *plugin,
		ColorgradeWindow *gui,
		int x,
		int y,
		float *output);
	int handle_event();
	ColorgradeMain *plugin;
	float *output;
};

class ColorgradeInputText : public BC_TumbleTextBox
{
public:
	ColorgradeInputText(ColorgradeMain *plugin,
		ColorgradeWindow *gui,
		int x,
		int y,
		int do_x);

	int handle_event();
	void update();

	ColorgradeMain *plugin;
	ColorgradeWindow *gui;
	int do_x;
};

class ColorgradeCanvas : public BC_SubWindow
{
public:
	ColorgradeCanvas(ColorgradeMain *plugin,
		ColorgradeWindow *gui,
		int x,
		int y,
		int w,
		int h);
	int button_press_event();
	int cursor_motion_event();
	int button_release_event();
	ColorgradeMain *plugin;
	ColorgradeWindow *gui;
};

class ColorgradeWindow : public BC_Window
{
public:
	ColorgradeWindow(ColorgradeMain *plugin, int x, int y);
	~ColorgradeWindow();

	int create_objects();
	int close_event();
	void update(int do_input);
	void update_mode();
	void update_canvas();
	void draw_canvas_overlay();
	void update_input();
	void update_output();
	int keypress_event();
	void get_point_extents(ColorgradePoint *current,
		int *x1, 
		int *y1, 
		int *x2, 
		int *y2,
		int *x,
		int *y);

	ColorgradeSlider *output;
	ColorgradeMode *mode_l, *mode_m, *mode_h;
	ColorgradeOutputText *output_min;
	ColorgradeOutputText *output_max;
	ColorgradeInputText *input_x;
	ColorgradeInputText *input_y;
	ColorgradeCanvas *canvas;
	ColorgradeMain *plugin;
	int canvas_w;
	int canvas_h;
	int title1_x;
	int title2_x;
	int title3_x;
	int title4_x;
	BC_Pixmap *max_picon, *mid_picon, *min_picon;
	ColorgradePlot *plot;
	ColorgradeSplit *split;
};



PLUGIN_THREAD_HEADER(ColorgradeMain, ColorgradeThread, ColorgradeWindow)




#endif

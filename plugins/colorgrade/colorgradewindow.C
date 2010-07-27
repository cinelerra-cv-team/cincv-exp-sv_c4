
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

#include "bcdisplayinfo.h"
#include "bcsignals.h"
#include "cursors.h"
#include "colorgrade.h"
#include "colorgradeconfig.h"
#include "colorgradewindow.h"
#include "keys.h"
#include "language.h"


#include <unistd.h>

PLUGIN_THREAD_OBJECT(ColorgradeMain, ColorgradeThread, ColorgradeWindow)



ColorgradeWindow::ColorgradeWindow(ColorgradeMain *plugin, int x, int y)
 : BC_Window(plugin->gui_string, 
 	x,
	y,
	440, 
	500, 
	440, 
	500, 
	0, 
	1,
	1)
{
	this->plugin = plugin; 
}

ColorgradeWindow::~ColorgradeWindow()
{
}

#include "max_picon_png.h"
#include "mid_picon_png.h"
#include "min_picon_png.h"
static VFrame max_picon_image(max_picon_png);
static VFrame mid_picon_image(mid_picon_png);
static VFrame min_picon_image(min_picon_png);

int ColorgradeWindow::create_objects()
{
	int x = 10, y = 10, x1 = 10;
	BC_Title *title = 0;

	max_picon = new BC_Pixmap(this, &max_picon_image);
	mid_picon = new BC_Pixmap(this, &mid_picon_image);
	min_picon = new BC_Pixmap(this, &min_picon_image);

	add_subwindow(new BC_Title(x, y, _("Value")));
	x += 80;

	add_subwindow(mode_l = new ColorgradeMode(plugin, 
		x, 
		y,
		COLORGRADE_LOW,
		_("Low")));
	x += 70;
	add_subwindow(mode_m = new ColorgradeMode(plugin, 
		x, 
		y,
		COLORGRADE_MID,
		_("Mid")));
	x += 70;
	add_subwindow(mode_h = new ColorgradeMode(plugin, 
		x, 
		y,
		COLORGRADE_HIGH,
		_("High")));

	x = x1;
	y += 30;
	add_subwindow(title = new BC_Title(x, y, _("Input X:")));
	x += title->get_w() + 10;
	input_x = new ColorgradeInputText(plugin,
		this,
		x,
		y,
		1);
	input_x->create_objects();

	x += input_x->get_w() + 10;
	add_subwindow(title = new BC_Title(x, y, _("Input Y:")));
	x += title->get_w() + 10;
	input_y = new ColorgradeInputText(plugin,
		this,
		x,
		y,
		0);
	input_y->create_objects();

	y += 30;
	x = x1;

	canvas_w = get_w() - x - x;
	canvas_h = get_h() - y - 190;
	title1_x = x;
	title2_x = x + (int)(canvas_w * -MIN_INPUT / FLOAT_RANGE);
	title3_x = x + (int)(canvas_w * (1.0 - MIN_INPUT) / FLOAT_RANGE);
	title4_x = x + (int)(canvas_w);
	add_subwindow(canvas = new ColorgradeCanvas(plugin,
		this,
		x, 
		y, 
		canvas_w, 
		canvas_h));
// Calculate output curve with no value function
	plugin->tabulate_curve(plugin->mode);
	draw_canvas_overlay();
	canvas->flash();

	y += canvas->get_h() + 1;
/*
	add_subwindow(new BC_Title(title1_x, 
		y, 
		"-10%"));
*/
	add_subwindow(new BC_Title(title2_x,
		y,
		"0%"));
	add_subwindow(new BC_Title(title3_x - get_text_width(MEDIUMFONT, "100"),
		y,
		"100%"));
/*
	add_subwindow(new BC_Title(title4_x - get_text_width(MEDIUMFONT, "110"),
		y,
		"110%"));
*/

	y += 20;
	add_subwindow(title = new BC_Title(x, y, _("Output min:")));
	x += title->get_w() + 10;
	output_min = new ColorgradeOutputText(plugin,
		this,
		x,
		y,
		&plugin->config.output_min[plugin->mode]);
	output_min->create_objects();
	x += output_min->get_w() + 10;
	add_subwindow(new BC_Title(x, y, _("Output Max:")));
	x += title->get_w() + 10;
	output_max = new ColorgradeOutputText(plugin,
		this,
		x,
		y,
		&plugin->config.output_max[plugin->mode]);
	output_max->create_objects();

	x = x1;
	y += 30;

	add_subwindow(output = new ColorgradeSlider(plugin, 
		this,
		x, 
		y, 
		get_w() - 20,
		30,
		0));
	output->update();
	y += 40;

	x = x1;
	add_subwindow(new ColorgradeReset(plugin, 
		x, 
		y));
	x = x1;
	y += 30;
	add_subwindow(plot = new ColorgradePlot(plugin, 
		x, 
		y));

	y += plot->get_h() + 5;
	add_subwindow(split = new ColorgradeSplit(plugin, 
		x, 
		y));

	show_window();

	return 0;
}

WINDOW_CLOSE_EVENT(ColorgradeWindow)

int ColorgradeWindow::keypress_event()
{
	int result = 0;
	if(get_keypress() == BACKSPACE ||
		get_keypress() == DELETE)
	{
		if(plugin->current_point >= 0)
		{
			ColorgradePoint *current = 
				plugin->config.points[plugin->mode].get_item_number(plugin->current_point);
			delete current;
			plugin->current_point = -1;
			update_input();
			update_canvas();
			plugin->send_configure_change();
			result = 1;
		}
	}
	return result;
}

void ColorgradeWindow::update(int do_input)
{
	update_mode();

	if(do_input) update_input();
	update_output();
}

void ColorgradeWindow::update_input()
{
	input_x->update();
	input_y->update();
}

void ColorgradeWindow::update_output()
{
	output->update();
	output_min->update(plugin->config.output_min[plugin->mode]);
	output_max->update(plugin->config.output_max[plugin->mode]);
}

void ColorgradeWindow::update_mode()
{
	mode_l->update(plugin->mode == COLORGRADE_LOW ? 1 : 0);
	mode_m->update(plugin->mode == COLORGRADE_MID ? 1 : 0);
	mode_h->update(plugin->mode == COLORGRADE_HIGH ? 1 : 0);
	output_min->output = &plugin->config.output_min[plugin->mode];
	output_max->output = &plugin->config.output_max[plugin->mode];
	plot->update(plugin->config.plot);
	split->update(plugin->config.split);
}

void ColorgradeWindow::draw_canvas_overlay()
{
	canvas->set_color(0x00ff00);
	int y1;


// Draw output line
	for(int i = 0; i < canvas_w; i++)
	{
		float input = (float)i / 
				canvas_w * 
				FLOAT_RANGE + 
				MIN_INPUT;
		float output = plugin->calculate_smooth(input, plugin->mode);

		int y2 = canvas_h - (int)(output * canvas_h);
		if(i > 0)
		{
			canvas->draw_line(i - 1, y1, i, y2);
		}
		y1 = y2;
	}

// Draw output points
	ColorgradePoint *current = plugin->config.points[plugin->mode].first;
	int number = 0;
	while(current)
	{
		int x1;
		int y1;
		int x2;
		int y2;
		int x;
		int y;
		get_point_extents(current,
			&x1, 
			&y1, 
			&x2, 
			&y2,
			&x,
			&y);

		if(number == plugin->current_point)
			canvas->draw_box(x1, y1, x2 - x1, y2 - y1);
		else
			canvas->draw_rectangle(x1, y1, x2 - x1, y2 - y1);
		current = NEXT;
		number++;
	}


// Draw 0 and 100% lines.
	canvas->set_color(0xff0000);
	canvas->draw_line(title2_x - canvas->get_x(), 
		0, 
		title2_x - canvas->get_x(), 
		canvas_h);
	canvas->draw_line(title3_x - canvas->get_x(), 
		0, 
		title3_x - canvas->get_x(), 
		canvas_h);
}

void ColorgradeWindow::update_canvas()
{
	int normalize = 0;
	int max = 0;


	canvas->set_color(0xffffff);
	canvas->draw_box(0, 0, canvas_w, canvas_h);

	draw_canvas_overlay();
	canvas->flash();
}

void ColorgradeWindow::get_point_extents(ColorgradePoint *current,
	int *x1, 
	int *y1, 
	int *x2, 
	int *y2,
	int *x,
	int *y)
{
	*x = (int)((current->x - MIN_INPUT) * canvas_w / FLOAT_RANGE);
	*y = (int)(canvas_h - current->y * canvas_h);
	*x1 = *x - BOX_SIZE / 2;
	*y1 = *y - BOX_SIZE / 2;
	*x2 = *x1 + BOX_SIZE;
	*y2 = *y1 + BOX_SIZE;
}







ColorgradeCanvas::ColorgradeCanvas(ColorgradeMain *plugin,
	ColorgradeWindow *gui,
	int x,
	int y,
	int w,
	int h)
 : BC_SubWindow(x,
 	y,
	w,
	h,
	0xffffff)
{
	this->plugin = plugin;
	this->gui = gui;
}

int ColorgradeCanvas::button_press_event()
{
	int result = 0;
	if(is_event_win() && cursor_inside())
	{
		if(!plugin->dragging_point)
		{
			ColorgradePoint *new_point = 0;
			gui->deactivate();
// Search for existing point under cursor
			ColorgradePoint *current = plugin->config.points[plugin->mode].first;
			plugin->current_point = -1;
			while(current)
			{
				int x = (int)((current->x - MIN_INPUT) * gui->canvas_w / FLOAT_RANGE);
				int y = (int)(gui->canvas_h - current->y * gui->canvas_h);

				if(get_cursor_x() >= x - BOX_SIZE / 2 &&
					get_cursor_y() >= y - BOX_SIZE / 2 &&
					get_cursor_x() < x + BOX_SIZE / 2 &&
					get_cursor_y() < y + BOX_SIZE / 2)
				{
					plugin->current_point = 
						plugin->config.points[plugin->mode].number_of(current);
					plugin->point_x_offset = get_cursor_x() - x;
					plugin->point_y_offset = get_cursor_y() - y;
					break;
				}
				current = NEXT;
			}

			if(plugin->current_point < 0)
			{
// Create new point under cursor
				float current_x = (float)get_cursor_x() * 
					FLOAT_RANGE / 
					get_w() + 
					MIN_INPUT;
				float current_y = 1.0 - 
					(float)get_cursor_y() / 
					get_h();
				new_point = 
					plugin->config.points[plugin->mode].insert(current_x, current_y);
				plugin->current_point = 
					plugin->config.points[plugin->mode].number_of(new_point);
				plugin->point_x_offset = 0;
				plugin->point_y_offset = 0;
			}

			plugin->dragging_point = 1;
			result = 1;

			plugin->config.boundaries();
			gui->update_input();
			gui->update_canvas();
			if(new_point)
			{
				plugin->send_configure_change();
			}
		}
	}
	return result;
}

int ColorgradeCanvas::cursor_motion_event()
{
	if(plugin->dragging_point)
	{
		float current_x = 
			(float)(get_cursor_x() - plugin->point_x_offset) * 
			FLOAT_RANGE / 
			get_w() + 
			MIN_INPUT;
		float current_y = 1.0 - 
			(float)(get_cursor_y() - plugin->point_y_offset) / 
			get_h();
		ColorgradePoint *current_point = 
			plugin->config.points[plugin->mode].get_item_number(plugin->current_point);
		current_point->x = current_x;
		current_point->y = current_y;
		plugin->config.boundaries();
		gui->update_input();
		gui->update_canvas();
		plugin->send_configure_change();
		return 1;
	}
	else
	if(is_event_win() && cursor_inside())
	{
		ColorgradePoint *current = plugin->config.points[plugin->mode].first;
		int done = 0;
		while(current && !done)
		{
			int x1;
			int y1;
			int x2;
			int y2;
			int x;
			int y;
			gui->get_point_extents(current,
				&x1, 
				&y1, 
				&x2, 
				&y2,
				&x,
				&y);
			int new_cursor = ARROW_CURSOR;
			if(get_cursor_x() >= x1 && 
				get_cursor_y() >= y1 &&
				get_cursor_x() < x2 &&
				get_cursor_y() < y2)
			{
				new_cursor = UPRIGHT_ARROW_CURSOR;
				done = 1;
			}

			if(new_cursor != get_cursor())
				set_cursor(new_cursor);


			current = NEXT;
		}
	}
	return 0;
}

int ColorgradeCanvas::button_release_event()
{
	if(plugin->dragging_point)
	{
// Test for out of order points to delete.
		ColorgradePoint *current = 
			plugin->config.points[plugin->mode].get_item_number(plugin->current_point);
		ColorgradePoint *prev = PREVIOUS;
		ColorgradePoint *next = NEXT;

		if((prev && prev->x >= current->x) ||
			(next && next->x <= current->x))
		{
			delete current;
			plugin->current_point = -1;
			plugin->config.boundaries();
			gui->update_input();
			gui->update_canvas();
			plugin->send_configure_change();
		}

		plugin->dragging_point = 0;
	}
	return 0;
}







ColorgradeReset::ColorgradeReset(ColorgradeMain *plugin, 
	int x,
	int y)
 : BC_GenericButton(x, y, _("Reset"))
{
	this->plugin = plugin;
}
int ColorgradeReset::handle_event()
{
	plugin->config.reset();
	plugin->thread->window->update(1);
	plugin->thread->window->update_canvas();
	plugin->send_configure_change();
	return 1;
}









ColorgradeSlider::ColorgradeSlider(ColorgradeMain *plugin, 
	ColorgradeWindow *gui,
	int x, 
	int y, 
	int w,
	int h,
	int is_input)
 : BC_SubWindow(x, y, w, h)
{
	this->plugin = plugin;
	this->gui = gui;
	this->is_input = is_input;
	operation = NONE;
}

int ColorgradeSlider::input_to_pixel(float input)
{
	return (int)((input - MIN_INPUT) / FLOAT_RANGE * get_w());
}

int ColorgradeSlider::button_press_event()
{
	if(is_event_win() && cursor_inside())
	{
		int min;
		int max;
		int w = get_w();
		int h = get_h();
		int half_h = get_h() / 2;

		gui->deactivate();

		if(operation == NONE)
		{
			int x1 = input_to_pixel(plugin->config.output_min[plugin->mode]) - 
				gui->mid_picon->get_w() / 2;
			int x2 = x1 + gui->mid_picon->get_w();
			if(get_cursor_x() >= x1 && get_cursor_x() < x2 &&
				get_cursor_y() >= half_h && get_cursor_y() < h)
			{
				operation = DRAG_MIN_OUTPUT;
			}
		}

		if(operation == NONE)
		{
			int x1 = input_to_pixel(plugin->config.output_max[plugin->mode]) - 
				gui->mid_picon->get_w() / 2;
			int x2 = x1 + gui->mid_picon->get_w();
			if(get_cursor_x() >= x1 && get_cursor_x() < x2 &&
				get_cursor_y() >= half_h && get_cursor_y() < h)
			{
				operation = DRAG_MAX_OUTPUT;
			}
		}
		return 1;
	}
	return 0;
}

int ColorgradeSlider::button_release_event()
{
	if(operation != NONE)
	{
		operation = NONE;
		return 1;
	}
	return 0;
}

int ColorgradeSlider::cursor_motion_event()
{
	if(operation != NONE)
	{
		float value = (float)get_cursor_x() / get_w() * FLOAT_RANGE + MIN_INPUT;
		CLAMP(value, MIN_INPUT, MAX_INPUT);

		switch(operation)
		{
			case DRAG_MIN_OUTPUT:
				value = MIN(plugin->config.output_max[plugin->mode], value);
				plugin->config.output_min[plugin->mode] = value;
				break;
			case DRAG_MAX_OUTPUT:
				value = MAX(plugin->config.output_min[plugin->mode], value);
				plugin->config.output_max[plugin->mode] = value;
				break;
		}
	
		plugin->config.boundaries();
		gui->update_output();

		plugin->send_configure_change();
		return 1;
	}
	return 0;
}

void ColorgradeSlider::update()
{
	int w = get_w();
	int h = get_h();
	int half_h = get_h() / 2;
	int quarter_h = get_h() / 4;
	int mode = plugin->mode;
	int r = 0xff;
	int g = 0xff;
	int b = 0xff;

	float hue, s, v, f, p, q, t;
	int I;


	clear_box(0, 0, w, h);

	switch(mode)
	{
		case COLORGRADE_LOW:
			v = 0.33;
			break;
		case COLORGRADE_MID:
			v = 0.66;
			break;
		case COLORGRADE_HIGH:
			v = 0.999;
			break;
	}
	
	s = 1;

	if (w <= 1)
	{
		return;
	}

	for(int i = 0; i < w; i++)
	{
		hue = (float) i * 6 / w;
		I = (int) floor(hue);
		f = hue - I;
		p = v * ( 1 - s);
		q = v * ( 1 - s * f );
		t = v * ( 1 - s * ( 1 - f ) );
		switch (I) 
		{ 
			case 0: 
				r = v * 0x100;
				g = t * 0x100;
				b = p * 0x100;
				break;
			case 1: 
				r = q * 0x100;
				g = v * 0x100;
				b = p * 0x100;
				break;
			case 2: 
				r = p * 0x100;
				g = v * 0x100;
				b = t * 0x100;
				break;
			case 3: 
				r = p * 0x100;
				g = q * 0x100;
				b = v * 0x100;
				break;
			case 4: 
				r = t * 0x100;
				g = p * 0x100;
				b = v * 0x100;
				break;
			case 5: 
				r = v * 0x100;
				g = p * 0x100;
				b = q * 0x100;
				break;
		}

//		int color = (int)(i * 0xff / w);
		set_color(
			(r << 16) | 
			(g << 8) | 
			(b) 
			 );

		draw_line(i, 0, i, half_h);
	}

	float min;
	float max;
	min = plugin->config.output_min[plugin->mode];
	max = plugin->config.output_max[plugin->mode];

	draw_pixmap(gui->min_picon,
		input_to_pixel(min) - gui->min_picon->get_w() / 2,
		half_h + 1);
	draw_pixmap(gui->max_picon,
		input_to_pixel(max) - gui->max_picon->get_w() / 2,
		half_h + 1);

	flash();
	flush();
}













ColorgradePlot::ColorgradePlot(ColorgradeMain *plugin, 
	int x, 
	int y)
 : BC_CheckBox(x, y, plugin->config.plot, _("Plot colorgramm"))
{
	this->plugin = plugin;
}

int ColorgradePlot::handle_event()
{
	plugin->config.plot = get_value();
	plugin->send_configure_change();
	return 1;
}




ColorgradeSplit::ColorgradeSplit(ColorgradeMain *plugin, 
	int x, 
	int y)
 : BC_CheckBox(x, y, plugin->config.split, _("Split output"))
{
	this->plugin = plugin;
}

int ColorgradeSplit::handle_event()
{
	plugin->config.split = get_value();
	plugin->send_configure_change();
	return 1;
}



ColorgradeMode::ColorgradeMode(ColorgradeMain *plugin, 
	int x, 
	int y,
	int value,
	char *text)
 : BC_Radial(x, y, plugin->mode == value, text)
{
	this->plugin = plugin;
	this->value = value;
}
int ColorgradeMode::handle_event()
{
	plugin->mode = value;
	plugin->current_point= -1;
	plugin->thread->window->update_canvas();
	plugin->thread->window->update_mode();
	plugin->thread->window->update_input();
	plugin->thread->window->update_canvas();
	plugin->thread->window->update_output();
	plugin->thread->window->output->update();
//	plugin->send_configure_change();
	return 1;
}









ColorgradeOutputText::ColorgradeOutputText(ColorgradeMain *plugin,
	ColorgradeWindow *gui,
	int x,
	int y,
	float *output)
 : BC_TumbleTextBox(gui, 
		output ? (float)*output : 0.0,
		(float)MIN_INPUT,
		(float)MAX_INPUT,
		x, 
		y, 
		60)
{
	this->plugin = plugin;
	this->output = output;
	set_precision(DIGITS);
	set_increment(PRECISION);
}


int ColorgradeOutputText::handle_event()
{
	if(output)
	{
		*output = atof(get_text());
	}

	plugin->thread->window->output->update();
	plugin->send_configure_change();
	return 1;
}








ColorgradeInputText::ColorgradeInputText(ColorgradeMain *plugin,
	ColorgradeWindow *gui,
	int x,
	int y,
	int do_x)
 : BC_TumbleTextBox(gui, 
		0.0,
		(float)MIN_INPUT,
		(float)MAX_INPUT,
		x, 
		y, 
		60)
{
	this->do_x = do_x;
	this->plugin = plugin;
	this->gui = gui;
	set_precision(DIGITS);
	set_increment(PRECISION);
}


int ColorgradeInputText::handle_event()
{
	if(plugin->current_point >= 0 &&
		plugin->current_point < plugin->config.points[plugin->mode].total())
	{
		ColorgradePoint *point = 
			plugin->config.points[plugin->mode].get_item_number(
				plugin->current_point);

		if(point)
		{
			if(do_x) 
				point->x = atof(get_text());
			else
				point->y = atof(get_text());

			plugin->config.boundaries();
			gui->update_canvas();

			plugin->thread->window->output->update();
			plugin->send_configure_change();
		}
	}
	return 1;
}

void ColorgradeInputText::update()
{
	if(plugin->current_point >= 0 &&
		plugin->current_point < plugin->config.points[plugin->mode].total())
	{
		ColorgradePoint *point = 

			plugin->config.points[plugin->mode].get_item_number(
				plugin->current_point);

		if(point)
		{
			if(do_x)
				BC_TumbleTextBox::update(point->x);
			else
				BC_TumbleTextBox::update(point->y);
		}
		else
		{
			BC_TumbleTextBox::update((float)0.0);
		}
	}
	else
	{
		BC_TumbleTextBox::update((float)0.0);
	}
	
}















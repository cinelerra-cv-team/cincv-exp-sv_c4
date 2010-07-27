
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

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "bcdisplayinfo.h"
#include "bcsignals.h"
#include "clip.h"
#include "bchash.h"
#include "filexml.h"
#include "colorgrade.h"
#include "colorgradeconfig.h"
#include "colorgradewindow.h"
#include "keyframe.h"
#include "language.h"
#include "loadbalance.h"
#include "playback3d.h"
#include "plugincolors.h"
#include "vframe.h"
#include "workarounds.h"

#include "aggregated.h"
#include "../colorbalance/aggregated.h"
#include "../interpolate/aggregated.h"
#include "../gamma/aggregated.h"

class ColorgradeMain;
class ColorgradeEngine;
class ColorgradeWindow;





REGISTER_PLUGIN(ColorgradeMain)












ColorgradeMain::ColorgradeMain(PluginServer *server)
 : PluginVClient(server)
{
	PLUGIN_CONSTRUCTOR_MACRO
	engine = 0;
	for(int i = 0; i < COLORGRADE_MODES; i++)
	{
		lookup[i] = 0;
		smoothed[i] = 0;
		linear[i] = 0;
		accum[i] = 0;
		preview_lookup[i] = 0;
	}
	current_point = -1;
	mode = COLORGRADE_LOW;
	dragging_point = 0;
	input = 0;
	output = 0;
}

ColorgradeMain::~ColorgradeMain()
{
	PLUGIN_DESTRUCTOR_MACRO
	for(int i = 0; i < COLORGRADE_MODES;i++)
	{
		delete [] lookup[i];
		delete [] smoothed[i];
		delete [] linear[i];
		delete [] accum[i];
		delete [] preview_lookup[i];
	}
	delete engine;
}

char* ColorgradeMain::plugin_title() { return N_("Colorgrade"); }
int ColorgradeMain::is_realtime() { return 1; }


#include "picon_png.h"
NEW_PICON_MACRO(ColorgradeMain)

SHOW_GUI_MACRO(ColorgradeMain, ColorgradeThread)

SET_STRING_MACRO(ColorgradeMain)

RAISE_WINDOW_MACRO(ColorgradeMain)

LOAD_CONFIGURATION_MACRO(ColorgradeMain, ColorgradeConfig)

void ColorgradeMain::render_gui(void *data)
{
	if(thread)
	{
		// render colorgram
	}
}

void ColorgradeMain::update_gui()
{
	if(thread)
	{
		thread->window->lock_window("ColorgradeMain::update_gui");
		int reconfigure = load_configuration();
		if(reconfigure) 
		{
			thread->window->update(0);
			thread->window->update_input();
		}
		thread->window->unlock_window();
	}
}


int ColorgradeMain::load_defaults()
{
	char directory[BCTEXTLEN], string[BCTEXTLEN];
// set the default directory
	sprintf(directory, "%scolorgrade.rc", BCASTDIR);

// load the defaults
	defaults = new BC_Hash(directory);
	defaults->load();

	for(int j = 0; j < COLORGRADE_MODES; j++)
	{
		while(config.points[j].last) delete config.points[j].last;

		sprintf(string, "TOTAL_POINTS_%d", j);
		int total_points = defaults->get(string, 0);

		for(int i = 0; i < total_points; i++)
		{
			ColorgradePoint *point = new ColorgradePoint;
			sprintf(string, "INPUT_X_%d_%d", j, i);
			point->x = defaults->get(string, point->x);
			sprintf(string, "INPUT_Y_%d_%d", j, i);
			point->y = defaults->get(string, point->y);
			config.points[j].append(point);
		}
	}


	for(int i = 0; i < COLORGRADE_MODES; i++)
	{
		sprintf(string, "OUTPUT_MIN_%d", i);
		config.output_min[i] = defaults->get(string, config.output_min[i]);
		sprintf(string, "OUTPUT_MAX_%d", i);
		config.output_max[i] = defaults->get(string, config.output_max[i]);
	}

	mode = defaults->get("MODE", mode);
	CLAMP(mode, 0, COLORGRADE_MODES - 1);
	config.plot = defaults->get("PLOT", config.plot);
	config.split = defaults->get("SPLIT", config.split);
	config.boundaries();
	return 0;
}


int ColorgradeMain::save_defaults()
{
	char string[BCTEXTLEN];



	for(int j = 0; j < COLORGRADE_MODES; j++)
	{
		int total_points = config.points[j].total();
		sprintf(string, "TOTAL_POINTS_%d", j);
		defaults->update(string, total_points);
		ColorgradePoint *current = config.points[j].first;
		int number = 0;
		while(current)
		{
			sprintf(string, "INPUT_X_%d_%d", j, number);
			defaults->update(string, current->x);
			sprintf(string, "INPUT_Y_%d_%d", j, number);
			defaults->update(string, current->y);
			current = NEXT;
			number++;
		}
	}


	for(int i = 0; i < COLORGRADE_MODES; i++)
	{
		sprintf(string, "OUTPUT_MIN_%d", i);
		defaults->update(string, config.output_min[i]);
		sprintf(string, "OUTPUT_MAX_%d", i);
	   	defaults->update(string, config.output_max[i]);
	}

	defaults->update("MODE", mode);
	defaults->update("PLOT", config.plot);
	defaults->update("SPLIT", config.split);
	defaults->save();
	return 0;
}



void ColorgradeMain::save_data(KeyFrame *keyframe)
{
	FileXML output;

// cause data to be stored directly in text
	output.set_shared_string(keyframe->data, MESSAGESIZE);
	output.tag.set_title("COLORGRADE");

	char string[BCTEXTLEN];


	for(int i = 0; i < COLORGRADE_MODES; i++)
	{
		sprintf(string, "OUTPUT_MIN_%d", i);
		output.tag.set_property(string, config.output_min[i]);
		sprintf(string, "OUTPUT_MAX_%d", i);
	   	output.tag.set_property(string, config.output_max[i]);
//printf("ColorgradeMain::save_data %d %f %d\n", config.input_min[i], config.input_mid[i], config.input_max[i]);
	}

	output.tag.set_property("PLOT", config.plot);
	output.tag.set_property("SPLIT", config.split);
	output.append_tag();
	output.tag.set_title("/COLORGRADE");
	output.append_tag();
	output.append_newline();





	for(int j = 0; j < COLORGRADE_MODES; j++)
	{
		output.tag.set_title("POINTS");
		output.append_tag();
		output.append_newline();


		ColorgradePoint *current = config.points[j].first;
		while(current)
		{
			output.tag.set_title("POINT");
			output.tag.set_property("X", current->x);
			output.tag.set_property("Y", current->y);
			output.append_tag();
			output.append_newline();
			current = NEXT;
		}


		output.tag.set_title("/POINTS");
		output.append_tag();
		output.append_newline();
	}






	output.terminate_string();
}

void ColorgradeMain::read_data(KeyFrame *keyframe)
{
	FileXML input;

	input.set_shared_string(keyframe->data, strlen(keyframe->data));

	int result = 0;
	int current_input_mode = 0;


	while(!result)
	{
		result = input.read_tag();

		if(!result)
		{
			if(input.tag.title_is("COLORGRADE"))
			{
				char string[BCTEXTLEN];
				for(int i = 0; i < COLORGRADE_MODES; i++)
				{
					sprintf(string, "OUTPUT_MIN_%d", i);
					config.output_min[i] = input.tag.get_property(string, config.output_min[i]);
					sprintf(string, "OUTPUT_MAX_%d", i);
					config.output_max[i] = input.tag.get_property(string, config.output_max[i]);
//printf("ColorgradeMain::read_data %d %f %d\n", config.input_min[i], config.input_mid[i], config.input_max[i]);
				}
				config.plot = input.tag.get_property("PLOT", config.plot);
				config.split = input.tag.get_property("SPLIT", config.split);
			}
			else
			if(input.tag.title_is("POINTS"))
			{
				if(current_input_mode < COLORGRADE_MODES)
				{
					ColorgradePoints *points = &config.points[current_input_mode];
					while(points->last) 
						delete points->last;
					while(!result)
					{
						result = input.read_tag();
						if(!result)
						{
							if(input.tag.title_is("/POINTS"))
							{
								break;
							}
							else
							if(input.tag.title_is("POINT"))
							{
								points->insert(
									input.tag.get_property("X", 0.0),
									input.tag.get_property("Y", 0.0));
							}
						}
					}

				}
				current_input_mode++;
			}
		}
	}

	config.boundaries();

}

float ColorgradeMain::calculate_linear(float input, int subscript)
{
	int done = 0;
	float output;

 	if(input < 0)
 	{
 		output = 0;
 		done = 1;
 	}
 
 	if(input > 1)
 	{
 		output = 1;
 		done = 1;
 	}

	if(!done)
	{
		float x1 = 0;
		float y1 = 0;
		float x2 = 1;
		float y2 = 1;

// Get 2 points surrounding current position
		ColorgradePoints *points = &config.points[subscript];
		ColorgradePoint *current = points->first;
		done = 0;
		while(current && !done)
		{
			if(current->x > input)
			{
				x2 = current->x;
				y2 = current->y;
				done = 1;
			}
			else
				current = NEXT;
		}

		current = points->last;
		done = 0;
		while(current && !done)
		{
			if(current->x <= input)
			{
				x1 = current->x;
				y1 = current->y;
				done = 1;
			}
			else
				current = PREVIOUS;
		}

// Linear
		if(!EQUIV(x2 - x1, 0))
			output = (input - x1) * (y2 - y1) / (x2 - x1) + y1;
		else
			output = input * y2;

	}

	float output_min = config.output_min[subscript];
	float output_max = config.output_max[subscript];
	float output_left;
	float output_right;
	float output_linear;

// Compress output for value followed by channel
	output = output_min + 
		output * 
		(output_max - output_min);

	return output;
}

float ColorgradeMain::calculate_smooth(float input, int subscript)
{
	return calculate_linear(input, subscript);
}


void ColorgradeMain::calculate_colorgrade(VFrame *data)
{
}

void ColorgradeMain::calculate_automatic(VFrame *data)
{
}


int ColorgradeMain::calculate_use_opengl()
{
	return 0;
}


int ColorgradeMain::process_buffer(VFrame *frame,
	int64_t start_position,
	double frame_rate)
{
SET_TRACE
	int need_reconfigure = load_configuration();


SET_TRACE
	read_frame(frame, 
		0, 
		start_position, 
		frame_rate,
		0);

	if(!engine) engine = new ColorgradeEngine(this,
		get_project_smp() + 1,
		get_project_smp() + 1);
	this->input = frame;
	this->output = frame;

// Always plot to set the curves
	if(config.plot) send_render_gui(frame);

// Generate tables here.	
SET_TRACE
	for(int i = 0; i < 3; ++i)
		tabulate_curve(i);
SET_TRACE


// Apply colorgrade
	engine->process_packages(ColorgradeEngine::APPLY, input, 0);

SET_TRACE

	return 0;
}

void ColorgradeMain::tabulate_curve(int subscript)
{
	int i;
	if (!linear[subscript])
		linear[subscript] = new float[COLORGRADE_SLOTS];

	float *current_linear = linear[subscript];
	for (i = 0; i < COLORGRADE_SLOTS; ++i)
	{
		current_linear[i] = calculate_linear( (float) i/1000, subscript);
//		fprintf(stderr, "%i %i %f ; ", subscript, i, current_linear[i]);
	}
//	fprintf(stderr, "\n");
}

float ColorgradeMain::hue_translate_dep_value(float inhue, float inv)
{
	//float lhi, lho, hhi, hho; 
	float lho, hho; 
	//decide which curves to use
	if (inv < 0.5)
	{
		//LOW / MID

//		lhi = 0;
		lho = linear[COLORGRADE_LOW][ (int) round(inhue * 1000 / 6) ];

//		hhi = 0.5;
		hho = linear[COLORGRADE_MID][ (int) round(inhue * 1000 / 6) ];

		return 6 * (lho + inv * (hho - lho));
	}
	else
	{
		//MID / HIGH
		lho = linear[COLORGRADE_MID][ (int) round(inhue * 1000 / 6) ];

		hho = linear[COLORGRADE_HIGH][ (int) round(inhue * 1000 / 6) ];

		return 6 * (lho + inv * (hho - lho));
	}
	fprintf(stderr, "Error occured in hue_translate_dep_value(hue, v)!\n");
	return 0;
}











ColorgradePackage::ColorgradePackage()
 : LoadPackage()
{
}




ColorgradeUnit::ColorgradeUnit(ColorgradeEngine *server, 
	ColorgradeMain *plugin)
 : LoadClient(server)
{
	this->plugin = plugin;
	this->server = server;
}

ColorgradeUnit::~ColorgradeUnit()
{
}

void ColorgradeUnit::process_package(LoadPackage *package)
{
	ColorgradePackage *pkg = (ColorgradePackage*)package;

	if(server->operation == ColorgradeEngine::APPLY)
	{
		//let the work be done.
		

		/* *
		 * RED:		row[0]
		 * GREEN:	row[1]
		 * BLUE:	row[2]
		 *(ALPHA:	row[3])
		 *
		 * */

#define MAX3(a, b, c) \
		(a > b ? a : b) > c ? (a > b ? a : b) : c

#define MIN3(a, b, c) \
		(a < b ? a : b) < c ? (a < b ? a : b) : c

#define PROCESS(type, components) \
{ \
	for(int i = pkg->start; i < pkg->end; i++) \
	{ \
		type *row = (type*)input->get_rows()[i]; \
		for(int j = 0; j < w; j++) \
		{ \
			if ( plugin->config.split && ((j + i * w / h) < w) ) \
		    	continue; \
			min = MIN3((float) row[0]/0x100, (float) row[1]/0x100, (float) row[2]/0x100);\
			max = MAX3((float) row[0]/0x100, (float) row[1]/0x100, (float) row[2]/0x100);\
			v = max;\
			s = ( v == 0 ? 0 : (1 - min/v) );\
			hue = ( s == 0 ? 0 : (float) row[0]/0x100 == max ? (row[1]-row[2])/((max - min)*0x100) : (float) row[1]/0x100 == max ? 2 + (row[2]-row[0])/((max - min)*0x100) : 4 + (row[0]-row[1])/((max - min)*0x100) );\
			hue = hue < 0 ? hue + 6 : hue;\
			hue = hue >= 6 ? hue - 6 : hue;\
			/* v-- here the processing takes place */ \
			hue = plugin->hue_translate_dep_value(hue, v);\
			/* ^-- here the processing takes place */ \
			hue = hue < 0 ? hue + 6 : hue;\
			hue = hue >= 6 ? hue - 6 : hue;\
			if (s == 0)\
			{ \
				row[0] = row[1] = row[2] = v * 0x100;\
			} \
			else \
			{ \
				I = (int) floor(hue);\
				f = hue - I;\
				p = v * ( 1 - s );\
				q = v * ( 1 - s * f );\
				t = v * ( 1 - s * ( 1 - f ) );\
				switch (I) \
				{ \
					case 0: \
						row[0] = v * 0x100;\
						row[1] = t * 0x100;\
						row[2] = p * 0x100;\
						break;\
					case 1: \
						row[0] = q * 0x100;\
						row[1] = v * 0x100;\
						row[2] = p * 0x100;\
						break;\
					case 2: \
						row[0] = p * 0x100;\
						row[1] = v * 0x100;\
						row[2] = t * 0x100;\
						break;\
					case 3: \
						row[0] = p * 0x100;\
						row[1] = q * 0x100;\
						row[2] = v * 0x100;\
						break;\
					case 4: \
						row[0] = t * 0x100;\
						row[1] = p * 0x100;\
						row[2] = v * 0x100;\
						break;\
					case 5: \
						row[0] = v * 0x100;\
						row[1] = p * 0x100;\
						row[2] = q * 0x100;\
						break;\
					default: \
						fprintf(stderr, "Conversion error while converting HSV to RGB! %i %i %i, %f %f %f\n", row[0], row[1], row[2], hue, s, v);\
						row[0] = row[0]; \
						row[1] = row[1]; \
						row[2] = row[2]; \
				}\
			}\
			row += components; \
		} \
	} \
}

		VFrame *input = plugin->input;
		VFrame *output = plugin->output;
		int w = input->get_w();
		int h = input->get_h();

		int r, g, b, a, I;
		float hue, s, v, max, min, f, p, q, t;

		switch(input->get_color_model())
		{
			case BC_RGB888:
				PROCESS(unsigned char, 3)
				break;
			case BC_RGBA8888:
				PROCESS(unsigned char, 4)
				break;
			default:
				fprintf(stderr, "color model currently unsupported by Colorgrade!\n");
		}
	}
	else
	{
		fprintf(stderr, "ColorgradeUnit::process_package(LoadPackage *package) called with server->operation != ColorgradeEngine::APPLY\n");
	}
}




int ColorgradeMain::handle_opengl()
{
}


ColorgradeEngine::ColorgradeEngine(ColorgradeMain *plugin, 
	int total_clients, 
	int total_packages)
 : LoadServer(total_clients, total_packages)
{
	this->plugin = plugin;
}

void ColorgradeEngine::init_packages()
{
	switch(operation)
	{
		case APPLY:
			total_size = data->get_h();
			break;
		default:
			fprintf(stderr, "ColorgradeUnit::init_packages() called with operation != ColorgradeEngine::APPLY\n");
			return;
			break;
	}


	int package_size = (int)((float)total_size / 
			get_total_packages() + 1);
	int start = 0;

	for(int i = 0; i < get_total_packages(); i++)
	{
		ColorgradePackage *package = (ColorgradePackage*)get_package(i);
		package->start = total_size * i / get_total_packages();
		package->end = total_size * (i + 1) / get_total_packages();
	}

// Initialize clients here in case some don't get run.
	for(int i = 0; i < get_total_clients(); i++)
	{
		ColorgradeUnit *unit = (ColorgradeUnit*)get_client(i);
		//bzero if necessary
	}

}

LoadClient* ColorgradeEngine::new_client()
{
	return new ColorgradeUnit(this, plugin);
}

LoadPackage* ColorgradeEngine::new_package()
{
	return new ColorgradePackage;
}

void ColorgradeEngine::process_packages(int operation, VFrame *data, int do_value)
{
	this->data = data;
	this->operation = operation;
	this->do_value = do_value;
	LoadServer::process_packages();
}




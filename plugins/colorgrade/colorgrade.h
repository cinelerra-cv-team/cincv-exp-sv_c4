
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

#ifndef COLORGRADE_H
#define COLORGRADE_H


#include "colorgrade.inc"
#include "colorgradeconfig.h"
#include "colorgradewindow.inc"
#include "loadbalance.h"
#include "plugincolors.h"
#include "pluginvclient.h"


class ColorgradeMain : public PluginVClient
{
public:
	ColorgradeMain(PluginServer *server);
	~ColorgradeMain();

	int process_buffer(VFrame *frame,
		int64_t start_position,
		double frame_rate);
	int is_realtime();
	int load_defaults();
	int save_defaults();
	void save_data(KeyFrame *keyframe);
	void read_data(KeyFrame *keyframe);
	void update_gui();
	void render_gui(void *data);
	int calculate_use_opengl();
	int handle_opengl();

	PLUGIN_CLASS_MEMBERS(ColorgradeConfig, ColorgradeThread)

// Interpolate quantized transfer table to linear output
	float calculate_linear(float input, int mode);
	float calculate_smooth(float input, int subscript);
// Convert input to smoothed output by looking up in smooth table.
	float calculate_curve(float input);
// Calculate automatic settings
	void calculate_automatic(VFrame *data);
// Calculate colorgrade.
// Value is only calculated for preview.
	void calculate_colorgrade(VFrame *data);
// Calculate the linear, smoothed, lookup curves
	void tabulate_curve(int subscript);

	float hue_translate_dep_value(float inhue, float inv);




	YUV yuv;
	VFrame *input, *output;
	ColorgradeEngine *engine;
	int *lookup[COLORGRADE_MODES];
	float *smoothed[COLORGRADE_MODES];
	float *linear[COLORGRADE_MODES];
// No value applied to this
	int *preview_lookup[COLORGRADE_MODES];
	int *accum[COLORGRADE_MODES];
// Input point being dragged or edited
	int current_point;
// Current channel being viewed
	int mode;
	int dragging_point;
	int point_x_offset;
	int point_y_offset;
};

class ColorgradePackage : public LoadPackage
{
public:
	ColorgradePackage();
	int start, end;
};

class ColorgradeUnit : public LoadClient
{
public:
	ColorgradeUnit(ColorgradeEngine *server, ColorgradeMain *plugin);
	~ColorgradeUnit();
	void process_package(LoadPackage *package);
	ColorgradeEngine *server;
	ColorgradeMain *plugin;
	int *accum[5];
};

class ColorgradeEngine : public LoadServer
{
public:
	ColorgradeEngine(ColorgradeMain *plugin, 
		int total_clients, 
		int total_packages);
	void process_packages(int operation, VFrame *data, int do_value);
	void init_packages();
	LoadClient* new_client();
	LoadPackage* new_package();
	ColorgradeMain *plugin;
	int total_size;


	int operation;
	enum
	{
		COLORGRADE,
		APPLY
	};
	VFrame *data;
	int do_value;
};











#endif

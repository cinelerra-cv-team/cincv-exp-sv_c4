
/*
 * CINELERRA
 * Copyright (C) 2008 Adam Williams <broadcast at earthling dot net>
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

#ifndef PRESETS_H
#define PRESETS_H


#include "arraylist.h"
#include "filexml.inc"
#include "keyframe.inc"


// Front end for a DB of presets for all plugins

// A single preset
class PresetsDBKeyframe
{
public:
	PresetsDBKeyframe(const char *title);
	~PresetsDBKeyframe();

	void set_data(char *data);

	char *title;
	char *data;
};

// Presets for a single plugin
class PresetsDBPlugin
{
public:
	PresetsDBPlugin(const char *title);
	~PresetsDBPlugin();
	
	
	void load(FileXML *file);
	void save(FileXML *file);

// Get a preset by name
	PresetsDBKeyframe* get_keyframe(char *title);
// Create a new keyframe
	PresetsDBKeyframe* new_keyframe(char *title);
	void delete_keyframe(char *title);
// Load a preset into the keyframe
	void load_preset(char *preset_title, KeyFrame *keyframe);
	int preset_exists(char *preset_title);

	ArrayList<PresetsDBKeyframe*> keyframes;
	char *title;
};

class PresetsDB
{
public:
	PresetsDB();

// Load the database from the file.
	void load();
// Save the database to the file.
	void save();

// Get the total number of presets for a plugin
	int get_total_presets(char *plugin_title);
// Get the title of a preset
	char* get_preset_title(char *plugin_title, int number);
// Get the data for a preset
	char* get_preset_data(char *plugin_title, int number);
// Get a pluginDB by name
	PresetsDBPlugin* get_plugin(char *plugin_title);
// Create a pluginDB
	PresetsDBPlugin* new_plugin(char *plugin_title);
	void save_preset(char *plugin_title, char *preset_title, char *data);
	void delete_preset(char *plugin_title, char *preset_title);
// Load a preset into the keyframe
	void load_preset(char *plugin_title, char *preset_title, KeyFrame *keyframe);
	int preset_exists(char *plugin_title, char *preset_title);

private:
// Remove all plugin data
	void clear();

	ArrayList<PresetsDBPlugin*> plugins;
};



#endif





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

#ifndef LOADFILE_H
#define LOADFILE_H

#include "bcdialog.h"
#include "guicast.h"
#include "loadmode.inc"
#include "mainmenu.inc"
#include "mwindow.inc"

class LoadFileThread;
class LoadFileWindow;

class Load : public BC_MenuItem
{
public:
	Load(MWindow *mwindow, MainMenu *mainmenu);
	~Load();

	void create_objects();
	int handle_event();

	MWindow *mwindow;
	MainMenu *mainmenu;
	LoadFileThread *thread;
};

class LoadFileThread : public BC_DialogThread
{
public:
	LoadFileThread(MWindow *mwindow, Load *menuitem);
	~LoadFileThread();

	
	BC_Window* new_gui();
	void handle_done_event(int result);

	MWindow *mwindow;
	Load *load;
	int load_mode;
	LoadFileWindow *window;
};

class NewTimeline;
class NewConcatenate;
class AppendNewTracks;
class EndofTracks;
class ResourcesOnly;
class ReelWindow;
class OverwriteReel;

class LoadFileWindow : public BC_FileBox
{
public:
	LoadFileWindow(MWindow *mwindow, 
		LoadFileThread *thread,
		char *init_directory);
	~LoadFileWindow();

	void create_objects();
	int resize_event(int w, int h);

	LoadFileThread *thread;
	LoadMode *loadmode;
	MWindow *mwindow;
};



class LocateFileWindow : public BC_FileBox
{
public:
	LocateFileWindow(MWindow *mwindow, char *init_directory, char *old_filename);
	~LocateFileWindow();
	MWindow *mwindow;
};

class LoadPrevious : public BC_MenuItem, public Thread
{
public:
	LoadPrevious(MWindow *mwindow);
	int handle_event();
	void run();
	
	int set_path(char *path);
	
	MWindow *mwindow;
	Load *loadfile;
	char path[1024];
};

class LoadBackup : public BC_MenuItem
{
public:
	LoadBackup(MWindow *mwindow);
	int handle_event();
	MWindow *mwindow;
};

class ReelWindow : public BC_Window
{
public:
	ReelWindow(MWindow *mwindow);
	~ReelWindow();

	void create_objects();
	int resize_event(int w, int h);

	BC_OKButton *ok_button;
	BC_CancelButton *cancel_button;
	BC_Title *reel_name_title, *reel_number_title;
	BC_TextBox *reel_name, *reel_number;
	OverwriteReel *checkbox;
	int overwrite_reel;
	
	MWindow *mwindow;
};


class OverwriteReel : public BC_CheckBox
{
public:
	OverwriteReel(ReelWindow *lwindow,
		int x, int y, int value);
	
	int handle_event();
	
	ReelWindow *rwindow;
};

#endif

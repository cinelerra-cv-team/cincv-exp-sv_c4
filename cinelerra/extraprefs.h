
/*
 * CINELERRA
 * Copyright (C) 2008 Adam Williams <broadcast at earthling dot net>
 *                    Simeon Voelkel <simeon_voelkel at arcor dot de>
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

#ifndef EXTRAPREFS_H
#define EXTRAPREFS_H


#include "adeviceprefs.inc"
#include "formattools.inc"
#include "mwindow.inc"
#include "preferencesthread.h"
#include "recordprefs.inc"
#include "extraprefs.inc"
#include "vdeviceprefs.inc"

class ExtraPrefs : public PreferencesDialog
{
public:
	ExtraPrefs(MWindow *mwindow, PreferencesWindow *pwindow);
	~ExtraPrefs();

	void create_objects();

//	FormatTools *recording_format;
//	ADevicePrefs *audio_in_device;
//	VDevicePrefs *video_in_device;
//	MWindow *mwindow;
};



class StillImageUseDuration : public BC_CheckBox
{
public:
       StillImageUseDuration(PreferencesWindow *pwindow, int value, int x, int y);
       int handle_event();
       PreferencesWindow *pwindow;
};

class StillImageDuration : public BC_TextBox
{
public:
       StillImageDuration(PreferencesWindow *pwindow, int x, int y);
       int handle_event();
       PreferencesWindow *pwindow;
};



#endif

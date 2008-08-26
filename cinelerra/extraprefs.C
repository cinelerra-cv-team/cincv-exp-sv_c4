
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

#include "clip.h"
#include "edl.h"
#include "edlsession.h"
#include "extraprefs.h"
#include "formattools.h"
#include "new.h"
#include "language.h"
#include "mwindow.h"
#include "preferences.h"
#include "theme.h"




ExtraPrefs::ExtraPrefs(MWindow *mwindow, PreferencesWindow *pwindow)
 : PreferencesDialog(mwindow, pwindow)
{
	this->mwindow = mwindow;
}

ExtraPrefs::~ExtraPrefs()
{
//        if (rulerdumptargetmenu) delete rulerdumptargetmenu;
}

void ExtraPrefs::create_objects()
{
	int x, y, x2;
//	char string[BCTEXTLEN];
	BC_Resources *resources = BC_WindowBase::get_resources();

	x = mwindow->theme->preferencesoptions_x;
	y = mwindow->theme->preferencesoptions_y;

        x = 5;

        add_subwindow(new BC_Title(x, y, _("Images"), LARGEFONT, resources->text_default));
        y += 25;
        add_subwindow(new StillImageUseDuration(pwindow,
                pwindow->thread->edl->session->si_useduration,
                x,
                y));
        x += 260;
        y += 5;
        add_subwindow(new StillImageDuration(pwindow, x, y));
        x += 80;
        y += 5;
        add_subwindow(new BC_Title(x, y, _("Seconds")));

        y += 30;
	
        add_subwindow(new BC_Bar(5, y, 	get_w() - 10));
	y += 5;
        
        x = 5;
        add_subwindow(new BC_Title(x, y, _("Ruler Dump Target"), LARGEFONT, resources->text_default));
        y += 25;
        if (rulerdumptargetmenu)
        {
  //              delete rulerdumptargetmenu;
        }
        add_subwindow(rulerdumptargetmenu = new RulerDumpTargetMenu(pwindow, 
                x,
                y));
        rulerdumptargetmenu->create_objects();
        
        y += 30;
        
        add_subwindow(new BC_Bar(5, y, 	get_w() - 10));
	y += 5;

        x = 5;
        
}

/*int ExtraPrefs::delete_objects()
{
        reset();
        return 0;
}*/

void ExtraPrefs::reset()
{
//        if (rulerdumptargetmenu)
//                delete rulerdumptargetmenu;
//        rulerdumptargetmenu = 0;
}



StillImageUseDuration::StillImageUseDuration(PreferencesWindow *pwindow, int value, int x, int y)
 : BC_CheckBox(x, y, value, _("Import images with a duration of"))
{ 
       this->pwindow = pwindow; 
}

int StillImageUseDuration::handle_event()
{
       pwindow->thread->edl->session->si_useduration = get_value();
}

StillImageDuration::StillImageDuration(PreferencesWindow *pwindow, int x, int y)
 : BC_TextBox(x, y, 70, 1, pwindow->thread->edl->session->si_duration)
{
       this->pwindow = pwindow;
}

int StillImageDuration::handle_event()
{
       pwindow->thread->edl->session->si_duration = atof(get_text());
       return 1;
}


RulerDumpTargetMenu::RulerDumpTargetMenu(PreferencesWindow *pwindow, int x, int y) : BC_PopupMenu(x, y, 175, ruler_dump_target_to_string(pwindow->thread->edl->session->ruler_dump_target), 1)
{
        this->pwindow = pwindow;
        target = pwindow->thread->edl->session->ruler_dump_target;
}

RulerDumpTargetMenu::~RulerDumpTargetMenu()
{
        ;
}

char* RulerDumpTargetMenu::ruler_dump_target_to_string(int target)
{
        switch (target)
        {
                case DUMP_TO_NOWHERE:
                        sprintf(string, _("DUMP TO NOWHERE"));
                        break;
                case DUMP_TO_STDOUT:
                        sprintf(string, _("DUMP TO STDOUT"));
                        break;
        }
        return string;
}

void RulerDumpTargetMenu::create_objects()
{
        add_item(new RulerDumpTargetItem(this, _("DUMP TO NOWHERE"), DUMP_TO_NOWHERE));
        add_item(new RulerDumpTargetItem(this, _("DUMP TO STDOUT"), DUMP_TO_STDOUT));
}

int RulerDumpTargetMenu::handle_event()
{
        pwindow->thread->edl->session->ruler_dump_target = target;
        return 1;
}

RulerDumpTargetItem::RulerDumpTargetItem(RulerDumpTargetMenu *popup, const char *text, int target) : BC_MenuItem(text)
{
        this->popup = popup;
        this->target = target;
}

RulerDumpTargetItem::~RulerDumpTargetItem()
{
        ;
}

int RulerDumpTargetItem::handle_event()
{
        popup->set_text(get_text());
        popup->target = target;
        popup->handle_event();
        return 1;
}



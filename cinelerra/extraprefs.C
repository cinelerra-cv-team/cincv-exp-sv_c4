
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

int ExtraPrefs::create_objects()
{
	int x, y, x2;
//	char string[BCTEXTLEN];
	BC_Resources *resources = BC_WindowBase::get_resources();

	x = mwindow->theme->preferencesoptions_x;
	y = mwindow->theme->preferencesoptions_y;

        x = 5;


        add_subwindow(new BC_Title(x, y, _("Proxy"), LARGEFONT, resources->text_default));
        y += 25;
/*
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

*/
        x = 5;
        add_subwindow(new BC_Title(x, y, _("Resolution Level")));
        y += 25;
        if (prlmenu)
        {
  //              delete rulerdumptargetmenu;
        }
        add_subwindow(prlmenu = new ProxyResolutionLevelMenu(pwindow, 
                x,
                y));
        prlmenu->create_objects();
        
        y += 30;

        add_subwindow(new BC_Bar(5, y, 	get_w() - 10));
	y += 5;

        x = 5;
   
        return 0;
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


ProxyResolutionLevelMenu::ProxyResolutionLevelMenu(PreferencesWindow *pwindow, int x, int y) : BC_PopupMenu(x, y, 175, proxy_resolution_level_to_string(pwindow->thread->preferences->proxy_resolution_level), 1)
{
        this->pwindow = pwindow;
        level = pwindow->thread->preferences->proxy_resolution_level;
}

ProxyResolutionLevelMenu::~ProxyResolutionLevelMenu()
{
        ;
}

char* ProxyResolutionLevelMenu::proxy_resolution_level_to_string(int level)
{
        switch (level)
        {
                case 1:
                        sprintf(string, _("1 (original resolution)"));
                        break;
                case 2:
                        sprintf(string, _("2"));
                        break;
                case 3:
                        sprintf(string, _("3"));
                        break;
                case 4:
                        sprintf(string, _("4"));
                        break;
        }
        return string;
}

void ProxyResolutionLevelMenu::create_objects()
{
        add_item(new ProxyResolutionLevelItem(this, _("1 (original resolution)"), 1));
        add_item(new ProxyResolutionLevelItem(this, _("2"), 2));
        add_item(new ProxyResolutionLevelItem(this, _("3"), 3));
        add_item(new ProxyResolutionLevelItem(this, _("4"), 4));
}

int ProxyResolutionLevelMenu::handle_event()
{
        pwindow->thread->preferences->proxy_resolution_level = level;
        return 1;
}

ProxyResolutionLevelItem::ProxyResolutionLevelItem(ProxyResolutionLevelMenu *popup, char *text, int level) : BC_MenuItem(text)
{
        this->popup = popup;
        this->level = level;
}

ProxyResolutionLevelItem::~ProxyResolutionLevelItem()
{
        ;
}

int ProxyResolutionLevelItem::handle_event()
{
        popup->set_text(get_text());
        popup->level = level;
        popup->handle_event();
        return 1;
}

/*
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
*/



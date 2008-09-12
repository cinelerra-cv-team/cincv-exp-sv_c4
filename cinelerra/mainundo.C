
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

#include "asset.h"
#include "assets.h"
#include "edl.h"
#include "filexml.h"
#include "mainindexes.h"
#include "mainmenu.h"
#include "mainsession.h"
#include "mainundo.h"
#include "mwindow.h"
#include "mwindowgui.h"
#include "undostackitem.h"
#include "tracks.h"
#include <string.h>

// Minimum number of undoable operations on the undo stack
#define UNDOMINLEVELS 5
// Limits the bytes of memory used by the undo stack
#define UNDOMEMORY 50000000


class MainUndoStackItem : public UndoStackItem
{
public:
	MainUndoStackItem(MainUndo* undo, char* description,
			uint32_t load_flags, void* creator);
	virtual ~MainUndoStackItem();

	void set_data_before(char *data);
	virtual void undo();
	virtual int get_size();

private:
// type of modification
	unsigned long load_flags;
	
// data before the modification for undos
	char *data_before;          

	MainUndo *main_undo;

	void load_from_undo(FileXML *file, uint32_t load_flags);	// loads undo from the stringfile to the project
};


MainUndo::MainUndo(MWindow *mwindow)
{ 
	this->mwindow = mwindow;
	new_entry = 0;
	data_after = 0;
	last_update = new Timer;

// get the initial project so we have something that the last undo reverts to
	capture_state();
}

MainUndo::~MainUndo()
{
	delete [] data_after;
	delete last_update;
}

/*
void MainUndo::update_undo(char *description, uint32_t load_flags, 
		void *creator, int changes_made)
{
	if (ignore_push(description, load_flags, creator))
	{
		capture_state();
		return;
	}

	MainUndoStackItem* new_entry = new MainUndoStackItem(this, description, load_flags, creator);

// the old data_after is the state before the change
	new_entry->set_data_before(data_after);

	push_undo_item(new_entry);
}

void MainUndo::push_undo_item(UndoStackItem *item)
{
// clear redo_stack
	while (redo_stack.last)
		redo_stack.remove(redo_stack.last);

// move item onto undo_stack
	undo_stack.append(item);
	prune_undo();

	capture_state();

	mwindow->session->changes_made = 1;
   mwindow->gui->lock_window("MainUndo::update_undo_before");
   mwindow->gui->mainmenu->undo->update_caption(item->description);
   mwindow->gui->mainmenu->redo->update_caption("");
   mwindow->gui->unlock_window();
}

void MainUndo::capture_state()
{
	FileXML file;
	mwindow->edl->save_xml(mwindow->plugindb, 
		&file, 
		"",
		0,
		0);
	file.terminate_string();
	delete [] data_after;
	data_after = new char[strlen(file.string)+1];
	strcpy(data_after, file.string);
}

bool MainUndo::ignore_push(char *description, uint32_t load_flags, void* creator)
{
// ignore this push under certain conditions:
// - if nothing was undone
	bool ignore = redo_stack.last == 0 &&
// - if it is not the first push
		undo_stack.last &&
// - if it has the same description as the previous undo
		strcmp(undo_stack.last->description, description) == 0 &&
// - if it originates from the same creator
		undo_stack.last->creator == creator &&
// - if it follows closely after the previous undo
		last_update->get_difference() < 300 ; // millisec
	last_update->update();
	return ignore;
}

void MainUndo::push_state(char *description, uint32_t load_flags, void* creator)
{
	if (ignore_push(description, load_flags, creator))
	{
		capture_state();
	}
	else
	{
		MainUndoStackItem* new_entry = new MainUndoStackItem(this, description, load_flags, creator);
// the old data_after is the state before the change
		new_entry->set_data_before(data_after);
		push_undo_item(new_entry);
	}
	mwindow->session->changes_made = 1;
}

*/

void MainUndo::update_undo_entry(const char *description, 
	uint32_t load_flags,
	void *creator, 
	int changes_made)
{
	FileXML file;

	mwindow->edl->save_xml(mwindow->plugindb, 
		&file, 
		"",
		0,
		0);
	file.terminate_string();
	if(changes_made) mwindow->session->changes_made = 1;

// Remove all entries after current and create new one
	UndoStackItem *current = undo_stack->push();

	current->set_flags(load_flags);
	current->set_data(file.string);
	current->set_description((char*)description);
	current->set_creator(creator);
	current->set_filename(mwindow->session->filename);
//printf("MainUndo::update_undo_entry %d %p %s\n", __LINE__, current, current->get_filename());

// Can't undo only 1 record.
	if(undo_stack->total() > 1)
	{
		mwindow->gui->lock_window("MainUndo::update_undo");
		mwindow->gui->mainmenu->undo->update_caption(description);
		mwindow->gui->mainmenu->redo->update_caption("");
		mwindow->gui->unlock_window();
	}

// Update timer
	last_update->update();
}

void MainUndo::update_undo_before(const char *description, void *creator)
{
//printf("MainUndo::update_undo_before %d\n", __LINE__);
	if(undo_stack->current && !(undo_stack->number_of(undo_stack->current) % 2))
	{
		printf("MainUndo::update_undo_before \"%s\": must be on an after entry to do this.\n",
			description);
// Move up an entry to get back in sync
//		return;
	}


// Discard if creator matches previous before entry and within a time limit
	if(creator)
	{
		UndoStackItem *current = undo_stack->current;
// Currently on an after entry
		if(current)
		{
			current = PREVIOUS;
		}

// Now on a before entry
		if(current)
		{
			if(current->get_creator() == creator &&
				!strcmp(current->get_description(), description) &&
				last_update->get_difference() < UNDO_SPAN)
			{
// Before entry has same creator within minimum time.  Reuse it.
// Stack must point to the before entry
				undo_stack->current = current;
				return;
			}
		}
	}

// Append new entry after current position
	update_undo_entry("", 0, creator, 0);
}

void MainUndo::update_undo_after(const char *description, 
	uint32_t load_flags,
	int changes_made)
{
//printf("MainUndo::update_undo_after %d\n", __LINE__);
	if(undo_stack->number_of(undo_stack->current) % 2)
	{
		printf("MainUndo::update_undo_after \"%s\": must be on a before entry to do this.\n",
			description);
		return;
	}
>>>>>>> FETCH_HEAD:cinelerra/mainundo.C

	update_undo_entry(description, load_flags, 0, changes_made);

// Update the before entry flags
	UndoStackItem *current = undo_stack->last;
	if(current) current = PREVIOUS;
	if(current)
	{
		current->set_flags(load_flags);
		current->set_description((char*)description);
	}
}



int MainUndo::undo()
{
/*
	UndoStackItem* current_entry = undo_stack.last;

	if(current_entry)
	{
// move item to redo_stack
		undo_stack.remove_pointer(current_entry);
		current_entry->undo();
		redo_stack.append(current_entry);
		capture_state();

		if(mwindow->gui)
		{
			mwindow->gui->mainmenu->redo->update_caption(current_entry->description);

			if(undo_stack.last)
				mwindow->gui->mainmenu->undo->update_caption(undo_stack.last->description);
			else
				mwindow->gui->mainmenu->undo->update_caption("");
*/
	UndoStackItem *current = undo_stack->current;
	char after_description[BCTEXTLEN];
	after_description[0] = 0;

//printf("MainUndo::undo 1\n");
//undo_stack->dump();

// Rewind to an after entry
	if(current && !(undo_stack->number_of(current) % 2))
	{
		current = PREVIOUS;
	}

// Rewind to a before entry
	if(current && (undo_stack->number_of(current) % 2))
	{
		strcpy(after_description, current->get_description());
		current = PREVIOUS;
	}

// Now have an even number
	if(current)
	{
		undo_stack->current = current;
// Set the redo text to the current description
		if(mwindow->gui) 
			mwindow->gui->mainmenu->redo->update_caption(
				after_description);

		FileXML file;
		char *current_data = current->get_data();
		if(current_data)
		{
			file.read_from_string(current_data);
			load_from_undo(&file, current->get_flags());
//printf("MainUndo::undo %d %s\n", __LINE__, current->get_filename());
			mwindow->set_filename(current->get_filename());
			delete [] current_data;

// move current entry back one step
			undo_stack->pull();    


			if(mwindow->gui)
			{
// Now update the menu with the after entry
				current = PREVIOUS;
// Must be a previous entry to perform undo
				if(current)
					mwindow->gui->mainmenu->undo->update_caption(
						current->get_description());
				else
					mwindow->gui->mainmenu->undo->update_caption("");
			}
		}
	}


//undo_stack->dump();
	reset_creators();
	return 0;
}

int MainUndo::redo()
{
/*
	UndoStackItem* current_entry = redo_stack.last;
	
	if(current_entry)
	{
// move item to undo_stack
		redo_stack.remove_pointer(current_entry);
		current_entry->undo();
		undo_stack.append(current_entry);
		capture_state();

		if(mwindow->gui)
		{
			mwindow->gui->mainmenu->undo->update_caption(current_entry->description);
			
			if(redo_stack.last)
				mwindow->gui->mainmenu->redo->update_caption(redo_stack.last->description);
			else
				mwindow->gui->mainmenu->redo->update_caption("");
*/
	UndoStackItem *current = undo_stack->current;
//printf("MainUndo::redo 1\n");
//undo_stack->dump();

// Get 1st entry
	if(!current) current = undo_stack->first;

// Advance to a before entry
	if(current && (undo_stack->number_of(current) % 2))
	{
		current = NEXT;
	}

// Advance to an after entry
	if(current && !(undo_stack->number_of(current) % 2))
	{
		current = NEXT;
	}

	if(current)
	{
		FileXML file;
		char *current_data = current->get_data();
		undo_stack->current = current;

		if(current_data)
		{
			mwindow->set_filename(current->get_filename());
			file.read_from_string(current_data);
			load_from_undo(&file, current->get_flags());
			delete [] current_data;

			if(mwindow->gui)
			{
// Update menu
				mwindow->gui->mainmenu->undo->update_caption(current->get_description());

// Get next after entry
				current = NEXT;			
				if(current)
					current = NEXT;

				if(current)
					mwindow->gui->mainmenu->redo->update_caption(current->get_description());
				else
					mwindow->gui->mainmenu->redo->update_caption("");
			}
		}
	}
	reset_creators();
//undo_stack->dump();
	return 0;
}

// enforces that the undo stack does not exceed a size of UNDOMEMORY
// except that it always has at least UNDOMINLEVELS entries
void MainUndo::prune_undo()
{
	int size = 0;
	int levels = 0;

	UndoStackItem* i = undo_stack.last;
	while (i != 0 && (levels < UNDOMINLEVELS || size <= UNDOMEMORY))
	{
		size += i->get_size();
		++levels;
		i = i->previous;
	}

	if (i != 0)
	{
// truncate everything before and including i
		while (undo_stack.first != i)
			undo_stack.remove(undo_stack.first);
		undo_stack.remove(undo_stack.first);
	}
}





MainUndoStackItem::MainUndoStackItem(MainUndo* main_undo, char* description,
			uint32_t load_flags, void* creator)
{
	data_before = 0;
	this->load_flags = load_flags;
	this->main_undo = main_undo;
	set_description(description);
	set_creator(creator);
}

MainUndoStackItem::~MainUndoStackItem()
{
	delete [] data_before;
}

void MainUndoStackItem::set_data_before(char *data)
{
	data_before = new char[strlen(data) + 1];
	strcpy(data_before, data);
}

void MainUndoStackItem::undo()
{
// move the old data_after here
	char* before = data_before;
	data_before = 0;
	set_data_before(main_undo->data_after);

// undo the state
	FileXML file;

	file.read_from_string(before);
	load_from_undo(&file, load_flags);
}

int MainUndoStackItem::get_size()
{
	return data_before ? strlen(data_before) : 0;
}

// Here the master EDL loads 
void MainUndoStackItem::load_from_undo(FileXML *file, uint32_t load_flags)
{
	MWindow* mwindow = main_undo->mwindow;
	mwindow->edl->load_xml(mwindow->plugindb, file, load_flags);
	for(Asset *asset = mwindow->edl->assets->first;
		asset;
		asset = asset->next)
	{
		mwindow->mainindexes->add_next_asset(0, asset);
	}
	mwindow->mainindexes->start_build();
}


void MainUndo::reset_creators()
{
	for(UndoStackItem *current = undo_stack.first;
		current;
		current = NEXT)
	{
		current->set_creator(0);
	}
}




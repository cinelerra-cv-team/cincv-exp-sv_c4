
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

#ifndef BC_WINDOWBASE_INC
#define BC_WINDOWBASE_INC

class BC_WindowBase;
class BC_WindowList;
/*
 * Change the following lines to change the size of most char arrays. 
 * 
 * This might be what you want to do if you have to use much longer 
 * titles or generally strings that are longer than 1KB or 1023 char.
 *
 * As 100KB are peanuts in 2007 i have changed the length of BCTEXTLEN
 * from 1024 to 102400. If you should get into troubles with this 
 * setting, set it to what you want.
 *
 **/
#define BCTEXTLEN 1024
#define OLTEXTLEN 1024
#define ODTEXTLEN 1024
/*
 * BCTEXTLEN is inherited from the previous git-version, for details 
 * the output of git blame: ^1c535de (Herman Robak      2003-06-16 
 * 20:00:48 +0000 17) #define BCTEXTLEN 1024
 *
 * OLTEXTLEN is used where it is evident that these char* have to 
 * have the same length.
 *
 * ODTEXTLEN is used where I wasn't sure why it is necessary to change 
 * the length of the array but otherwise cinelerra crashed unexpected.
 * (what might still happen but was unlikely here)
 *
 **/

// Milliseconds before cursor disappears during video playback
#define VIDEO_CURSOR_TIMEOUT 2000

#endif

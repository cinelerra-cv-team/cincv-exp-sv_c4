
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

#ifndef COLORGRADE_INC
#define COLORGRADE_INC





// modes
#define COLORGRADE_MODES 3
#define COLORGRADE_LOW 0
#define COLORGRADE_MID 1
#define COLORGRADE_HIGH 2

// Number of divisions in colorgrade.  
// 1001 as we use indices {0, 1, ... 1000}
#define COLORGRADE_SLOTS 1001

#define FLOAT_RANGE 1
// Minimum value in percentage
#define COLORGRADE_MIN 0
#define MIN_INPUT 0
// Maximum value in percentage
#define COLORGRADE_MAX 1
#define MAX_INPUT 1

#define PRECISION 0.001
#define DIGITS 3

#define BOX_SIZE 10


class ColorgradeEngine;
class ColorgradeMain;


#endif

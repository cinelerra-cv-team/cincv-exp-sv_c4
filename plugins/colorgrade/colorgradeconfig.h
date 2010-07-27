
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

#ifndef COLORGRADECONFIG_H
#define COLORGRADECONFIG_H


#include "colorgrade.inc"
#include "colorgradeconfig.inc"
#include "linklist.h"
#include <stdint.h>

class ColorgradePoint : public ListItem<ColorgradePoint>
{
public:
	ColorgradePoint();
	~ColorgradePoint();

	int equivalent(ColorgradePoint *src);
	float x, y;
};


class ColorgradePoints : public List<ColorgradePoint>
{
public:
	ColorgradePoints();
	~ColorgradePoints();

// Insert new point
	ColorgradePoint* insert(float x, float y);
	int equivalent(ColorgradePoints *src);
	void boundaries();
	void copy_from(ColorgradePoints *src);
	void interpolate(ColorgradePoints *prev, 
		ColorgradePoints *next,
		double prev_scale,
		double next_scale);
};

class ColorgradeConfig
{
public:
	ColorgradeConfig();

	int equivalent(ColorgradeConfig &that);
	void copy_from(ColorgradeConfig &that);
	void interpolate(ColorgradeConfig &prev, 
		ColorgradeConfig &next, 
		int64_t prev_frame, 
		int64_t next_frame, 
		int64_t current_frame);
// Used by constructor and reset button
	void reset();
	void reset_points();
	void boundaries();
	void dump();

// Range 0 - 1.0
// Input points
	ColorgradePoints points[COLORGRADE_MODES];
// Output points
	float output_min[COLORGRADE_MODES];
	float output_max[COLORGRADE_MODES];
	int plot;
	int split;
};


#endif




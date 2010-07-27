
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

#include "clip.h"
#include "colorgradeconfig.h"
#include "units.h"

#include <math.h>





ColorgradePoint::ColorgradePoint()
 : ListItem<ColorgradePoint>()
{
}

ColorgradePoint::~ColorgradePoint()
{
}

int ColorgradePoint::equivalent(ColorgradePoint *src)
{
	return EQUIV(x, src->x) && EQUIV(y, src->y);
}




ColorgradePoints::ColorgradePoints()
 : List<ColorgradePoint>()
{
}

ColorgradePoints::~ColorgradePoints()
{
}

ColorgradePoint* ColorgradePoints::insert(float x, float y)
{
	ColorgradePoint *current = first;

// Get existing point after new point
	while(current)
	{
		if(current->x > x)
			break;
		else
			current = NEXT;
	}

// Insert new point before current point
	ColorgradePoint *new_point = new ColorgradePoint;
	if(current)
	{
		insert_before(current, new_point);
	}
	else
// Append new point to list
	{
		append(new_point);
	}

	new_point->x = x;
	new_point->y = y;


	return new_point;
}

void ColorgradePoints::boundaries()
{
	ColorgradePoint *current = first;
	while(current)
	{
		CLAMP(current->x, 0.0, 1.0);
		CLAMP(current->y, 0.0, 1.0);
		current = NEXT;
	}
}

int ColorgradePoints::equivalent(ColorgradePoints *src)
{
	ColorgradePoint *current_this = first;
	ColorgradePoint *current_src = src->first;
	while(current_this && current_src)
	{
		if(!current_this->equivalent(current_src)) return 0;
		current_this = current_this->next;
		current_src = current_src->next;
	}

	if(!current_this && current_src ||
		current_this && !current_src)
		return 0;
	return 1;
}

void ColorgradePoints::copy_from(ColorgradePoints *src)
{
	while(last)
		delete last;
	ColorgradePoint *current = src->first;
	while(current)
	{
		ColorgradePoint *new_point = new ColorgradePoint;
		new_point->x = current->x;
		new_point->y = current->y;
		append(new_point);
		current = NEXT;
	}
}

void ColorgradePoints::interpolate(ColorgradePoints *prev, 
	ColorgradePoints *next,
	double prev_scale,
	double next_scale)
{
	ColorgradePoint *current = first;
	ColorgradePoint *current_prev = prev->first;
	ColorgradePoint *current_next = next->first;

	while(current && current_prev && current_next)
	{
		current->x = current_prev->x * prev_scale +
			current_next->x * next_scale;
		current->y = current_prev->y * prev_scale +
			current_next->y * next_scale;
		current = NEXT;
		current_prev = current_prev->next;
		current_next = current_next->next;
	}
}














ColorgradeConfig::ColorgradeConfig()
{
	plot = 1;
	split = 0;
	reset();
}

void ColorgradeConfig::reset()
{
	reset_points();

	
	for(int i = 0; i < COLORGRADE_MODES; i++)
	{
		output_min[i] = 0.0;
		output_max[i] = 1.0;
	}
}

void ColorgradeConfig::reset_points()
{
	for(int i = 0; i < COLORGRADE_MODES; i++)
	{
			while(points[i].last) delete points[i].last;
	}
}


void ColorgradeConfig::boundaries()
{
	for(int i = 0; i < COLORGRADE_MODES; i++)
	{
		points[i].boundaries();
		CLAMP(output_min[i], MIN_INPUT, MAX_INPUT);
		CLAMP(output_max[i], MIN_INPUT, MAX_INPUT);
		output_min[i] = Units::quantize(output_min[i], PRECISION);
		output_max[i] = Units::quantize(output_max[i], PRECISION);
	}
}

int ColorgradeConfig::equivalent(ColorgradeConfig &that)
{
	for(int i = 0; i < COLORGRADE_MODES; i++)
	{
		if(!points[i].equivalent(&that.points[i]) ||
			!EQUIV(output_min[i], that.output_min[i]) ||
			!EQUIV(output_max[i], that.output_max[i])) return 0;
	}

	if(plot != that.plot ||
		split != that.split) return 0;
	return 1;
}

void ColorgradeConfig::copy_from(ColorgradeConfig &that)
{
	for(int i = 0; i < COLORGRADE_MODES; i++)
	{
		points[i].copy_from(&that.points[i]);
		output_min[i] = that.output_min[i];
		output_max[i] = that.output_max[i];
	}

	plot = that.plot;
	split = that.split;
}

void ColorgradeConfig::interpolate(ColorgradeConfig &prev, 
	ColorgradeConfig &next, 
	int64_t prev_frame, 
	int64_t next_frame, 
	int64_t current_frame)
{
	double next_scale = (double)(current_frame - prev_frame) / (next_frame - prev_frame);
	double prev_scale = 1.0 - next_scale;

	for(int i = 0; i < COLORGRADE_MODES; i++)
	{
		points[i].interpolate(&prev.points[i], &next.points[i], prev_scale, next_scale);
		output_min[i] = prev.output_min[i] * prev_scale + next.output_min[i] * next_scale;
		output_max[i] = prev.output_max[i] * prev_scale + next.output_max[i] * next_scale;
	}

	plot = prev.plot;
	split = prev.split;
}


void ColorgradeConfig::dump()
{
	for(int j = 0; j < COLORGRADE_MODES; j++)
	{
		printf("ColorgradeConfig::dump mode=%d plot=%d split=%d\n", j, plot, split);
		ColorgradePoints *points = &this->points[j];
		ColorgradePoint *current = points->first;
		while(current)
		{
			printf("%f,%f ", current->x, current->y);
			fflush(stdout);
			current = NEXT;
		}
		printf("\n");
	}
}




#include "autos.h"
#include "clip.h"
#include "edl.h"
#include "filexml.h"
#include "floatauto.h"
#include "floatautos.h"
#include "localsession.h"
#include "transportque.inc"
#include "automation.inc"

FloatAuto::FloatAuto(EDL *edl, FloatAutos *autos)
 : Auto(edl, (Autos*)autos)
{
	val = 0;
	ctrl_in_val  = 0;
	ctrl_out_val = 0;
	ctrl_in_pos  = 0;
	ctrl_out_pos = 0;
	pos_valid    = -1; //"dirty"
	tangent_mode = SMOOTH;
//  note: in most cases the tangent_mode-value    
//        is set by the method fill_from_template()
}

FloatAuto::~FloatAuto()
{
	// as we are going away, the neighbouring float auto nodes 
	// need to re-adjust their ctrl point positions and tangents
	if (autos && autos->get_type()==AUTOMATION_TYPE_FLOAT)
	{
		if (next)
			((FloatAuto*)next)->tangent_dirty();
		if (previous)
			((FloatAuto*)previous)->tangent_dirty();
	}
}

int FloatAuto::operator==(Auto &that)
{
	return identical((FloatAuto*)&that);
}


int FloatAuto::operator==(FloatAuto &that)
{
	return identical((FloatAuto*)&that);
}


int FloatAuto::identical(FloatAuto *src)
{
	return EQUIV(val, src->val) &&
		EQUIV(ctrl_in_val,  src->ctrl_in_val) &&
		EQUIV(ctrl_out_val, src->ctrl_out_val);
		// ctrl positions ignored, as they may depend on neighbours
		// tangent_mode is ignored, no recalculations
}

/* Note: the following is essentially display-code and has been moved to: 
 *  TrackCanvas::value_to_percentage(float auto_value, int autogrouptype)
 * 
float FloatAuto::value_to_percentage()
{
}
float FloatAuto::value_to_percentage()
{
}
float FloatAuto::value_to_percentage()
{
}
*/


void FloatAuto::copy_from(Auto *that)
{
	copy_from((FloatAuto*)that);
}

void FloatAuto::copy_from(FloatAuto *that)
{
	Auto::copy_from(that);
	this->val          = that->val;
	this->ctrl_in_val  = that->ctrl_in_val;
	this->ctrl_out_val = that->ctrl_out_val;
	this->ctrl_in_pos  = that->ctrl_in_pos;
	this->ctrl_out_pos = that->ctrl_out_pos;
	this->tangent_mode = that->tangent_mode;
// note: literate copy, no recalculations    
}

void FloatAuto::fill_from_template(Auto *other, int64_t new_position)
// using the tangent of the template doesn't make sense 
// in the majority of cases. Instead, we use automatically 
// smoothed tangents as a starting point. 
{
	Auto::fill_from_template(other,new_position);
	FloatAuto *templ = (FloatAuto*)other;
	this->val = templ->val;
	
	if(templ->tangent_mode==FREE || templ->tangent_mode==TFREE)
	{
		this->tangent_mode=SMOOTH;
	}
	else
	{
		this->tangent_mode = templ->tangent_mode;
	}
	adjust_ctrl_positions(); // implies adjust_tangents()
	
}

int FloatAuto::interpolate_from(Auto *a1, Auto *a2, int64_t pos)
{
	// set this->value using bézier interpolation if possible
	if (a1 && a2 && (a1->autos == a2->autos) &&
	    a1->autos && a1->autos->get_type()==AUTOMATION_TYPE_FLOAT
		)
	{	
		FloatAuto *left = (FloatAuto*)a1;
		FloatAuto *right = (FloatAuto*)a2;
		fill_from_template(left,pos);
		this->value( FloatAutos::calculate_bezier(left,right,pos) );
		return 1; //return true: really interpolated...
	}
	else return Auto::interpolate_from(a1,a2,pos);
}


void FloatAuto::change_tangent_mode(t_mode new_mode)
{
	if (new_mode==TFREE && !(ctrl_in_pos && ctrl_out_pos))
		new_mode = FREE; // only if tangents on both sides...

	tangent_mode = new_mode;
	adjust_tangents();
}

void FloatAuto::toggle_tangent_mode()
{
	switch (tangent_mode) {
		case SMOOTH:	change_tangent_mode(TFREE);  break;
		case LINEAR:	change_tangent_mode(FREE);   break;
		case TFREE :	change_tangent_mode(LINEAR); break;
		case FREE  :	change_tangent_mode(SMOOTH); break;
	}
}


void FloatAuto::value(float newval)
{
	this->val=newval; 
	this->adjust_tangents();
	if (previous) ((FloatAuto*)previous)->adjust_tangents();
	if (next)     ((FloatAuto*)next)->adjust_tangents();
} 

void FloatAuto::control_in_value(float newval)
{
	switch (tangent_mode) {
		case TFREE:	ctrl_out_val = ctrl_out_pos*newval/ctrl_in_pos;
		case FREE:	ctrl_in_val = newval;
		default:	return;
}	} 

void FloatAuto::control_out_value(float newval)
{
	switch (tangent_mode) {
		case TFREE:	ctrl_in_val = ctrl_in_pos*newval/ctrl_out_pos;
		case FREE:	ctrl_out_val=newval;
		default:	return;
}	} 



inline int sgn(float val) { return (val==0)? 0 : (val < 0) ? -1 : 1; }

inline float wighted_mean(float v1,float v2,float w1,float w2){
	if ( 0.000001 > fabs(w1+w2) )
		return 0;
	else
		return (w1*v1 + w2*v2)/(w1+w2);
}




void FloatAuto::adjust_tangents()
{ // recalculates tangents if current mode 
  // implies automatic adjustment of tangents
  if (!autos) return;

	if (tangent_mode==SMOOTH)
	{
		// normally, one would use the slope of chord between the neighbours.
		// but this could cause the curve to overshot extremal automation nodes.
		// (e.g when setting a fade node at zero, the curve could go negative)
		// we can interpret the slope of chord as a weighted mean value, where
		// the lenght of the intervalles is used as weight; we just use other 
		// weights: intervall length /and/ reciprocal of slope. so, if the
		// connection to one of the neighbours has very low slope this will
		// dominate the calculated tangent slope at this automation node.
		// if the slope goes byond the zero line, e.g if left connection
		// has positive and right connetcion has negative slope, then
		// we force the calculated tangent to be horizontal.
		float g,dxl,dxr,gl,gr;
		calc_slope(previous,this,gl,dxl);
		calc_slope(this,next,gr,dxr);
		
		if ( 0 < sgn(gl)*sgn(gr) )
		{	
			float wl = fabs(dxl/gl);
			float wr = fabs(dxr/gr);
			g = wighted_mean(gl,gr,wl,wr);
		}
		else g = 0; // fixed hoizontal tangent
		
		ctrl_in_val = g*ctrl_in_pos;
		ctrl_out_val = g*ctrl_out_pos;
	}
	
	else
	if (tangent_mode==LINEAR)
 	{
		float g,dx;
		if (previous)
		{	calc_slope(this,previous,g,dx);
			ctrl_in_val = g*dx/3;
		}
		if (next)
		{	calc_slope(this,next,g,dx);
			ctrl_out_val = g*dx/3;
	}	}
	
	else
	if (tangent_mode==TFREE && ctrl_in_pos && ctrl_out_pos)
 	{
		float gl = ctrl_in_val/ctrl_in_pos;
		float gr = ctrl_out_val/ctrl_out_pos;
		float wl = fabs(ctrl_in_val);
		float wr = fabs(ctrl_out_val);
		float g = wighted_mean(gl,gr,wl,wr);
		
		ctrl_in_val = g*ctrl_in_pos;
		ctrl_out_val = g*ctrl_out_pos;
	}
}

inline void FloatAuto::calc_slope(Auto *l, Auto *r, float &g, float &dx)
{
	g=0; dx=0;
	if (!l || !r) return;
	
	FloatAuto *prev = (FloatAuto*)l;
	FloatAuto *next = (FloatAuto*)r;
	dx = next->position - prev->position;
	float dy = next->val - prev->val;
	g = dx==0? 0 : dy/dx;
}


void FloatAuto::adjust_ctrl_positions(FloatAuto *prev, FloatAuto *next)
{ // recalculates location of ctrl points to be
  // allways 1/3 and 2/3 of the distance to the
  // next neighbours. The reason is: for this special
  // distance the bézier function yields x(t) = t, i.e.
  // we can use the y(t) as if it was a simple function y(x).
  
  // This adjustment is done only on demand and involves 
  // updating neighbours and adjust_tangents() as well.
	if (!prev && !next)
	{	// use current siblings
		prev = (FloatAuto*)this->previous;
		next = (FloatAuto*)this->next;
	}	
	
	if (prev)
	{	set_ctrl_positions(prev,this);
		prev->adjust_tangents();
	}
	else // disable tangent on left side
		ctrl_in_pos = 0;
		
	if (next) 
	{	set_ctrl_positions(this,next);
		next->adjust_tangents();
	}
	else // disable right tangent
		ctrl_out_pos = 0;
		
	this->adjust_tangents();
}


inline void FloatAuto::set_ctrl_positions(FloatAuto *prev, FloatAuto* next)
{
	int64_t distance = next->position - prev->position;
	prev->ctrl_out_pos = +distance/3;
	next->ctrl_in_pos  = -distance/3;
}



void FloatAuto::adjust_to_new_coordinates(int64_t position, float value)
{ // define new position and value in one step, do necessary re-adjustments
	this->val = value;
	this->position = position;
	adjust_ctrl_positions();
}



int FloatAuto::value_to_str(char *string, float value)
{
	int j = 0, i = 0;
	if(value > 0) 
		sprintf(string, "+%.2f", value);
	else
		sprintf(string, "%.2f", value);

// fix number
	if(value == 0)
	{
		j = 0;
		string[1] = 0;
	}
	else
	if(value < 1 && value > -1) 
	{
		j = 1;
		string[j] = string[0];
	}
	else 
	{
		j = 0;
		string[3] = 0;
	}
	
	while(string[j] != 0) string[i++] = string[j++];
	string[i] = 0;

	return 0;
}

void FloatAuto::copy(int64_t start, int64_t end, FileXML *file, int default_auto)
{
	file->tag.set_title("AUTO");
	if(default_auto)
		file->tag.set_property("POSITION", 0);
	else
		file->tag.set_property("POSITION", position - start);
	file->tag.set_property("VALUE", val);
	file->tag.set_property("CONTROL_IN_VALUE",  ctrl_in_val/2.0 ); // compatibility, see below
	file->tag.set_property("CONTROL_OUT_VALUE", ctrl_out_val/2.0 );
	file->tag.set_property("TANGENT_MODE", (int)tangent_mode);
	file->append_tag();
	file->tag.set_title("/AUTO");
	file->append_tag();
	file->append_newline();
}

void FloatAuto::load(FileXML *file)
{
	val = file->tag.get_property("VALUE", val);
	ctrl_in_val = file->tag.get_property("CONTROL_IN_VALUE", ctrl_in_val);
	ctrl_out_val = file->tag.get_property("CONTROL_OUT_VALUE", ctrl_out_val);
	tangent_mode = (t_mode)file->tag.get_property("TANGENT_MODE", FREE ); 
	
	// Compatibility to old session data format:
	// Versions previous to the bezier auto patch (Jun 2006) aplied a factor 2
	// to the y-coordinates of ctrl points while calculating the bezier function.
	// To retain compatibility, we now aply this factor while loading
	ctrl_in_val *= 2.0;
	ctrl_out_val *= 2.0;
    
// restore ctrl positions and adjust tangents if necessary
	adjust_ctrl_positions();
}

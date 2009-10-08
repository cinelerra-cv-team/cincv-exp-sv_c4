#include "auto.h"
#include "autos.h"
#include "filexml.h"

Auto::Auto()
 : ListItem<Auto>()
{
	this->edl = 0;
	this->autos = 0;
	position = 0;
	skip = 0;
	WIDTH = 10;
	HEIGHT = 10;
	is_default = 0;
}

Auto::Auto(EDL *edl, Autos *autos)
 : ListItem<Auto>()
{
	this->edl = edl;
	this->autos = autos;
	position = 0;
	skip = 0;
	WIDTH = 10;
	HEIGHT = 10;
	is_default = 0;
}

Auto& Auto::operator=(Auto& that)
{
	copy_from(&that);
	return *this;
}

int Auto::operator==(Auto &that)
{
	printf("Auto::operator== called\n");
	return 0;
}

void Auto::copy(int64_t start, int64_t end, FileXML *file, int default_only)
{
	printf("Auto::copy called\n");
}

void Auto::copy_from(Auto *that)
{
	this->position = that->position;
}

void Auto::fill_from_template(Auto *other, int64_t new_position)
{   // may be overridden to handle the templating as a special case (see FloatAuto)
	copy_from(other);
	position = new_position;
}

int Auto::interpolate_from(Auto *a1, Auto *a2, int64_t position)
{
	fill_from_template(a1,position);
	return 0;
}

void Auto::load(FileXML *xml)
{
	printf("Auto::load\n");
}



#ifndef AUTO_H
#define AUTO_H

#include "auto.inc"
#include "edl.inc"
#include "guicast.h"
#include "filexml.inc"
#include "autos.inc"






// The default constructor is used for menu effects.

class Auto : public ListItem<Auto>
{
public:
	Auto();
	Auto(EDL *edl, Autos *autos);
	virtual ~Auto() {};

	virtual Auto& operator=(Auto &that);
	virtual int operator==(Auto &that);
	/* literate copy */
	virtual void copy_from(Auto *that);
	/* use the other as Template. 
	 * Defaults to copy_from while retaining position */
	virtual void fill_from_template(Auto *other, int64_t new_position);
	/* for interpolation creation */
	/* if not possible, fill from a1 and return 0*/ 	
	virtual int interpolate_from(Auto *a1, Auto *a2, int64_t position); 
	virtual void copy(int64_t start, int64_t end, FileXML *file, int default_only);

	virtual void load(FileXML *file);

	virtual void get_caption(char *string) {};


	int skip;       // if added by selection event for moves
	EDL *edl;
	Autos *autos;
	int WIDTH, HEIGHT;
// Units native to the track
	int is_default;
	int64_t position;

private:
	virtual int value_to_str(char *string, float value) {};
};



#endif

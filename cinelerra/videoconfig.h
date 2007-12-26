#ifndef VIDEOCONFIG_H
#define VIDEOCONFIG_H

#include "bchash.inc"






// REMOVE
// This is obsolete.
class VideoConfig
{
public:
	VideoConfig();
	~VideoConfig();
	
	VideoConfig& operator=(VideoConfig &that);
	int load_defaults(BC_Hash *defaults);
	int save_defaults(BC_Hash *defaults);

// Input
	int video_in_driver;
	char v4l_in_device[ODTEXTLEN];
	char lml_in_device[ODTEXTLEN];
	char screencapture_display[ODTEXTLEN];
	int vfirewire_in_port, vfirewire_in_channel;
// number of frames to read from device during video recording.
	int capture_length;   

// Output
	int video_out_driver;
	char lml_out_device[ODTEXTLEN];
};

#endif

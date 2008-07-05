#include "bcdisplayinfo.h"
#include "clip.h"
#include "bchash.h"
#include "filexml.h"
#include "language.h"
#include "picon_png.h"
#include "spectrogram.h"
#include "units.h"
#include "vframe.h"


#include <string.h>



REGISTER_PLUGIN(Spectrogram)

// 16384 is a decent value 
// Smaller values are more accurate with time, while larger values 
// offer more accuracy in frequency. This should be easily set by the user
// eventually...
#define WINDOW_SIZE 4096
#define HALF_WINDOW (WINDOW_SIZE/2)

// Step can be seen as how zoomed in the spectrometer is. The higher the
// step size, the more zoomed in on the x-axis the GUI will be. An unfortunate
// side effect is that large values of STEP slow down the playback.
#define STEP 8


SpectrogramConfig::SpectrogramConfig()
{
	level = 0.0;
}







SpectrogramLevel::SpectrogramLevel(Spectrogram *plugin, int x, int y)
 : BC_FPot(x, y, plugin->config.level, -100, 0)
{
	this->plugin = plugin;
}

int SpectrogramLevel::handle_event()
{
	plugin->config.level = get_value();
	plugin->send_configure_change();
	return 1;
}











SpectrogramWindow::SpectrogramWindow(Spectrogram *plugin, int x, int y)
 : BC_Window(plugin->gui_string, 
 	x, 
	y, 
	640, 
	480, 
	640, 
	480,
	0, 
	0,
	1)
{
	this->plugin = plugin;
}

SpectrogramWindow::~SpectrogramWindow()
{
}

void SpectrogramWindow::create_objects()
{
	int x = 60, y = 10;
	int divisions = 20;
	char string[BCTEXTLEN];

	add_subwindow(canvas = new BC_SubWindow(x, 
		y, 
		get_w() - x - 10, 
		get_h() - 50 - y,
		BLACK));
	x = 10;

	for(int i = 0; i <= divisions; i++)
	{
		y = (int)((float)(canvas->get_h() - 10) / divisions * i) + 10;
		sprintf(string, "%d", 
			Freq::tofreq((int)((float)TOTALFREQS / divisions * (divisions - i))));
		add_subwindow(new BC_Title(x, y, string));
	}

	x = canvas->get_x();
	y = canvas->get_y() + canvas->get_h() + 5;

	add_subwindow(new BC_Title(x, y + 10, _("Level:")));
	add_subwindow(level = new SpectrogramLevel(plugin, x + 50, y));

	show_window();
	flush();
}

WINDOW_CLOSE_EVENT(SpectrogramWindow)


void SpectrogramWindow::update_gui()
{
	level->update(plugin->config.level);
}










PLUGIN_THREAD_OBJECT(Spectrogram, SpectrogramThread, SpectrogramWindow)









SpectrogramFFT::SpectrogramFFT(Spectrogram *plugin)
 : CrossfadeFFT()
{
	this->plugin = plugin;
}

SpectrogramFFT::~SpectrogramFFT()
{
}


int SpectrogramFFT::signal_process()
{
	double level = DB::fromdb(plugin->config.level);
	for(int i = 0; i < HALF_WINDOW; i++)
	{
		plugin->data[i] += level *
			sqrt(freq_real[i] * freq_real[i] + 
				freq_imag[i] * freq_imag[i]);
	}

	plugin->total_windows++;
	return 0;
}

int SpectrogramFFT::read_samples(int64_t output_sample, 
	int samples, 
	double *buffer)
{
	return plugin->read_samples(buffer,
		0,
		plugin->get_samplerate(),
		output_sample,
		samples);
}









Spectrogram::Spectrogram(PluginServer *server)
 : PluginAClient(server), column(0), background(0)
{
	reset();
	PLUGIN_CONSTRUCTOR_MACRO
}

Spectrogram::~Spectrogram()
{
	PLUGIN_DESTRUCTOR_MACRO

	if(fft) delete fft;
	if(data) delete [] data;
}


void Spectrogram::reset()
{
	thread = 0;
	fft = 0;
	done = 0;
	data = 0;
}


char* Spectrogram::plugin_title() { return N_("Spectrogram"); }
int Spectrogram::is_realtime() { return 1; }

int Spectrogram::process_buffer(int64_t size, 
		double *buffer,
		int64_t start_position,
		int sample_rate)
{
	load_configuration();
	if(!fft)
	{
		fft = new SpectrogramFFT(this);
		fft->initialize(WINDOW_SIZE);
	}
	if(!data)
	{
		data = new float[HALF_WINDOW];
	}

	read_samples(buffer, 0, sample_rate, start_position, size);
	double* copy = new double[size];
	int inc = (size/STEP);
	for(int i=0; i<size; i+= inc){
	//	bzero(data, sizeof(float) * HALF_WINDOW);
		total_windows = 0;
		fft->process_buffer(start_position+i,
			size, 
			copy,
			get_direction());
		for(int j = 0; j < HALF_WINDOW; j++)
			data[j] /= total_windows;
		send_render_gui(data, HALF_WINDOW);
	}

	//printf("Size is: %i\n", size);
	//printf("Start is: %i\n", start_position);

	delete [] copy;

	return 0;
}

SET_STRING_MACRO(Spectrogram)

NEW_PICON_MACRO(Spectrogram)

SHOW_GUI_MACRO(Spectrogram, SpectrogramThread)

RAISE_WINDOW_MACRO(Spectrogram)

void Spectrogram::update_gui()
{
	if(thread)
	{
		load_configuration();
		thread->window->lock_window("Spectrogram::update_gui");
		thread->window->update_gui();
		thread->window->unlock_window();
	}
}

void Spectrogram::render_gui(void *data, int size)
{
	if(thread)
	{
		thread->window->lock_window("Spectrogram::render_gui");
		float *frame = (float*)data;
		int niquist = get_project_samplerate();
		BC_SubWindow *canvas = thread->window->canvas;
		int h = canvas->get_h();
		int input1 = HALF_WINDOW - 1;
		double *temp = new double[h];

// Scale frame to canvas height
		for(int i = 0; i < h; i++)
		{
			int input2 = (int)((h - 1 - i) * TOTALFREQS / h);
			input2 = Freq::tofreq(input2) * 
				HALF_WINDOW / 
				niquist;
			input2 = MIN(HALF_WINDOW - 1, input2);
			double sum = 0;
			if(input1 > input2)
			{
				for(int j = input1 - 1; j >= input2; j--)
					sum += frame[j];

				sum /= input1 - input2;
			}
			else
			{
				sum = frame[input2];
			}

			temp[i] = sum;
			input1 = input2;
		}

//Draw the data
		column %= canvas->get_w();
		if(column == 0){
			background++;
			background %= 3;
		}
		int x = column++;
		//double scale = (double)0xffffff;
		double scale = (double) 0xff;
		for(int i = 0; i < h; i++)
		{
			int64_t color;
			int brightness = (int) (scale * temp[i]);
			brightness = brightness > 0 ? brightness : 0;
			brightness = brightness < 0xff ? brightness : 0xff;

			color = brightness | brightness << 8 | brightness << 16;
			color |= 0xff << (background*8) ; 
			color &= ~(0x7f << (background*8)) ; //(0x80 == ~0x7f)

			canvas->set_color(color);
			canvas->draw_pixel(x, i);
		}

		canvas->flash();
		canvas->flush();
		delete [] temp;

		thread->window->unlock_window();
	}
}

void Spectrogram::load_configuration()
{
	KeyFrame *prev_keyframe;
	prev_keyframe = get_prev_keyframe(get_source_position());

 	read_data(prev_keyframe);
}

void Spectrogram::read_data(KeyFrame *keyframe)
{
	FileXML input;
	input.set_shared_string(keyframe->data, strlen(keyframe->data));

	int result = 0;
	while(!result)
	{
		result = input.read_tag();

		if(!result)
		{
			if(input.tag.title_is("SPECTROGRAM"))
			{
				config.level = input.tag.get_property("LEVEL", config.level);
			}
		}
	}
}

void Spectrogram::save_data(KeyFrame *keyframe)
{
	FileXML output;
	output.set_shared_string(keyframe->data, MESSAGESIZE);

	output.tag.set_title("SPECTROGRAM");
	output.tag.set_property("LEVEL", (double)config.level);
	output.append_tag();
	output.tag.set_title("/SPECTROGRAM");
	output.append_tag();
	output.append_newline();
	output.terminate_string();
}

int Spectrogram::load_defaults()
{
	char directory[BCTEXTLEN];

	sprintf(directory, "%sspectrogram.rc", BCASTDIR);
	defaults = new BC_Hash(directory);
	defaults->load();
	config.level = defaults->get("LEVEL", config.level);
	return 0;
}

int Spectrogram::save_defaults()
{
	defaults->update("LEVEL", config.level);
	defaults->save();
	return 0;
}





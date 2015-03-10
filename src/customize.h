#include <pebble.h>

#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H
	
	enum {
		KEY_INVERT = 0,
		KEY_SHOW_DATE = 1
	};

	Window *s_main_window;
	TextLayer *s_hours_layer;
	TextLayer *s_minutes_layer;
	TextLayer *s_date_layer;
	TextLayer *s_batt_layer;

	void perform_customisation(Tuple *t);
	
#endif
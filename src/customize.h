#include <pebble.h>

#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H
	
#define ANIM_DURATION 1500
#define ANIM_DELAY 1000*60*5
	
enum {
	KEY_INVERT = 0,
	KEY_SHOW_DATE_BATT = 1
	//,KEY_USE_DATE = 2,
	//KEY_USE_BATT = 3
};

Window *s_main_window;
TextLayer *s_hours_layer;
TextLayer *s_minutes_layer;
TextLayer *s_date_layer;
TextLayer *s_batt_layer;
GFont s_time_font;
GFont s_date_font;

typedef struct {
	TextLayer *s_anim_curr_layer;
	TextLayer *s_anim_next_layer;
	PropertyAnimation *s_curr_animation;
	PropertyAnimation *s_next_animation;
} Line;

Line line_date_batt;

void perform_customisation(Tuple *t);
	
#endif
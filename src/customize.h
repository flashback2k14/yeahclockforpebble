#include <pebble.h>

#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H
	
#define ANIM_DURATION 1500
#define ANIM_DELAY 1000*60*5
#define ANIM_DELAY_RESTART 100	
	
//enum for customisation
enum {
	KEY_INVERT = 0,
	KEY_SHOW_DATE_WEATHER = 1,
	KEY_TEMPERATURE = 2,
	KEY_CONDITIONS = 3
};

//UI elements
Window *s_main_window;
TextLayer *s_hours_layer;
TextLayer *s_minutes_layer;
GFont s_time_font;
GFont s_date_font;

//UI Element for Animation
typedef struct {
	TextLayer *s_anim_curr_layer;
	TextLayer *s_anim_next_layer;
	PropertyAnimation *s_curr_animation;
	PropertyAnimation *s_next_animation;
} Line;

//global variables
Line line_date_weather;
char temperature_buffer[8];
char conditions_buffer[32];
char weather_layer_buffer[32];
extern bool restartAnimationNext;
extern bool restartAnimationCurr;

//global methods
void updateLineDateToWeather(Line *line);
void perform_customisation(Tuple *t);
	
#endif
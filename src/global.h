#include <pebble.h>

#ifndef GLOBAL_H
#define GLOBAL_H
	
/**
 * Constant variables
 */
#define PLACE_FR_RECT GRect(0, -2,144,70)
#define START_PLACE_MR_RECT GRect(0, 72, 144, 37)	
#define TO_RIGHT_MR_RECT GRect(144, 72, 144, 37)
#define FROM_THE_LEFT_MR_RECT GRect(-144, 72, 144, 37)	
#define PLACE_LR_RECT	GRect(0, 92,144,70)
	
#define ANIM_DURATION 1500
#define ANIM_DELAY 1000
#define ANIM_DELAY_RESTART 100	
	
/**
 * enum for customisation
 */
enum {
	KEY_INVERT = 0,
	KEY_SHOW_DATE_WEATHER = 1,
	KEY_TEMPERATURE_CELSIUS = 2,
	KEY_TEMPERATURE_FAHRENHEIT = 3,
	KEY_CONDITIONS = 4
};

/**
 * Definition Struct Line
 */
typedef struct {
	TextLayer *date_layer;
	TextLayer *weather_layer;
	PropertyAnimation *go_to_right_anim;
	PropertyAnimation *from_the_left_anim;
} Line;

/**
 * UI elements
 */
extern Window *main_window;
extern TextLayer *hours_layer;
extern TextLayer *minutes_layer;
extern GFont time_font;
extern GFont date_font;

/** 
 * global variables
 */
extern Line line_date_weather;
extern char temperature_celsius_buffer[8];
extern char temperature_fahrenheit_buffer[8];
extern char conditions_buffer[32];
extern char weather_layer_buffer[32];

extern bool isDateShown;
extern bool restartAnimation;

#endif	
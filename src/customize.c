#include <pebble.h>
#include "customize.h"


void perform_customisation(Tuple *t) {
	//which key was received?
	switch(t->key) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Key %d recognized!", (int)t->key);
		case KEY_INVERT:
			//it's the KEY_INVERT key
			if (strcmp(t -> value -> cstring, "on") == 0) {
				//set and save as inverted
				text_layer_set_background_color(s_hours_layer, GColorWhite);
				text_layer_set_text_color(s_hours_layer, GColorBlack);
				text_layer_set_background_color(s_minutes_layer, GColorWhite);
				text_layer_set_text_color(s_minutes_layer, GColorBlack);
				text_layer_set_background_color(line_date_weather.s_anim_curr_layer, GColorWhite);
				text_layer_set_text_color(line_date_weather.s_anim_curr_layer, GColorBlack);
				text_layer_set_background_color(line_date_weather.s_anim_next_layer, GColorWhite);
				text_layer_set_text_color(line_date_weather.s_anim_next_layer, GColorBlack);
				window_set_background_color(s_main_window, GColorWhite);
				//save invert key
				persist_write_bool(KEY_INVERT, true);
			} else if (strcmp(t -> value -> cstring, "off") == 0) {
				//set and save as not inverted
				text_layer_set_background_color(s_hours_layer, GColorBlack);
				text_layer_set_text_color(s_hours_layer, GColorWhite);
				text_layer_set_background_color(s_minutes_layer, GColorBlack);
				text_layer_set_text_color(s_minutes_layer, GColorWhite);
				text_layer_set_background_color(line_date_weather.s_anim_curr_layer, GColorBlack);
				text_layer_set_text_color(line_date_weather.s_anim_curr_layer, GColorWhite);
				text_layer_set_background_color(line_date_weather.s_anim_next_layer, GColorBlack);
				text_layer_set_text_color(line_date_weather.s_anim_next_layer, GColorWhite);
				window_set_background_color(s_main_window, GColorBlack);
				//save invert key
				persist_write_bool(KEY_INVERT, false);
			} else {
				APP_LOG(APP_LOG_LEVEL_ERROR, "KEY INVERT - no valid information received!");
			}		
			break;
		
		case KEY_SHOW_DATE_WEATHER:
			//it's the KEY_SHOW_DATE_WEATHER key
			if (strcmp(t -> value -> cstring, "on") == 0) {
				//restart animations
				restartAnimationNext = 0;
				restartAnimationCurr = 0;
				updateLineDateToWeather(&line_date_weather);
				//set and save show date and Weather layer				
				layer_set_hidden((Layer *) line_date_weather.s_anim_curr_layer, false);
				layer_set_hidden((Layer *) line_date_weather.s_anim_next_layer, false);
				//set new position for hours and minutes layer
				layer_set_frame((Layer *) s_hours_layer, GRect(0,-2,144,70));
				layer_set_frame((Layer *) s_minutes_layer, GRect(0,92,144,70));
				//save show date
				persist_write_bool(KEY_SHOW_DATE_WEATHER, true);
			} else if (strcmp(t -> value -> cstring, "off") == 0) {
				//stop any animation in progress
				animation_unschedule_all();
				//set and save hide date and weather layer
				layer_set_hidden((Layer *) line_date_weather.s_anim_curr_layer, true);
				layer_set_hidden((Layer *) line_date_weather.s_anim_next_layer, true);
				//set new position for hours and minutes layer
				layer_set_frame((Layer *) s_hours_layer, GRect(0,10,144,70));
				layer_set_frame((Layer *) s_minutes_layer, GRect(0,80,144,70));
				//save hide date
				persist_write_bool(KEY_SHOW_DATE_WEATHER, false);
			} else {
				APP_LOG(APP_LOG_LEVEL_ERROR, "KEY_SHOW_DATE - no valid information received!");
			}
			break;
		
		case KEY_TEMPERATURE:
			snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
			//APP_LOG(APP_LOG_LEVEL_INFO, "perform_customisation: %d", (int)t->value->int32);
      break;
		
    case KEY_CONDITIONS:
			snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
			//APP_LOG(APP_LOG_LEVEL_INFO, "perform_customisation: %s", t->value->cstring);
      break;
		
		default:
			APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
	}
}
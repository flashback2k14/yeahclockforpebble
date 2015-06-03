#include <pebble.h>
#include "global.h"	
#include "customize.h"
#include "layeranimation.h"
	
	
void perform_customisation(Tuple *t) {
	//which key was received?
	switch(t->key) {
		case KEY_INVERT:
			//it's the KEY_INVERT key
			if (strcmp(t -> value -> cstring, "on") == 0) {
				//set and save as inverted
				text_layer_set_background_color(hours_layer, GColorWhite);
				text_layer_set_text_color(hours_layer, GColorBlack);
				text_layer_set_background_color(minutes_layer, GColorWhite);
				text_layer_set_text_color(minutes_layer, GColorBlack);
				text_layer_set_background_color(line_date_weather.date_layer, GColorWhite);
				text_layer_set_text_color(line_date_weather.date_layer, GColorBlack);
				text_layer_set_background_color(line_date_weather.weather_layer, GColorWhite);
				text_layer_set_text_color(line_date_weather.weather_layer, GColorBlack);
				window_set_background_color(main_window, GColorWhite);
				//save invert key
				persist_write_bool(KEY_INVERT, true);
			} else if (strcmp(t -> value -> cstring, "off") == 0) {
				//set and save as not inverted
				text_layer_set_background_color(hours_layer, GColorBlack);
				text_layer_set_text_color(hours_layer, GColorWhite);
				text_layer_set_background_color(minutes_layer, GColorBlack);
				text_layer_set_text_color(minutes_layer, GColorWhite);
				text_layer_set_background_color(line_date_weather.date_layer, GColorBlack);
				text_layer_set_text_color(line_date_weather.date_layer, GColorWhite);
				text_layer_set_background_color(line_date_weather.weather_layer, GColorBlack);
				text_layer_set_text_color(line_date_weather.weather_layer, GColorWhite);
				window_set_background_color(main_window, GColorBlack);
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
				restartAnimation = 0;
				start_animation_process(&line_date_weather);
				//set and save show date and Weather layer				
				layer_set_hidden((Layer *) line_date_weather.date_layer, false);
				layer_set_hidden((Layer *) line_date_weather.weather_layer, false);
				//set new position for hours and minutes layer
				layer_set_frame((Layer *) hours_layer, GRect(0,-2,144,70));
				layer_set_frame((Layer *) minutes_layer, GRect(0,92,144,70));
				//save show date
				persist_write_bool(KEY_SHOW_DATE_WEATHER, true);
			} else if (strcmp(t -> value -> cstring, "off") == 0) {
				//stop any animation in progress
				#ifdef PBL_PLATFORM_APLITE
					property_animation_destroy(line_date_weather.go_to_right_anim);
					property_animation_destroy(line_date_weather.from_the_left_anim);
				#endif
				restartAnimation = 1;
				//set and save hide date and weather layer
				layer_set_hidden((Layer *) line_date_weather.date_layer, true);
				layer_set_hidden((Layer *) line_date_weather.weather_layer, true);
				//set new position for hours and minutes layer
				layer_set_frame((Layer *) hours_layer, GRect(0,10,144,70));
				layer_set_frame((Layer *) minutes_layer, GRect(0,80,144,70));
				//save hide date
				persist_write_bool(KEY_SHOW_DATE_WEATHER, false);
			} else {
				APP_LOG(APP_LOG_LEVEL_ERROR, "KEY_SHOW_DATE - no valid information received!");
			}
			break;
		
		case KEY_TEMPERATURE_CELSIUS:
			snprintf(temperature_celsius_buffer, sizeof(temperature_celsius_buffer), "%dC", (int)t->value->int32);
			APP_LOG(APP_LOG_LEVEL_INFO, "perform_customisation: %d", (int)t->value->int32);
      break;
		
		case KEY_TEMPERATURE_FAHRENHEIT:
			snprintf(temperature_fahrenheit_buffer, sizeof(temperature_fahrenheit_buffer), "%dF", (int)t->value->int32);
			APP_LOG(APP_LOG_LEVEL_INFO, "perform_customisation: %d", (int)t->value->int32);
      break;
		
    case KEY_CONDITIONS:
			snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
			APP_LOG(APP_LOG_LEVEL_INFO, "perform_customisation: %s", t->value->cstring);
      break;
		
		case KEY_SWITCH_TEMPERATURE:
			if (strcmp(t -> value -> cstring, "celsius") == 0) {
				switchTemperatureToFahrenheit = 0;
				persist_write_bool(KEY_SWITCH_TEMPERATURE, false);
			} else {
				switchTemperatureToFahrenheit = 1;
				persist_write_bool(KEY_SWITCH_TEMPERATURE, true);
			}
			break;
		
		default:
			APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
	}
}
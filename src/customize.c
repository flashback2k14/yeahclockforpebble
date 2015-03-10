#include <pebble.h>
#include "customize.h"

void perform_customisation(Tuple *t) {
	// which key was received?
	switch(t->key) {
		case KEY_INVERT:
		//it's the KEY_INVERT key
		if (strcmp(t -> value -> cstring, "on") == 0) {
			//set and save as inverted
			text_layer_set_background_color(s_hours_layer, GColorWhite);
			text_layer_set_text_color(s_hours_layer, GColorBlack);
			text_layer_set_background_color(s_date_layer, GColorWhite);
			text_layer_set_text_color(s_date_layer, GColorBlack);
			text_layer_set_background_color(s_batt_layer, GColorWhite);
			text_layer_set_text_color(s_batt_layer, GColorBlack);
			text_layer_set_background_color(s_minutes_layer, GColorWhite);
			text_layer_set_text_color(s_minutes_layer, GColorBlack);
			window_set_background_color(s_main_window, GColorWhite);
			//save invert key
			persist_write_bool(KEY_INVERT, true);
		} else if (strcmp(t -> value -> cstring, "off") == 0) {
			//set and save as not inverted
			text_layer_set_background_color(s_hours_layer, GColorBlack);
			text_layer_set_text_color(s_hours_layer, GColorWhite);
			text_layer_set_background_color(s_date_layer, GColorBlack);
			text_layer_set_text_color(s_date_layer, GColorWhite);
			text_layer_set_background_color(s_batt_layer, GColorBlack);
			text_layer_set_text_color(s_batt_layer, GColorWhite);
			text_layer_set_background_color(s_minutes_layer, GColorBlack);
			text_layer_set_text_color(s_minutes_layer, GColorWhite);
			window_set_background_color(s_main_window, GColorBlack);
			//save invert key
			persist_write_bool(KEY_INVERT, false);
		} else {
			APP_LOG(APP_LOG_LEVEL_ERROR, "KEY INVERT - no valid information received!");
		}		
		break;
		case KEY_SHOW_DATE:
		//it's the KEY_SHOW_DATE key
		if (strcmp(t -> value -> cstring, "on") == 0) {
			//set and save show date
			layer_set_hidden((Layer *) s_date_layer, false);
			layer_set_hidden((Layer *) s_batt_layer, true);
			layer_set_frame((Layer *) s_hours_layer, GRect(0,-2,144,70));
			layer_set_frame((Layer *) s_minutes_layer, GRect(0,92,144,70));
			//save show date
			persist_write_bool(KEY_SHOW_DATE, true);
		} else if (strcmp(t -> value -> cstring, "off") == 0) {
			//set and save hide date
			/*layer_set_hidden((Layer *) s_date_layer, true);
				layer_set_frame((Layer *) s_hours_layer, GRect(0,10,144,70));
				layer_set_frame((Layer *) s_minutes_layer, GRect(0,80,144,70));*/
			layer_set_hidden((Layer *) s_date_layer, true);
			layer_set_hidden((Layer *) s_batt_layer, false);
			layer_set_frame((Layer *) s_hours_layer, GRect(0,-2,144,70));
			layer_set_frame((Layer *) s_minutes_layer, GRect(0,92,144,70));
			//save hide date
			persist_write_bool(KEY_SHOW_DATE, false);
		} else {
			APP_LOG(APP_LOG_LEVEL_ERROR, "KEY_SHOW_DATE - no valid information received!");
		}	
		break;
		default:
		APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
	}
}
#include <pebble.h>
/**
 * define app keys
 */
enum {
  KEY_INVERT = 0,
  KEY_SHOW_DATE = 1
};
/**
 * Globals
 */
static Window *s_main_window;
static TextLayer *s_hours_layer;
static TextLayer *s_minutes_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont s_date_font;
/**
 * customise watchface
 */
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	// read first item
  Tuple *t = dict_read_first(iterator);
  // for all items
  while(t != NULL) {
    // which key was received?
    switch(t->key) {
    case KEY_INVERT:
			//it's the KEY_INVERT key
			if(strcmp(t -> value -> cstring, "on") == 0) {
				//set and save as inverted
				text_layer_set_background_color(s_hours_layer, GColorWhite);
				text_layer_set_text_color(s_hours_layer, GColorBlack);
				text_layer_set_background_color(s_date_layer, GColorWhite);
				text_layer_set_text_color(s_date_layer, GColorBlack);
				text_layer_set_background_color(s_minutes_layer, GColorWhite);
				text_layer_set_text_color(s_minutes_layer, GColorBlack);
				window_set_background_color(s_main_window, GColorWhite);
				//save invert key
				persist_write_bool(KEY_INVERT, true);
			} else if(strcmp(t -> value -> cstring, "off") == 0) {
				//set and save as not inverted
				text_layer_set_background_color(s_hours_layer, GColorBlack);
				text_layer_set_text_color(s_hours_layer, GColorWhite);
				text_layer_set_background_color(s_date_layer, GColorBlack);
				text_layer_set_text_color(s_date_layer, GColorWhite);
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
			if(strcmp(t -> value -> cstring, "on") == 0) {
				bool inverted = persist_read_bool(KEY_INVERT);
				//set and save show date
				if (inverted == true) {
					text_layer_set_background_color(s_date_layer, GColorWhite);
					text_layer_set_text_color(s_date_layer, GColorBlack);
				} else {
					text_layer_set_background_color(s_date_layer, GColorBlack);
					text_layer_set_text_color(s_date_layer, GColorWhite);
				}
				//save show date
				persist_write_bool(KEY_SHOW_DATE, true);
			} else if(strcmp(t -> value -> cstring, "off") == 0) {
				bool inverted = persist_read_bool(KEY_INVERT);
				//set and save hide date
				if (inverted == true) {
					text_layer_set_background_color(s_date_layer, GColorWhite);
					text_layer_set_text_color(s_date_layer, GColorWhite);
				} else {
					text_layer_set_background_color(s_date_layer, GColorBlack);
					text_layer_set_text_color(s_date_layer, GColorBlack);
				}
				//save hide date
				persist_write_bool(KEY_SHOW_DATE, false);
			} else {
				APP_LOG(APP_LOG_LEVEL_ERROR, "KEY_SHOW_DATE - no valid information received!");
			}	
			break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
    }
    //look for next item
    t = dict_read_next(iterator);
  }
}
/**
 * update time function
 */
static void update_time() {
	// Get a tm structure
	time_t temp = time(NULL); 
	struct tm *tick_time = localtime(&temp);
	// Create a long-lived buffer
	static char buffer_h[] = "00";
	static char buffer_min[] = "00";
	static char buffer_date[] = "Mon, 01.01.15";
	// Write the current hours and minutes into the buffer
	if(clock_is_24h_style() == true) {
		// Use 24 hour format
		strftime(buffer_h, sizeof("00"), "%H", tick_time);
	} else {
		// Use 12 hour format
		strftime(buffer_h, sizeof("00"), "%I", tick_time);  
	}
	strftime(buffer_min, sizeof("00"), "%M", tick_time);
	strftime(buffer_date, sizeof("Mon, 01.01.15"), "%a, %d.%m.%y", tick_time);
	// Display this time on the TextLayer
	text_layer_set_text(s_hours_layer, buffer_h);	
	text_layer_set_text(s_minutes_layer, buffer_min);
	text_layer_set_text(s_date_layer, buffer_date);
}
/**
 * handle  time function hour
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}
/**
 * init Watchface layout
 */
static void main_window_load(Window *window) {
	bool inverted = persist_read_bool(KEY_INVERT);
	//create GFont
	s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_SLAB_64));
	s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_SLAB_20));
	//create textlayers
	s_hours_layer = text_layer_create(GRect(0, -2,144,70));
	s_date_layer = text_layer_create(GRect(0, 72,144,40));	
	s_minutes_layer = text_layer_create(GRect(0, 92,144,70));
	//set color
	if (inverted == true) {
		text_layer_set_background_color(s_hours_layer, GColorWhite);
		text_layer_set_text_color(s_hours_layer, GColorBlack);
		text_layer_set_background_color(s_date_layer, GColorWhite);
		text_layer_set_text_color(s_date_layer, GColorBlack);
		text_layer_set_background_color(s_minutes_layer, GColorWhite);
		text_layer_set_text_color(s_minutes_layer, GColorBlack);
		window_set_background_color(s_main_window, GColorWhite);
	} else {
		text_layer_set_background_color(s_hours_layer, GColorBlack);
		text_layer_set_text_color(s_hours_layer, GColorWhite);
		text_layer_set_background_color(s_date_layer, GColorBlack);
		text_layer_set_text_color(s_date_layer, GColorWhite);
		text_layer_set_background_color(s_minutes_layer, GColorBlack);
		text_layer_set_text_color(s_minutes_layer, GColorWhite);
		window_set_background_color(s_main_window, GColorBlack);
	}
	//set font
	text_layer_set_font(s_hours_layer, s_time_font);
	text_layer_set_font(s_date_layer, s_date_font);
	text_layer_set_font(s_minutes_layer, s_time_font);
	//set alignment
	text_layer_set_text_alignment(s_hours_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(s_minutes_layer, GTextAlignmentCenter);
	//add as child to window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hours_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minutes_layer));
	//Make sure the time is displayed from the start
	update_time();
}
/**
 * deinit Watchface layout
 */
static void main_window_unload(Window *window) {
	//destroy textlayer
	text_layer_destroy(s_hours_layer);
	text_layer_destroy(s_minutes_layer);
	text_layer_destroy(s_date_layer);
	//unload GFont
	fonts_unload_custom_font(s_time_font);
	fonts_unload_custom_font(s_date_font);
}
/**
 * init watchface
 */
static void init() {
	//create main window element and assign to pointer
	s_main_window = window_create();
	//set BG Color
	window_set_background_color(s_main_window, GColorBlack);
	//hide system status
	window_set_fullscreen(s_main_window, true);
	//set handler to manage elements inside the window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	//show window on pebble with animation
	window_stack_push(s_main_window, true);
	//register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	//register receiver for customisation
	app_message_register_inbox_received(inbox_received_callback);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}
/**
 * deinit watchface
 */
static void deinit() {
	//destroy window
	window_destroy(s_main_window);
}
/**
 * entry point
 */
int main(void) {
	init();
	app_event_loop();
	deinit();
}
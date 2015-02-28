#include <pebble.h>
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
 * update hour time function
 */
static void update_time_hour() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  // Create a long-lived buffer
  static char buffer_h[] = "00";
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
		// Use 24 hour format
		strftime(buffer_h, sizeof("00"), "%H", tick_time);
    } else {
    	// Use 12 hour format
		strftime(buffer_h, sizeof("00"), "%I", tick_time);  
    }
  	// Display this time on the TextLayer
  	text_layer_set_text(s_hours_layer, buffer_h);	
}
/**
 * update minutes time function
 */
static void update_time_minutes() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  // Create a long-lived buffer
  static char buffer_min[] = "00";
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
		// Use 24 hour format
		strftime(buffer_min, sizeof("00"), "%M", tick_time);
    } else {
    	// Use 12 hour format
		strftime(buffer_min, sizeof("00"), "%M", tick_time);
    }
  	// Display this time on the TextLayer
  	text_layer_set_text(s_minutes_layer, buffer_min);
}
/**
 * update date time function
 */
static void update_time_date() {
	  // Get a tm structure
	  time_t temp = time(NULL); 
	  struct tm *tick_time = localtime(&temp);
	  // Create a long-lived buffer
	  static char buffer_min[] = "Mon, 01.01.15";
	  // Write the current hours and minutes into the buffer
	  strftime(buffer_min, sizeof("Mon, 01.01.15"), "%a, %d.%m.%y", tick_time);
	  // Display this time on the TextLayer
  	  text_layer_set_text(s_date_layer, buffer_min);
}
/**
 * handle  time function hour
 */
static void tick_handler_hour(struct tm *tick_time, TimeUnits units_changed) {
	update_time_hour();
}
/**
 * handle  time function minutes
 */
static void tick_handler_min(struct tm *tick_time, TimeUnits units_changed) {
	update_time_minutes();
}
/**
 * handle  time function date
 */
static void tick_handler_date(struct tm *tick_time, TimeUnits units_changed) {
	update_time_date();
}
/**
 * init Watchface layout
 */
static void main_window_load(Window *window) {
	//create GFont
	s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_SLAB_64));
	s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_SLAB_20));
	//create time textlayer
	s_hours_layer = text_layer_create(GRect(0, -5,144,70));
	s_date_layer = text_layer_create(GRect(0, 72,144,40));
	s_minutes_layer = text_layer_create(GRect(0, 95,144,70));
	//set color
	#ifdef PBL_COLOR
		text_layer_set_background_color(s_hours_layer, GColorFolly);
		text_layer_set_text_color(s_hours_layer, GColorBlack);
	#else
		text_layer_set_background_color(s_hours_layer, GColorBlack);
		text_layer_set_text_color(s_hours_layer, GColorWhite);
	#endif
		
	#ifdef PBL_COLOR
		text_layer_set_background_color(s_date_layer, GColorBrightGreen);
		text_layer_set_text_color(s_date_layer, GColorBlack);
	#else
		text_layer_set_background_color(s_date_layer, GColorBlack);
		text_layer_set_text_color(s_date_layer, GColorWhite);
	#endif
		
	#ifdef PBL_COLOR
		text_layer_set_background_color(s_minutes_layer, GColorOrange);
		text_layer_set_text_color(s_minutes_layer, GColorWhite);
	#else
		text_layer_set_background_color(s_minutes_layer, GColorBlack);
		text_layer_set_text_color(s_minutes_layer, GColorWhite);
	#endif
	//improve layout
	text_layer_set_font(s_hours_layer, s_time_font);
	text_layer_set_text_alignment(s_hours_layer, GTextAlignmentCenter);
	
	text_layer_set_font(s_date_layer, s_date_font);
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
	
	text_layer_set_font(s_minutes_layer,s_time_font);
	text_layer_set_text_alignment(s_minutes_layer, GTextAlignmentCenter);
	//add as child to window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hours_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minutes_layer));
	//register with TickTimerService
	tick_timer_service_subscribe(HOUR_UNIT, tick_handler_hour);
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler_min);
	tick_timer_service_subscribe(DAY_UNIT, tick_handler_date);
}
/**
 * deinit Watchface layout
 */
static void main_window_unload(Window *window) {
	//destroy textlayer
	text_layer_destroy(s_hours_layer);
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_minutes_layer);
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
	#ifdef PBL_COLOR
	  window_set_background_color(s_main_window, GColorDukeBlue);
	#else
	  window_set_background_color(s_main_window, GColorBlack);
	#endif
	//hide system status
	window_set_fullscreen(s_main_window, true);
	//set handler to manage elements inside the window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	//show window on pebble with animation
	window_stack_push(s_main_window, true);
	//Make sure the time is displayed from the start
	update_time_hour();
	update_time_minutes();
	update_time_date();
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
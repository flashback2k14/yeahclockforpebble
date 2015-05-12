#include <pebble.h>
#include "global.h"	
#include "customize.h"
#include "layeranimation.h"

/**
 * declaration UI Elements
 */
Window *main_window;
TextLayer *hours_layer;
TextLayer *minutes_layer;
GFont time_font;
GFont date_font;
Line line_date_weather;

/**
 * declaration of global variables
 */
bool isDateShown = 1;	
bool restartAnimation = 0;

/**
 * Initialisation TextLayer
 */
static TextLayer* init_text_layer(GRect location, GColor textColor, GColor backgroundColor, GFont textFont, GTextAlignment alignment) {
  TextLayer *layer = text_layer_create(location);
  text_layer_set_text_color(layer, textColor);
  text_layer_set_background_color(layer, backgroundColor);
  text_layer_set_font(layer, textFont);
  text_layer_set_text_alignment(layer, alignment);
  return layer;
}

/**
 * customise watchface
 */
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	//read first item
  Tuple *t = dict_read_first(iterator);
  // for all items
  while(t != NULL) {
    //customise the watchface
    perform_customisation(t);
    //look for next item
    t = dict_read_next(iterator);
  }
	//assemble weather information and display
	APP_LOG(APP_LOG_LEVEL_INFO, "conditions_buffer: %s", conditions_buffer);
	if (strcmp(conditions_buffer, "") == 0) {
		//set placeholder text for weather informations
		text_layer_set_text(line_date_weather.weather_layer, "hack o'clock...");
	} else {
		snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
		text_layer_set_text(line_date_weather.weather_layer, weather_layer_buffer);
	}
}

/**
 * eventhandler for weather
 */
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

/**
 * call AppMessage Outbox send
 */
static void callAppMessageOutboxSend() {
	//begin dictionary
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	//add a key-value pair
	Tuplet value = TupletInteger(0, 1);
	dict_write_tuplet(iter, &value);
	//send the message!
	app_message_outbox_send();
}

/**
 * update time function
 */
static void update_time() {
	//get a tm structure
	time_t temp = time(NULL); 
	struct tm *tick_time = localtime(&temp);
	//create a long-lived buffer
	static char buffer_h[] = "00";
	static char buffer_min[] = "00";
	static char buffer_date[] = "Mon, 01.01.15";
	//write the current hours and minutes into the buffer
	if(clock_is_24h_style() == true) {
		// Use 24 hour format
		strftime(buffer_h, sizeof("00"), "%H", tick_time);
	} else {
		//use 12 hour format
		strftime(buffer_h, sizeof("00"), "%I", tick_time);  
	}
	strftime(buffer_min, sizeof("00"), "%M", tick_time);
	strftime(buffer_date, sizeof("Mon, 01.01.15"), "%a, %d.%m.%y", tick_time);
	//display this time on the TextLayer
	text_layer_set_text(hours_layer, buffer_h);	
	text_layer_set_text(minutes_layer, buffer_min);
	text_layer_set_text(line_date_weather.date_layer, buffer_date);
	//get weather update every 5 minutes
	//start animation process every 2 minutes
	int min = tick_time->tm_min;
	if (((min % 2) == 0) == (restartAnimation == 0))  {
		callAppMessageOutboxSend();
		start_animation_process(&line_date_weather);
	}
}

/**
 * handle time function
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

/**
 * init Watchface layout
 */
static void main_window_load(Window *window) {
	//create GFont
	time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_SLAB_64));
	date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_SLAB_20));
	
	//init TextLayers
	#ifdef PBL_COLOR
		hours_layer = init_text_layer(PLACE_FR_RECT, GColorWhite, GColorClear, time_font, GTextAlignmentCenter);
		minutes_layer = init_text_layer(PLACE_LR_RECT, GColorWhite, GColorClear, time_font, GTextAlignmentCenter);
		line_date_weather.date_layer = init_text_layer(START_PLACE_MR_RECT, GColorWhite, GColorClear, date_font, GTextAlignmentCenter);
		line_date_weather.weather_layer = init_text_layer(FROM_THE_LEFT_MR_RECT, GColorWhite, GColorClear, date_font, GTextAlignmentCenter);
	#else
		hours_layer = init_text_layer(PLACE_FR_RECT, GColorWhite, GColorClear, time_font, GTextAlignmentCenter);
		minutes_layer = init_text_layer(PLACE_LR_RECT, GColorWhite, GColorClear, time_font, GTextAlignmentCenter);
		line_date_weather.date_layer = init_text_layer(START_PLACE_MR_RECT, GColorWhite, GColorClear, FONT_KEY_BITHAM_42_BOLD, GTextAlignmentCenter);
		line_date_weather.weather_layer = init_text_layer(FROM_THE_LEFT_MR_RECT, GColorWhite, GColorClear, FONT_KEY_BITHAM_42_BOLD, GTextAlignmentCenter);
	#endif	
	
	//add as child to window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(hours_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(minutes_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(line_date_weather.date_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(line_date_weather.weather_layer));
	
	//start animation loop
	#ifdef PBL_PLATFORM_APLITE
		init_animations_aplite(&line_date_weather);
	#endif
	//make sure the time is displayed from the start
	update_time();
}

/**
 * deinit Watchface layout
 */
static void main_window_unload(Window *window) {
	//destroy textlayer
	text_layer_destroy(hours_layer);
	text_layer_destroy(minutes_layer);
	text_layer_destroy(line_date_weather.date_layer);
	text_layer_destroy(line_date_weather.weather_layer);
	//destroy animations
	#ifdef PBL_PLATFORM_APLITE
		property_animation_destroy(line_date_weather.go_to_right_anim);
 		property_animation_destroy(line_date_weather.from_the_left_anim);
	#endif
	//unload GFont
	fonts_unload_custom_font(time_font);
	fonts_unload_custom_font(date_font);
}

/**
 * init watchface
 */
static void init() {
	//create main window element and assign to pointer
	main_window = window_create();
	//set BG Color
	window_set_background_color(main_window, GColorBlack);
	//hide system status
	#ifdef PBL_SDK_2
		window_set_fullscreen(main_window, true);
	#endif
	//set handler to manage elements inside the window
	window_set_window_handlers(main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	//register receiver for customisation
	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	//register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	//show window on pebble with animation
	window_stack_push(main_window, true);
}

/**
 * deinit watchface
 */
static void deinit() {
	//app_message_deregister_callbacks();
  //animation_unschedule_all();
	//tick_timer_service_unsubscribe();
	window_destroy(main_window);
}

/**
 * entry point
 */
int main(void) {
	init();
	app_event_loop();
	deinit();
}
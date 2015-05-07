#include <pebble.h>
#include "customize.h"	

/**
 * declaration of global variables
 * controls restarting process of animations
 */
bool restartAnimationNext = 1;
bool restartAnimationCurr = 1;
/**
 * animation
 */
/**
 * animation handler
 */
static void animationStoppedHandler(struct Animation *animation, bool finished, void *context) {
#ifdef PBL_PLATFORM_APLITE
		//free animation
		property_animation_destroy((PropertyAnimation *) animation);
#endif
	//
	Layer *textLayer = text_layer_get_layer((TextLayer *)context);
	GRect rect = layer_get_frame(textLayer);
	rect.origin.x = 144;
	layer_set_frame(textLayer, rect);
	//
	if (finished) {
		updateLineDateToWeather(&line_date_weather);
	}
}
/**
 * animate line
 */
static void makeAnimationsForLayers(Line *line, TextLayer *current, TextLayer *next) {
	GRect fromRect = layer_get_frame(text_layer_get_layer(next));
	GRect toRect = fromRect;
	toRect.origin.x -= 144;
	
	line->s_next_animation = property_animation_create_layer_frame(text_layer_get_layer(next), &fromRect, &toRect);
	animation_set_duration((Animation *)line->s_next_animation, ANIM_DURATION);
	if (restartAnimationNext == 0) {
		animation_set_delay((Animation *)line->s_next_animation, ANIM_DELAY_RESTART);
		restartAnimationNext = 1;
	} else {
		animation_set_delay((Animation *)line->s_next_animation, ANIM_DELAY);
	}
	animation_set_curve((Animation *)line->s_next_animation, AnimationCurveEaseOut);
	animation_schedule((Animation *)line->s_next_animation);
	
	GRect fromRect2 = layer_get_frame(text_layer_get_layer(current));
	GRect toRect2 = fromRect2;
	toRect2.origin.x -= 144;
	
	line->s_curr_animation = property_animation_create_layer_frame(text_layer_get_layer(current), &fromRect2, &toRect2);
	animation_set_duration((Animation *)line->s_curr_animation, ANIM_DURATION);
	if (restartAnimationCurr == 0) {
		animation_set_delay((Animation *)line->s_curr_animation, ANIM_DELAY_RESTART);
		restartAnimationCurr = 1;
	} else {
		animation_set_delay((Animation *)line->s_curr_animation, ANIM_DELAY);
	}
	animation_set_curve((Animation *)line->s_curr_animation, AnimationCurveEaseOut);
	
	animation_set_handlers((Animation *)line->s_curr_animation, (AnimationHandlers) {
		.stopped = (AnimationStoppedHandler)animationStoppedHandler
	}, current);
	
	animation_schedule((Animation *)line->s_curr_animation);
}
/**
 * update line
 */
void updateLineDateToWeather(Line *line) {
	TextLayer *next, *current;
	
	GRect rect = layer_get_frame(text_layer_get_layer(line->s_anim_curr_layer));
	current = (rect.origin.x == 0) ? line->s_anim_curr_layer : line->s_anim_next_layer;
	next = (current == line->s_anim_curr_layer) ? line->s_anim_next_layer : line->s_anim_curr_layer;

	makeAnimationsForLayers(line, current, next);
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
	snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
	text_layer_set_text(line_date_weather.s_anim_next_layer, weather_layer_buffer);
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
	text_layer_set_text(s_hours_layer, buffer_h);	
	text_layer_set_text(s_minutes_layer, buffer_min);
	text_layer_set_text(line_date_weather.s_anim_curr_layer, buffer_date);
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
 * handle time function
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
	/**
 	 * get weather update every 5 minutes
   */
	if(tick_time->tm_min % 5 == 0) {
		callAppMessageOutboxSend();
	}
}
/**
 * init Watchface layout
 */
static void main_window_load(Window *window) {
	//create GFont
	s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_SLAB_64));
	s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_SLAB_20));
	//create textlayers
	s_hours_layer = text_layer_create(GRect(0, -2,144,70));
	s_minutes_layer = text_layer_create(GRect(0, 92,144,70));
	line_date_weather.s_anim_curr_layer = text_layer_create(GRect(0, 72,144,37));
	line_date_weather.s_anim_next_layer = text_layer_create(GRect(144, 72,144,37));
	//set background color
	text_layer_set_background_color(s_hours_layer, GColorBlack);
	text_layer_set_background_color(s_minutes_layer, GColorBlack);
	text_layer_set_background_color(line_date_weather.s_anim_curr_layer, GColorBlack);
	text_layer_set_background_color(line_date_weather.s_anim_next_layer, GColorBlack);
	window_set_background_color(s_main_window, GColorBlack);
	//set text color
	text_layer_set_text_color(s_hours_layer, GColorWhite);
	text_layer_set_text_color(s_minutes_layer, GColorWhite);
	text_layer_set_text_color(line_date_weather.s_anim_curr_layer, GColorWhite);	
	text_layer_set_text_color(line_date_weather.s_anim_next_layer, GColorWhite);
	//set font
	text_layer_set_font(s_hours_layer, s_time_font);
	text_layer_set_font(s_minutes_layer, s_time_font);
	text_layer_set_font(line_date_weather.s_anim_curr_layer, s_date_font);
	text_layer_set_font(line_date_weather.s_anim_next_layer, s_date_font);
	//set alignment
	text_layer_set_text_alignment(s_hours_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(s_minutes_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(line_date_weather.s_anim_curr_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(line_date_weather.s_anim_next_layer, GTextAlignmentCenter);
	//add as child to window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hours_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minutes_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(line_date_weather.s_anim_curr_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(line_date_weather.s_anim_next_layer));
	//make sure the time is displayed from the start
	update_time();
	//start animation loop
	updateLineDateToWeather(&line_date_weather);
}
/**
 * deinit Watchface layout
 */
static void main_window_unload(Window *window) {
	//destroy textlayer
	text_layer_destroy(s_hours_layer);
	text_layer_destroy(s_minutes_layer);
	text_layer_destroy(line_date_weather.s_anim_curr_layer);
	text_layer_destroy(line_date_weather.s_anim_next_layer);
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
#ifdef PBL_SDK_2
	window_set_fullscreen(s_main_window, true);
#endif
	//set handler to manage elements inside the window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
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
	window_stack_push(s_main_window, true);
}
/**
 * deinit watchface
 */
static void deinit() {
	//stop any animation in progress
  animation_unschedule_all();
	//unsubscribe timer service
	tick_timer_service_unsubscribe();
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
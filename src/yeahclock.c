#include <pebble.h>
#include "customize.h"	

/**
 * animation
 */
/**
 * function prototype 
 */
static void updateLineDateToBatt(Line *line);
/**
 * animation handler
 */
static void animationStoppedHandler(struct Animation *animation, bool finished, void *context) {
	Layer *textLayer = text_layer_get_layer((TextLayer *)context);
	GRect rect = layer_get_frame(textLayer);
	rect.origin.x = 144;
	layer_set_frame(textLayer, rect);
	
	if (finished) {
		updateLineDateToBatt(&line_date_batt);
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
	animation_set_delay((Animation *)line->s_next_animation, ANIM_DELAY);
	animation_set_curve((Animation *)line->s_next_animation, AnimationCurveEaseOut);
	animation_schedule((Animation *)line->s_next_animation);
	
	GRect fromRect2 = layer_get_frame(text_layer_get_layer(current));
	GRect toRect2 = fromRect2;
	toRect2.origin.x -= 144;
	
	line->s_curr_animation = property_animation_create_layer_frame(text_layer_get_layer(current), &fromRect2, &toRect2);
	animation_set_duration((Animation *)line->s_curr_animation, ANIM_DURATION);
	animation_set_delay((Animation *)line->s_curr_animation, ANIM_DELAY);
	animation_set_curve((Animation *)line->s_curr_animation, AnimationCurveEaseOut);
	
	animation_set_handlers((Animation *)line->s_curr_animation, (AnimationHandlers) {
		.stopped = (AnimationStoppedHandler)animationStoppedHandler
	}, current);
	
	animation_schedule((Animation *)line->s_curr_animation);
}
/**
 * update line
 */
static void updateLineDateToBatt(Line *line) {
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
	// read first item
  Tuple *t = dict_read_first(iterator);
  // for all items
  while(t != NULL) {
    // customise the watchface
    perform_customisation(t);
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
	text_layer_set_text(line_date_batt.s_anim_curr_layer, buffer_date);
}
/**
 * handle time function
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}
/**
 * update battery percent
 */
static void update_battery() {
	//get battery info, copy to long-lived buffer and display
	BatteryChargeState state = battery_state_service_peek();
	static char buffer[] = "Batt: 100";
	snprintf(buffer, sizeof("Batt: 100"), "Batt: %d", state.charge_percent);
	text_layer_set_text(line_date_batt.s_anim_next_layer, buffer);
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
	line_date_batt.s_anim_curr_layer = text_layer_create(GRect(0, 72,144,37));
	line_date_batt.s_anim_next_layer = text_layer_create(GRect(144, 72,144,37));
	//set background color
	text_layer_set_background_color(s_hours_layer, GColorBlack);
	text_layer_set_background_color(s_minutes_layer, GColorBlack);
	text_layer_set_background_color(line_date_batt.s_anim_curr_layer, GColorBlack);
	text_layer_set_background_color(line_date_batt.s_anim_next_layer, GColorBlack);
	window_set_background_color(s_main_window, GColorBlack);
	//set text color
	text_layer_set_text_color(s_hours_layer, GColorWhite);
	text_layer_set_text_color(s_minutes_layer, GColorWhite);
	text_layer_set_text_color(line_date_batt.s_anim_curr_layer, GColorWhite);	
	text_layer_set_text_color(line_date_batt.s_anim_next_layer, GColorWhite);
	//set font
	text_layer_set_font(s_hours_layer, s_time_font);
	text_layer_set_font(s_minutes_layer, s_time_font);
	text_layer_set_font(line_date_batt.s_anim_curr_layer, s_date_font);
	text_layer_set_font(line_date_batt.s_anim_next_layer, s_date_font);
	//set alignment
	text_layer_set_text_alignment(s_hours_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(s_minutes_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(line_date_batt.s_anim_curr_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(line_date_batt.s_anim_next_layer, GTextAlignmentCenter);
	//add as child to window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hours_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minutes_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(line_date_batt.s_anim_curr_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(line_date_batt.s_anim_next_layer));
	//make sure the time is displayed from the start
	update_time();
	//get battery state
	update_battery();
	//start animation loop
	updateLineDateToBatt(&line_date_batt);
}
/**
 * deinit Watchface layout
 */
static void main_window_unload(Window *window) {
	//destroy textlayer
	text_layer_destroy(s_hours_layer);
	text_layer_destroy(s_minutes_layer);
	text_layer_destroy(line_date_batt.s_anim_curr_layer);
	text_layer_destroy(line_date_batt.s_anim_next_layer);
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
#include <pebble.h>
#include "global.h"	
#include "customize.h"
#include "layeranimation.h"

/**
 * Initialisation for Aplite
 */
void animationStoppedHandler(Animation *anim, bool finished, void *context){
	APP_LOG(APP_LOG_LEVEL_INFO, "inside animationStoppedHandler");
	property_animation_destroy((PropertyAnimation *) anim);
}	
	
void init_animations_aplite(Line *line) {
	APP_LOG(APP_LOG_LEVEL_INFO, "inside init_animations_aplite");
	Layer *layer_date = text_layer_get_layer(line->date_layer);
	Layer *layer_weather = text_layer_get_layer(line->weather_layer);
	
	if (isDateShown == 0) {
		APP_LOG(APP_LOG_LEVEL_INFO, "inside IF isDateShown");
		line->go_to_right_anim = property_animation_create_layer_frame(layer_date, &START_PLACE_MR_RECT, &TO_RIGHT_MR_RECT);
		animation_set_duration((Animation*) line->go_to_right_anim, ANIM_DURATION);
		animation_set_curve((Animation*) line->go_to_right_anim, AnimationCurveEaseOut);
		AnimationHandlers handlerDate = {
			.stopped = (AnimationStoppedHandler) animationStoppedHandler
		};
		animation_set_handlers((Animation *) line->go_to_right_anim, handlerDate, NULL);

		line->from_the_left_anim = property_animation_create_layer_frame(layer_weather, &FROM_THE_LEFT_MR_RECT, &START_PLACE_MR_RECT);
		animation_set_duration((Animation*) line->from_the_left_anim, ANIM_DURATION);
		animation_set_curve((Animation*) line->from_the_left_anim, AnimationCurveEaseOut);
		AnimationHandlers handlerWeather = {
			.stopped = (AnimationStoppedHandler) animationStoppedHandler
		};
		animation_set_handlers((Animation *) line->go_to_right_anim, handlerWeather, NULL);
		
		isDateShown = 1;
	} else {
		APP_LOG(APP_LOG_LEVEL_INFO, "inside ELSE isDateShown");
		line->go_to_right_anim = property_animation_create_layer_frame(layer_weather, &START_PLACE_MR_RECT, &TO_RIGHT_MR_RECT);
		animation_set_duration((Animation*) line->go_to_right_anim, ANIM_DURATION);
		animation_set_curve((Animation*) line->go_to_right_anim, AnimationCurveEaseOut);
		AnimationHandlers handlerWeather = {
			.stopped = (AnimationStoppedHandler) animationStoppedHandler
		};
		animation_set_handlers((Animation *) line->go_to_right_anim, handlerWeather, NULL);
		
		line->from_the_left_anim = property_animation_create_layer_frame(layer_date, &FROM_THE_LEFT_MR_RECT, &START_PLACE_MR_RECT);
		animation_set_duration((Animation*) line->from_the_left_anim, ANIM_DURATION);
		animation_set_curve((Animation*) line->from_the_left_anim, AnimationCurveEaseOut);
		AnimationHandlers handlerDate = {
			.stopped = (AnimationStoppedHandler) animationStoppedHandler
		};
		animation_set_handlers((Animation *) line->go_to_right_anim, handlerDate, NULL);
		
		isDateShown = 0;
	}
}	

/**
 * Initialisation for Basalt
 */
void init_animations_basalt(Line *line) {
  APP_LOG(APP_LOG_LEVEL_INFO, "inside init_animations_basalt");
	Layer *layer_date = text_layer_get_layer(line->date_layer);
	Layer *layer_weather = text_layer_get_layer(line->weather_layer);

	if (isDateShown == 0) {
		APP_LOG(APP_LOG_LEVEL_INFO, "inside IF isDateShown");
		line->go_to_right_anim = property_animation_create_layer_frame(layer_date, &START_PLACE_MR_RECT, &TO_RIGHT_MR_RECT);
		animation_set_duration((Animation*) line->go_to_right_anim, ANIM_DURATION);
		animation_set_curve((Animation*) line->go_to_right_anim, AnimationCurveEaseOut);

		line->from_the_left_anim = property_animation_create_layer_frame(layer_weather, &FROM_THE_LEFT_MR_RECT, &START_PLACE_MR_RECT);
		animation_set_duration((Animation*) line->from_the_left_anim, ANIM_DURATION);
		animation_set_curve((Animation*) line->from_the_left_anim, AnimationCurveEaseOut);
		
		isDateShown = 1;
	} else {
		APP_LOG(APP_LOG_LEVEL_INFO, "inside ELSE isDateShown");
		line->go_to_right_anim = property_animation_create_layer_frame(layer_weather, &START_PLACE_MR_RECT, &TO_RIGHT_MR_RECT);
		animation_set_duration((Animation*) line->go_to_right_anim, ANIM_DURATION);
		animation_set_curve((Animation*) line->go_to_right_anim, AnimationCurveEaseOut);
		
		line->from_the_left_anim = property_animation_create_layer_frame(layer_date, &FROM_THE_LEFT_MR_RECT, &START_PLACE_MR_RECT);
		animation_set_duration((Animation*) line->from_the_left_anim, ANIM_DURATION);
		animation_set_curve((Animation*) line->from_the_left_anim, AnimationCurveEaseOut);
		
		isDateShown = 0;
	}
}

/**
 * Handle Animations
 */ 
void schedule_animation(Line *line) {
  static bool use_go_to_right_rect = false;

  if (use_go_to_right_rect) {
		APP_LOG(APP_LOG_LEVEL_INFO, "schedule_animation inside IF");
		animation_set_delay((Animation*) line->go_to_right_anim, ANIM_DELAY);
		animation_schedule((Animation*) line->go_to_right_anim);
		
		animation_set_delay((Animation*) line->from_the_left_anim, ANIM_DELAY);
		animation_schedule((Animation*) line->from_the_left_anim);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "schedule_animation inside ELSE");
		animation_set_delay((Animation*) line->from_the_left_anim, ANIM_DELAY);
		animation_schedule((Animation*) line->from_the_left_anim);
		
		animation_set_delay((Animation*) line->go_to_right_anim, ANIM_DELAY);
		animation_schedule((Animation*) line->go_to_right_anim);
  }

  use_go_to_right_rect = !use_go_to_right_rect;
}

/**
 * start animation process
 */
void start_animation_process(Line *line) {
	APP_LOG(APP_LOG_LEVEL_INFO, "start_animation_process");
	//
	#ifndef PBL_PLATFORM_APLITE
		init_animations_basalt(line);
	#else
		init_animations_aplite(line);
	#endif	
	schedule_animation(line);
}

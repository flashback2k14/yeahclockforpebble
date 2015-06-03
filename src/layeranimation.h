#include <pebble.h>

#ifndef LAYERANIMATION_H
#define LAYERANIMATION_H
	
/**
 * Global Methods for Aplite
 */
extern void animationStoppedHandler(Animation *anim, bool finished, void *context);
extern void init_animations_aplite(Line *line);

/**
 * Global Methods for Basalt
 */
extern void init_animations_basalt(Line *line);

/**
 * Global Methods - Non Device-specific
 */
extern void schedule_animation(Line *line);
extern void start_animation_process(Line *line);

#endif	
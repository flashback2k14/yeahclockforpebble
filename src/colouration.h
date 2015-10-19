#include <pebble.h>

#ifndef COLOURATION_H
#define COLOURATION_H
	
	
	
extern GColor getColor(char *colorName);	
extern void setUserColor(Tuple *t, TextLayer *layer, bool isBackground);	
extern void setUserWindowBGColor(Tuple *t, Window *window);	
extern GColor get_user_color(GColor defaultColor, char *colorName);	
	
#endif
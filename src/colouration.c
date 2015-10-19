#include <pebble.h>
#include "colouration.h"

/**
 * get color from string
 */
GColor getColor(char *colorName) {
	#ifdef PBL_COLOR
		if (strcmp(colorName, "GColorInchworm") == 0) return GColorInchworm;
		else if (strcmp(colorName, "GColorBrightGreen") == 0) return GColorBrightGreen;
		else if (strcmp(colorName, "GColorSpringBud") == 0) return GColorSpringBud;
		else if (strcmp(colorName, "GColorGreen") == 0) return GColorGreen;
		else if (strcmp(colorName, "GColorMintGreen") == 0) return GColorMintGreen;
		else if (strcmp(colorName, "GColorScreaminGreen") == 0) return GColorScreaminGreen;
		else if (strcmp(colorName, "GColorMalachite") == 0) return GColorMalachite;
		else if (strcmp(colorName, "GColorIslamicGreen") == 0) return GColorIslamicGreen;
		else if (strcmp(colorName, "GColorKellyGreen") == 0) return GColorKellyGreen;
		else if (strcmp(colorName, "GColorBrass") == 0) return GColorBrass;
		else if (strcmp(colorName, "GColorLimerick") == 0) return GColorLimerick;
		else if (strcmp(colorName, "GColorYellow") == 0) return GColorYellow;
		else if (strcmp(colorName, "GColorIcterine") == 0) return GColorIcterine;
		else if (strcmp(colorName, "GColorPastelYellow") == 0) return GColorPastelYellow;
		else if (strcmp(colorName, "GColorRajah") == 0) return GColorRajah;
		else if (strcmp(colorName, "GColorChromeYellow") == 0) return GColorChromeYellow;
		else if (strcmp(colorName, "GColorOrange") == 0) return GColorOrange;
		else if (strcmp(colorName, "GColorMelon") == 0) return GColorMelon;
		else if (strcmp(colorName, "GColorRed") == 0) return GColorRed;
		else if (strcmp(colorName, "GColorWindsorTan") == 0) return GColorWindsorTan;
		else if (strcmp(colorName, "GColorArmyGreen") == 0) return GColorArmyGreen;
		else if (strcmp(colorName, "GColorDarkGreen") == 0) return GColorDarkGreen;
		else if (strcmp(colorName, "GColorMayGreen") == 0) return GColorMayGreen;
		else if (strcmp(colorName, "GColorJaegerGreen") == 0) return GColorJaegerGreen;
		else if (strcmp(colorName, "GColorMediumAquamarine") == 0) return GColorMediumAquamarine;
		else if (strcmp(colorName, "GColorMediumSpringGreen") == 0) return GColorMediumSpringGreen;
		else if (strcmp(colorName, "GColorTiffanyBlue") == 0) return GColorTiffanyBlue;
		else if (strcmp(colorName, "GColorCadetBlue") == 0) return GColorCadetBlue;
		else if (strcmp(colorName, "GColorMidnightGreen") == 0) return GColorMidnightGreen;
		else if (strcmp(colorName, "GColorRoseVale") == 0) return GColorRoseVale;
		else if (strcmp(colorName, "GColorFolly") == 0) return GColorFolly;
		else if (strcmp(colorName, "GColorDarkCandyAppleRed") == 0) return GColorDarkCandyAppleRed;
		else if (strcmp(colorName, "GColorBulgarianRose") == 0) return GColorBulgarianRose;
		else if (strcmp(colorName, "GColorCobaltBlue") == 0) return GColorCobaltBlue;
		else if (strcmp(colorName, "GColorCyan") == 0) return GColorCyan;
		else if (strcmp(colorName, "GColorElectricBlue") == 0) return GColorElectricBlue;
		else if (strcmp(colorName, "GColorCeleste") == 0) return GColorCeleste;
		else if (strcmp(colorName, "GColorVividCerulean") == 0) return GColorVividCerulean;
		else if (strcmp(colorName, "GColorDukeBlue") == 0) return GColorDukeBlue;
		else if (strcmp(colorName, "GColorOxfordBlue") == 0) return GColorOxfordBlue;
		else if (strcmp(colorName, "GColorImperialPurple") == 0) return GColorImperialPurple;
		else if (strcmp(colorName, "GColorJazzberryJam") == 0) return GColorJazzberryJam;
		else if (strcmp(colorName, "GColorFashionMagenta") == 0) return GColorFashionMagenta;
		else if (strcmp(colorName, "GColorBrilliantRose") == 0) return GColorBrilliantRose;
		else if (strcmp(colorName, "GColorMagenta") == 0) return GColorMagenta;
		else if (strcmp(colorName, "GColorPurple") == 0) return GColorPurple;
		else if (strcmp(colorName, "GColorIndigo") == 0) return GColorIndigo;
		else if (strcmp(colorName, "GColorElectricUltramarine") == 0) return GColorElectricUltramarine;
		else if (strcmp(colorName, "GColorBlue") == 0) return GColorBlue;
		else if (strcmp(colorName, "GColorPictonBlue") == 0) return GColorPictonBlue;
		else if (strcmp(colorName, "GColorBlueMoon") == 0) return GColorBlueMoon;
		else if (strcmp(colorName, "GColorVeryLightBlue") == 0) return GColorVeryLightBlue;
		else if (strcmp(colorName, "GColorLiberty") == 0) return GColorLiberty;
		else if (strcmp(colorName, "GColorVividViolet") == 0) return GColorVividViolet;
		else if (strcmp(colorName, "GColorPurpureus") == 0) return GColorPurpureus;
		else if (strcmp(colorName, "GColorShockingPink") == 0) return GColorShockingPink;
		else if (strcmp(colorName, "GColorRichBrilliantLavender") == 0) return GColorRichBrilliantLavender;
		else if (strcmp(colorName, "GColorBabyBlueEyes") == 0) return GColorBabyBlueEyes;
		else if (strcmp(colorName, "GColorLavenderIndigo") == 0) return GColorLavenderIndigo;
		else if (strcmp(colorName, "GColorWhite") == 0) return GColorWhite;
		else if (strcmp(colorName, "GColorLightGray") == 0) return GColorLightGray;
		else if (strcmp(colorName, "GColorDarkGray") == 0) return GColorDarkGray;
		else if (strcmp(colorName, "GColorBlack") == 0) return GColorBlack;
		else return GColorClear;
	#else
		if (strcmp(colorName, "GColorWhite") == 0) return GColorWhite;
		else if (strcmp(colorName, "GColorClear") == 0) return GColorClear;
		else return GColorBlack;
	#endif
}
/**
 * set Color to Text or Background
 */
void setUserColor(Tuple *t, TextLayer *layer, bool isBackground) {
	char colorNameFromTuple[30];
	strcpy(colorNameFromTuple, t->value->cstring);
		
	if (isBackground) text_layer_set_background_color(layer, getColor(colorNameFromTuple));
	else text_layer_set_text_color(layer, getColor(colorNameFromTuple));
}
/**
 * set Window Background Color
 */
void setUserWindowBGColor(Tuple *t, Window *window) {
	char colorNameFromTuple[30];
	strcpy(colorNameFromTuple, t->value->cstring);
	window_set_background_color(window, getColor(colorNameFromTuple));
}
/**
 * get color from user customisation
 */
GColor get_user_color(GColor defaultColor, char *colorName) {
	return (strcmp(colorName, "") == 0) ? defaultColor : getColor(colorName);
}
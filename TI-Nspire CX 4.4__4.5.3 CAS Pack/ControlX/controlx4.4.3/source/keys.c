#include "types.h"
#include "keys.h"

uint8_t getMenuKey() {
	int option=0xFF;
	if(is_key_pressed(KEY_1)) option=1;
	else if(is_key_pressed(KEY_2)) option=2;
	else if(is_key_pressed(KEY_3)) option=3;
	else if(is_key_pressed(KEY_4)) option=4;
	else if(is_key_pressed(KEY_5)) option=5;
	else if(is_key_pressed(KEY_6)) option=6;
	else if(is_key_pressed(KEY_7)) option=7;
	else if(is_key_pressed(KEY_8)) option=8;
	else if(is_key_pressed(KEY_9)) option=9;
	else if(is_key_pressed(KEY_A)) option=10;
	else if(is_key_pressed(KEY_B)) option=11;
	else if(is_key_pressed(KEY_C)) option=12;
	else if(is_key_pressed(KEY_L)) option=21;
	else if(is_key_pressed(KEY_X)) option=33;
	else if(is_key_pressed(KEY_Z)) option=35;
	else if(is_key_pressed(KEY_0) || is_key_pressed(KEY_ESC)) option=0;
	return option;
}

uint8_t isModifierKeyPressed() {
	return is_key_pressed(KEY_CTRL) || is_key_pressed(KEY_SHIFT);
}

void whileKeyPressed() {
	while(getMenuKey()!=NO_OPTION || isModifierKeyPressed());
}
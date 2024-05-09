typedef struct {int row;int col} key_t;

#define NO_OPTION	0xFF

#define KEY_0		(key_t){0,7}
#define KEY_1		(key_t){1,7}
#define KEY_2		(key_t){6,4}
#define KEY_3		(key_t){1,3}
#define KEY_4		(key_t){2,7}
#define KEY_5		(key_t){5,6}
#define KEY_6		(key_t){2,3}
#define KEY_7		(key_t){3,7}
#define KEY_8		(key_t){6,6}
#define KEY_9		(key_t){3,3}
#define KEY_A		(key_t){4,6}
#define KEY_B		(key_t){4,5}
#define KEY_C		(key_t){4,4}
#define KEY_L		(key_t){3,0}
#define KEY_X		(key_t){1,0}
#define KEY_Z		(key_t){0,5}
#define KEY_ENTER	(key_t){0,1}
#define KEY_EE		(key_t){2,8}
#define KEY_VAR		(key_t){5,1}
#define KEY_MINUS	(key_t){5,2}
#define KEY_PLUS	(key_t){6,2}
#define KEY_DOC		(key_t){6,3}
#define KEY_MENU	(key_t){6,5}
#define KEY_ESC		(key_t){6,7}
#define KEY_TAB		(key_t){6,9}
#define KEY_SHIFT	(key_t){7,8}
#define KEY_CTRL	(key_t){7,9}

uint8_t getMenuKey();
uint8_t isModifierKeyPressed();
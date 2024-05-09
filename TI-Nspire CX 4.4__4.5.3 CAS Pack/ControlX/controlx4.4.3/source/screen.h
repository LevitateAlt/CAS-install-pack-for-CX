#include "types.h"
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240

#define SCREEN_BASE_PTR		0xC0000010 
unsigned char* SCREEN_BASE_ADDR;

void init_screen();
void setBufPixelRGB(unsigned char* buf, int x, int y, unsigned int r, unsigned int g, unsigned int b);
void putBufChar(unsigned short int* buf, int x, int y, char ch, int trsp);
void drwBufHoriz(unsigned char* buf, int y, int x1, int x2);
void drwBufVert(unsigned char* buf, int x, int y1, int y2);
void drwBufBox(unsigned char* buf, int x1, int y1, int x2, int y2);
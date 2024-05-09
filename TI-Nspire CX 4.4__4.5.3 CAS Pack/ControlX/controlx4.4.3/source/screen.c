#include "types.h"
#include "boot1.h"
#include "screen.h"
#include "charmaps.h"

unsigned char curr=0;
unsigned char curg=0;
unsigned char curb=0;
unsigned char* SCREEN_BASE_ADDR = 0;

void setCurColorRGB(unsigned int r,unsigned int g,unsigned int b) {
	curr=(r>>3);
	curg=(g>>2);
	curb=(b>>3);
}

void init_screen() {
	SCREEN_BASE_ADDR	=*(unsigned char**)SCREEN_BASE_PTR;
}

void setBufPixelRGB(unsigned char* buf, int x, int y, unsigned int r, unsigned int g, unsigned int b)
{	if(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
	{	unsigned int color=(r<<11)|(g<<5)|b;
		buf[(y*SCREEN_WIDTH+x)*2]=color&0b0000000011111111;
		buf[(y*SCREEN_WIDTH+x)*2+1]=(color&0b1111111100000000)>>8;
	}
}

void putBufChar(unsigned short int* buf, int x, int y, char ch, int trsp)
{
  int i, j;
  unsigned char pixelOn;
  for(i = 0; i < CHAR_HEIGHT; i++)  
  {
    for(j = 0; j < CHAR_WIDTH; j++) 
    {
		pixelOn = _font_bits[(unsigned char)ch][i];
		pixelOn = pixelOn<<j;
//      pixelOn = charMap_ascii[(unsigned char)ch][i] << j ;  
      pixelOn = pixelOn & 0x80 ;  
	  int back=0xFF;
	  if(curr==255 && curg==255 && curb==255) back=0;
      if (pixelOn)
			setBufPixelRGB(buf,x + j, y + i, curr, curg, curb); 
      else if(!trsp)
			setBufPixelRGB(buf,x + j, y + i, back, back, back);
    }
  } 
}

void drwBufVert(unsigned char* buf, int x, int y1, int y2)
{	int ym = (y1<y2)?y2:y1;
	int yi = (y1<y2)?y1:y2;
	for(;yi<=ym;yi++)
		setBufPixelRGB(buf,x,yi,curr,curg,curb);
}

void drawFastHoriz(unsigned char* buf, int y, int x1, int x2, uint8_t val)
{	int m = (x1<x2)?x2:x1;
	int i = (x1<x2)?x1:x2;
	memset(buf+y*SCREEN_WIDTH*2+x1*2,val,(m-i+1)*2);
}

void clearLines(unsigned char* buf, int y1, int y2)
{	int ym = (y1<y2)?y2:y1;
	int yi = (y1<y2)?y1:y2;
	memset(buf+yi*SCREEN_WIDTH*2,0xFF,(ym-yi+1)*SCREEN_WIDTH*2);
}
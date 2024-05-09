#include "console.h"
#include "screen.h"
#include "boot1.h"
#include "tools.h"

uint8_t console_mode=0;

uint8_t col=0;
uint8_t line=0;

uint8_t getCol() {
	return col;
}

void setLine(uint8_t newline) {
	line=newline;
}

void setCol(uint8_t newcol) {
	col=newcol;
}

void dispBuf( uint8_t* buf, char* message, uint8_t newline) {
  uint8_t i;
  char c;
  for (i = 0; i < strlen(message) && col<MAX_COL; i++) {
	c=message[i];
	if(c=='\n') {
		col=-1;
		line++;
		
	}
	else
		putBufChar(buf, col*CHAR_WIDTH, line*CHAR_HEIGHT, c, console_mode&TRSP_MODE);
	col++;
  }
  if(newline) {
	  col=0;
	  line++;
  }
}

void disp(char* msg) {
	dispBuf(SCREEN_BASE_ADDR,msg,0);
}

void displn(char* msg) {
	dispBuf(SCREEN_BASE_ADDR,msg,1);
}

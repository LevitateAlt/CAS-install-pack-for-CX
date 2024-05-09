#include <os.h>
#include "screen.h"
#include "console.h"
#include "tools.h"
#include "charmaps.h"

int col=0;
int line=0;
int mode=0;

int getMode()
{	return mode;
}

void setMode(int m)
{	mode=m;
}

int getLine()
{	return line;
}

void setLine(int nline)
{	line=nline;
}

int getCol()
{	return col;
}

void setCol(int ncol)
{	col=ncol;
}

void dispBuf( unsigned char* buf, char* message, int ret,int trsp )
{ int l = strlen(message);
  int i, stop=0;
  for (i = 0; i < l && !stop; i++) {
	char c = message[i];
	  
	if(c==0x0A) {
		if(ret) {
			col=0;
			line++;
		}
		else
			c=' ';
	}
	
	if(c!=0x0A) {
      putBufChar(buf, col*CHAR_WIDTH, line*CHAR_HEIGHT, c, trsp);
	  if(mode&VERT_MODE) {
		  line++;
		  if(line>=MAX_LGN)
          { if ( !ret ) stop=1;
            else
            { line = 0;
              col ++;
            }
          }
          if(line>=MAX_COL) { col=0; }
	  }
	  else {
		col ++;
        if (col >= MAX_COL)
        { if ( !ret ) stop=1;
          else
          { col = 0;
            line ++;
          }
        }
		if(line>=MAX_LGN) { line=0; }
	  }
    }
  }
}

void dispBufc(unsigned char* buf, char c, int ret, int trsp)
{	char* tmp = malloc(2);
	sprintf(tmp,"%c",c);
	dispBuf(buf,tmp,ret,trsp);
	free(tmp);
}

void disp(char* msg, int ret, int trsp)
{	dispBuf(getScreen(),msg,ret,trsp);
}

void dispc(char c, int ret, int trsp)
{	char* tmp = malloc(2);
	sprintf(tmp,"%c",c);
	disp(tmp,ret,trsp);
	free(tmp);
}

void dispi(int i, int ret, int trsp)
{	char* tmp = malloc(32);
	sprintf(tmp,"%i",i);
	disp(tmp,ret,trsp);
	free(tmp);
}

void displnBuf( unsigned char* buf, char* message, int ret,int trsp )
{	dispBuf(buf, message, ret,trsp);
	col=0;
	line++;
	if(line>=MAX_LGN) { line=0; }
}

void displn(char* msg, int ret,int trsp)
{	displnBuf(getScreen(),msg,ret,trsp);
}

void pausecons( char* message, int ret, int trsp, int invite)
{	if(message)
		if(*message)
			displn(message, ret,trsp);
	if(invite)
		disp("Press [esc] to continue...", 0,trsp);
	while(!isKeyPressed(KEY_NSPIRE_ESC)) {}
	displn("",0,trsp);
}

void resetConsole()
{	col=0;
	line=0;
}
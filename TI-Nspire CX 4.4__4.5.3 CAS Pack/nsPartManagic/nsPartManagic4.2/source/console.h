#define CHAR_HEIGHT 10
#define CHAR_WIDTH 6

#define VERT_MODE 1

#define MAX_COL SCREEN_WIDTH/CHAR_WIDTH
#define MAX_LGN SCREEN_HEIGHT/CHAR_HEIGHT
#define LSEPARATOR "----------------------------------------"

void dispBuf( unsigned char* buf, char* message, int ret,int trsp );
void disp(char* msg, int ret,int trsp);
void displnBuf( unsigned char* buf, char* message, int ret,int trsp );
void displn(char* msg, int ret,int trsp);
void resetConsole();
void setCol(int col);
int getCol();
void dispi(int i, int ret, int trsp);
int getLine();
void setLine(int nline);
void pausecons( char* message, int ret,int trsp, int invite);

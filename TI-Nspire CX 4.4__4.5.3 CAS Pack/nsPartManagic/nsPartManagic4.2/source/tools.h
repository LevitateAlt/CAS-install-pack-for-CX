#define PATH_SIZE 128
#define NBR_SIZE 16

#define max(a,b) (((a)<(b))?(b):(a))
#define min(a,b) (((a)<(b))?(a):(b))

int filelength(FILE* file);
int waitConfirm();
int chex2int(char c);
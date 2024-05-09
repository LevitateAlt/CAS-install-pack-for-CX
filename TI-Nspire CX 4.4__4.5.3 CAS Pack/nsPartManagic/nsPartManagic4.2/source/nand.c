#include <os.h>
#include "nand.h"

char locales[NLANG+1][LOCAL_SIZE]  = {	'\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF','\xFF',
										'z'   ,'h'   ,'_'   ,'C'   ,'N'   ,'\x00','\x00','\x00',
										'a'   ,'r'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'f'   ,'r'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'd'   ,'a'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'd'   ,'e'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'e'   ,'n'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'e'   ,'n'   ,'_'   ,'G'   ,'B'   ,'\x00','\x00','\x00',
										'e'   ,'s'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'f'   ,'i'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'i'   ,'t'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'n'   ,'l'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'n'   ,'l'   ,'_'   ,'B'   ,'E'   ,'\x00','\x00','\x00',
										'n'   ,'o'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'p'   ,'t'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										's'   ,'v'   ,'\x00','\x00','\x00','\x00','\x00','\x00',
										'z'   ,'h'   ,'_'   ,'T'   ,'W'   ,'\x00','\x00','\x00'};
char dlocales[NLANG+1][9] = {'n' ,'o' ,'n' ,'e' ,'\0','\0','\0','\0','\0',
							  'z' ,'h' ,'C' ,'N' ,'\0','\0','\0','\0','\0',
							  'a' ,'r' ,'\0','\0','\0','\0','\0','\0','\0',
							  'f' ,'r' ,'\0','\0','\0','\0','\0','\0','\0',
							  'd' ,'a' ,'\0','\0','\0','\0','\0','\0','\0',
							  'd' ,'e' ,'\0','\0','\0','\0','\0','\0','\0',
							  'e' ,'n' ,'\0','\0','\0','\0','\0','\0','\0',
							  'e' ,'n' ,'G' ,'B' ,'\0','\0','\0','\0','\0',
							  'e' ,'s' ,'\0','\0','\0','\0','\0','\0','\0',
							  'f' ,'i' ,'\0','\0','\0','\0','\0','\0','\0',
							  'i' ,'t' ,'\0','\0','\0','\0','\0','\0','\0',
							  'n' ,'l' ,'\0','\0','\0','\0','\0','\0','\0',
							  'n' ,'l' ,'B' ,'E' ,'\0','\0','\0','\0','\0',
							  'n' ,'o' ,'\0','\0','\0','\0','\0','\0','\0',
							  'p' ,'t' ,'\0','\0','\0','\0','\0','\0','\0',
							  's' ,'v' ,'\0','\0','\0','\0','\0','\0','\0',
							  'z' ,'h' ,'T' ,'W' ,'\0','\0','\0','\0','\0'};


int eraseFlash(int start, int end)
{	int result = nand_erase_range(start, end);
	return !result;
}

int writeFlash(char* data, int size, int start)
{	int result = write_nand(data, size, start);
	return !result;
}

int getLocale(char* buf)
{	int i,j;
	for(i=0;i<NLANG+1;i++)
	{	for(j=0;j<LOCAL_SIZE;j++)
		{	if(locales[i][j]!=buf[j])
				break;
		}
		if(j>=LOCAL_SIZE)
			return i;
	}
	return 0;
}

// backward compatible
void bc_read_nand(void* dest, int size, int offset, int unknown, int percent_max, void *progress_cb) {
	if (nl_ndless_rev() < 989) // Ndless 3.1
		read_nand_31(dest, size, offset, unknown, percent_max, progress_cb);
	else
		read_nand(dest, size, offset, unknown, percent_max, progress_cb);
}

int bc_write_nand(char* data, int size, int start) {
	if (nl_ndless_rev() < 989) // Ndless 3.1
		write_nand_31(data, size, start);
	else
		write_nand(data, size, start);
	return 0;
}

int bc_nand_erase_range(int start, int end) {
	if (nl_ndless_rev() < 989) // Ndless 3.1
		nand_erase_range_31(start, end);
	else
		nand_erase_range(start, end);
	return 0;
}

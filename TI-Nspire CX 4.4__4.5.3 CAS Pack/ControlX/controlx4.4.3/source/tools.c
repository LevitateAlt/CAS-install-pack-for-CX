#include "types.h"
#include "boot1.h"

static char* hexdigits="0123456789ABCDEFG";
char hexdigstr[2]={0,0};

uint8_t chex2int(char c)
{	if(c>='0' && c<='9')
		return c-'0';
	if(c>='A' && c<='F')
		return c-'A'+10;
	if(c>='a' && c<='f')
		return c-'a'+10;
	return 0;
}

char* hexdigtoa(uint8_t i) {
	hexdigstr[0]=*(hexdigits+(i&0xF));
	return hexdigstr;
}

void outhex(uint32_t i,uint8_t o) {
	uint8_t j=8,sign=0;
	for(uint8_t j=8;j>0;j--) {
		uint8_t d=(i&0xF0000000)>>28;
		if(d)
			sign=1;
		if(d || sign) {
			char* p=hexdigtoa(d);
			if(o)
				disp(p);
			else
				puts(p);
		}
		i=i<<4;
	}
	if(!o)
		puts("\n");
}
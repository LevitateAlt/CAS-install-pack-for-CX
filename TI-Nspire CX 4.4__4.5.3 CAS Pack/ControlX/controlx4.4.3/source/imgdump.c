#include "types.h"
#include "boot1.h"
#include "linuxloader.h"
/* Decompression for boot2 */

unsigned char getbyte(char** p) {
	int byte = *(*p);
	*p=(*p)+1;
	if (byte < 0) {
		puts("EOF reached unexpectedly\n");
	}
	return byte;
}

unsigned short gethalfword(char** p) {
	unsigned char hi = getbyte(p);
	return hi << 8 | getbyte(p);
}

unsigned int getword(char** p) {
	unsigned short hi = gethalfword(p);
	return hi << 16 | gethalfword(p);
}

int getbits(unsigned int n, char** p) {
	static unsigned int buf = 0, bits = 0;
	while (bits < n) {
		buf = buf << 8 | getbyte(p);
		bits += 8;
	}
	bits -= n;
	return buf >> bits & ((1 << n) - 1);
}

void decompress(unsigned int comp_size, char** p, char** outp) {
	unsigned int size = getword(p);
	unsigned short common[64];
	int i;

	for (i = 0; i < 64; i++)
		common[i] = gethalfword(p);

	for (; size > 0; size -= 2) {
		unsigned short hw;
		if (getbits(1,p))
			hw = getbits(16,p);
		else
			hw = common[getbits(6,p)];
		**outp = hw >> 8;
		(*outp)++;
		**outp = hw;
		(*outp)++;
	}
}

int isValidField(int field) {
	return field>=0x8000 && field<0x9000 || field>=0x0200 && field<0x0400;
}

unsigned int getSize(unsigned int size,char** pp) {
	size = size & 0x000F;
	if (size == 0x0D)      size = getbyte(pp);
	else if (size == 0x0E) size = gethalfword(pp);
	else if (size == 0x0F) size = getword(pp);
	return size;
}

int getFieldData(char* buf, int cfield, int num, int dsize, char** pptr, char** fptr)
{	char* p = buf;
	unsigned int size=0;
	int inum=0;
	char* cfptr;
	while (p-buf<dsize) {
		cfptr=p;
		unsigned short field = gethalfword(&p);
		if(p-buf>dsize) break;
		size = field & 0x000F;
		field &= 0xFFF0;
		if (size == 0x0D)      size = getbyte(&p);
		else if (size == 0x0E) size = gethalfword(&p);
		else if (size == 0x0F) size = getword(&p);
		if (field == cfield) {
			inum++;
			if(!num || inum==num)
			{
				if(pptr) *pptr=p;
				if(fptr) *fptr=cfptr;
				return (p-buf+size<=dsize)?size:0;
			}
		} else if(!isValidField(field)) {
			break;
		}
		p+=size;
	}
	return 0;
}

int getFieldDataSize(char* buf, int cfield, int num, int dsize)
{	return getFieldData(buf,cfield,num,dsize,0,0);
}

char* getFieldDataPtr(char* buf, int cfield, int num, int dsize)
{	char* ptr=0;
	getFieldData(buf,cfield,num,dsize,&ptr,0);
	return ptr;
}

void copyFieldData(char* buf, int cfield, int num, int dsize, char* outbuf, int bufsize) {
	unsigned int size=getFieldDataSize(buf,cfield,num,dsize);
	if(size>0) {
		char* data=getFieldDataPtr(buf,cfield,num,dsize);
		if(size>bufsize) size=bufsize;
		memcpy(outbuf,data,size);
	}
}

int getTotalFieldsSize(char* buf, int dsize, unsigned int* attachsize) {
	char* p = buf;
	char* lastp = p;

	while (p-buf<dsize) {
		unsigned short field = gethalfword(&p);
		unsigned short cfield = field & 0xFFF0;
		if((field!=0xFFF0 && !isValidField(field)) || (cfield==0x8000 && p-buf>2)) {
			p-=2;
			break;
		}
		if(p-buf>dsize) return 0;
		unsigned int size=getSize(field,&p);
		if(cfield==0x8000 && !getFieldDataSize(p,0x8070,1,size)) {
			char* data8080=0;
			int size8080=getFieldData(p,0x8080,0,size,&data8080,0);
			if(*data8080) {
				unsigned int hsize=0, lsize=0;
				memcpy(&hsize,data8080+4,4);
				if(hsize>=data8080-buf+4) hsize-=data8080-buf+4;
				LLHeader header;
				memcpy(&header,data8080+4+hsize-sizeof(LLHeader),sizeof(LLHeader));
				if(header.signature == LLHeader_sig) {
					lsize+=header.dtb_size+header.initrd_size+header.kernel_size;
					if(attachsize) *attachsize=lsize;
				}
				if((data8080-buf)+4+hsize+lsize>dsize) return 0;
				return (data8080-buf)+4+hsize+lsize;
			}
		}
		p+=size;
		lastp=p;
		if(p-buf>dsize) return 0;
		if(cfield==0xFFF0)
			break;
	}
	return lastp-buf;
}

int decompressFiles(char* buf, int size, char* outbuf)
{	char* p = buf;
	unsigned int flags=0;
	char* outp=outbuf;
	unsigned int addr=0;
	int size8000=0;
	char* data8000=0;
	while (1) {
		unsigned short field = gethalfword(&p);
		unsigned int size = field & 0x000F;
		field &= 0xFFF0;
		if(isValidField(field)) {
			if (size == 0x0D)      size = getbyte(&p);
			else if (size == 0x0E) size = gethalfword(&p);
			else if (size == 0x0F) size = getword(&p);
		}
		if (field == 0x8000) {
			/* Don't skip - 8070 is inside this */
			size8000=size;
			data8000=p;
		}
		else if (field == 0x8080) {
			if (size < 8) {
				return 1;
			}
			addr = getword(&p);
			memcpy(&flags,p,4);
			p+=4;
//			flags = getword(&p);
		} else if (field == 0x8070) {
			if (flags) {
				outp = outbuf;
				decompress(size,&p,&outp);
				break;
			} else {
				memcpy(outbuf,p,size);
				p+=size;
				outp=outbuf+size;
				break;
				//return 1;
			}
		} else if (field == 0xFFF0) {
			break;
		} else if(size8000 && addr && flags && p-data8000>=size8000) {
			p-=2;
			size=flags;
			size-=4;
			memcpy(outbuf,p,size);
			p+=size;
			outp=outbuf+size;
			break;
		} else {
			/* Skip it */
			p+=size;
		}
	}
	return outp-(outbuf);
}

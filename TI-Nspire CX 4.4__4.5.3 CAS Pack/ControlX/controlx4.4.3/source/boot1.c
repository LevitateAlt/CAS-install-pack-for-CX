#include "types.h"

void (*puts)(const char *str) = (void*) 0x368;
void (*load_start_boot2)() = (void*)0x1C7C;
//int (*memcmp)(const void* ptr1, const void* ptr2, int size) = (void*) 0xFF18;
//void (*memcpy)(const void* dest, const void* src, int size) = (void*) 0xFFB0;
void (*memset)(void* dest, int val, int size) = (void*) 0x10170;
//int (*strlen)(char* str) = (void*) 0x102d4;
int (*read_nand)(void* dest, uint32_t size, uint32_t nand_offset, int progress_start, int progress_end, void* progress_cb) = (void*) 0xb164;
int (*nand_erase_range)(uint32_t start, uint32_t end) = (void*)0xB11C;
int (*write_nand)(void* data, uint32_t size, uint32_t nand_offset) = (void*) 0xb464;
void (*itoa)(uint32_t n, char* s, int radix) = (void*) 0x5c;
int (*is_any_key_pressed)()= (void*)0x19974;
int (*is_rc_pressed)(int row, int col) = (void*) 0x197D0;
void *progress_callback = (void*) 0xA40;
void (*reset)()= (void*)0;

int memcmp(const void* ptr1, const void* ptr2, uint32_t size) {
	uint8_t* cptr1=(uint8_t*)ptr1;
	uint8_t* cptr2=(uint8_t*)ptr2;
	uint32_t i;
	for(i=0;i<size;i++)
		if(cptr1[i]!=cptr2[i])
			return cptr1[i]-cptr2[i];
	return 0;
}

void* memcpy(const void* dest, const void* src, uint32_t size) {
	uint8_t* cdest=(uint8_t*)dest;
	uint8_t* csrc=(uint8_t*)src;
	uint32_t i;
	for(i=0;i<size;i++)
		cdest[i]=csrc[i];
	return dest;
}

uint32_t strlen(const char *str) {
	char* ptr=str;
	while(*ptr)
		ptr++;
	return ptr-str;
}

void* memmove(void* dest, void* src, uint32_t size) {
	uint8_t* cdest=(uint8_t*)dest;
	uint8_t* csrc=(uint8_t*)src;
	int32_t i;
	if(cdest==csrc || !size) return;
	if(cdest<csrc) {
		for(i=0;i<size;i++)
			cdest[i]=csrc[i];
	}
	else {
		for(i=size-1;i>=0;i--)
			cdest[i]=csrc[i];
	}
	return dest;
}

const void * memchr ( const void * buf, uint8_t value, uint32_t num ) {
	uint8_t* ptr=buf;
	while(num) {
		if(*ptr==value)
			return ptr;
		ptr++;
		num--;
	}
	return 0;
}

const char * strchr ( const char* str, char character ) {
	return memchr(str, character, strlen(str));
}
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t file_size(const char *path)
{
	FILE *t = fopen(path, "r");
	assert(t);
	assert(fseek(t, 0, SEEK_END) == 0);
	uint32_t ret = ftell(t);
	assert(fclose(t) == 0);
	return ret;
}

void append_file(FILE *dest, const char *src, uint32_t size)
{
	void *buf = malloc(size);
	assert(buf);
	FILE *s = fopen(src, "rb");
	assert(s);
	assert(fread(buf, size, 1, s) == 1);
	assert(fclose(s) == 0);
	assert(fwrite(buf, size, 1, dest) == 1);
}

int main(int argc, char **argv)
{
	if(argc != 4)
	{
		fprintf(stderr, "Usage: %s dtb initrd kernel\n", argv[0]);
		return 1;
	}
	
	FILE *dest = fopen("diags.img.tns", "wb");
	assert(dest);
	append_file(dest, "loader.bin", file_size("loader.bin"));
	uint32_t v = 0xDEDC0FFE;
	assert(fwrite(&v, sizeof(v), 1, dest) == 1);
	uint32_t dtb_size = file_size(argv[1]);
	assert(fwrite(&dtb_size, sizeof(v), 1, dest) == 1);
	uint32_t initrd_size = file_size(argv[2]);
	assert(fwrite(&initrd_size, sizeof(v), 1, dest) == 1);
	uint32_t kernel_size = file_size(argv[3]);
	assert(fwrite(&kernel_size, sizeof(v), 1, dest) == 1);
	append_file(dest, argv[1], dtb_size);
	append_file(dest, argv[2], initrd_size);
	append_file(dest, argv[3], kernel_size);
}
static const uint32_t LLHeader_sig = 0xDEDC0FFE;

typedef struct LLHeader {
	uint32_t signature; // 0xDEDC0FFE
	uint32_t dtb_size;
	uint32_t initrd_size;
	uint32_t kernel_size;
} LLHeader;
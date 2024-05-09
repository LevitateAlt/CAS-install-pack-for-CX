#include <libfdt.h>

#include "header.h"

// Must only be used before loading the kernel. The kernel overwrites the serial hook
void (*boot1_puts)(const char *str) = (void*) 0x368;
int (*boot1_read_nand)(void* dest, uint32_t size, uint32_t nand_offset, int progress_start, int progress_end, void* progress_cb) = (void*) 0xb164;
uint32_t (*boot1_nand_partition_start)(unsigned int nr) = (void*) 0x87ec;
void *progress_callback = (void*) 0xA40;

static int _fdt_make_node(void *fdt, int parentoffset, const char *name) {
    int e = fdt_subnode_offset(fdt, parentoffset, name);
    if(e != FDT_ERR_NOTFOUND)
        return e;

    return fdt_add_subnode(fdt, parentoffset, name);
}

void fail(const char *s)
{
	boot1_puts(s);
	while(1);
}

asm(".section .text._start\n"
	"_start: .global _start\n"
	"ldr sp, =#0xa400c268\n"
	"b main\n");

void main()
{	
	// Disable watchdog
	*(volatile unsigned*)0x90060C00 = 0x1ACCE551;
        *(volatile unsigned*)0x90060008 = 0;
        *(volatile unsigned*)0x90060C00 = 0;
	
	// Enable access to second 32MiB of SDRAM
	volatile uint32_t *mmu_entry = (volatile uint32_t*)0xA4004000;
	mmu_entry += 0x12000000 >> 20;
	for(int i = 0; i < 0x20; ++i)
	{
		*mmu_entry &= ~0xFFF;
		*mmu_entry++ |= 0xc1e;
	}
	
	int zero = 0;
	__asm__ volatile("0: mrc p15, 0, pc, c7, c14, 3;"
			"bne 0b;"
			"0: mrc p15, 0, pc, c7, c10, 3;"
			"bne 0b;"
			"mcr p15, 0, %[zero], c7, c10, 4;"
			"mcr p15, 0, %[zero], c7, c7, 0;"
			"mcr p15, 0, %[zero], c8, c7, 0;"
			:: [zero] "r" (0) : "cc");

	extern char _end;
	LLHeader *header = (LLHeader*)&_end;
	if(header->signature != LLHeader_sig)
		fail("Header not found!\n");
	
	uint32_t current_nand_offset = boot1_nand_partition_start(3) + ((uint32_t)(&_end) - 0x10000000) + sizeof(LLHeader);
	
	// Load fdt first, use kernel region for temporary storage
	void *fdt = kernel_dest;
	if(!boot1_read_nand(fdt, header->dtb_size, current_nand_offset, 0, 10, progress_callback))
		fail("Couldn't read DTB!\n");
	
	current_nand_offset += header->dtb_size;
	
	if(fdt_open_into(fdt, dtb_dest, 0x10000) < 0)
		fail("FDT invalid!\n");
	
	int chosen = _fdt_make_node(dtb_dest, 0, "chosen");
	if(chosen < 0)
		fail("FDT fail\n");
	
	if(fdt_setprop_cell(dtb_dest, chosen, "linux,initrd-start", (uint32_t) initrd_dest) < 0
		|| fdt_setprop_cell(dtb_dest, chosen, "linux,initrd-end", (uint32_t)(initrd_dest) + header->initrd_size) < 0)
		fail("FDT fail 2\n");
	
	fdt_pack(dtb_dest);
	
	// Load initrd
	if(!boot1_read_nand(initrd_dest, header->initrd_size, current_nand_offset, 10, 70, progress_callback))
		fail("Couldn't read initrd!\n");
	
	current_nand_offset += header->initrd_size;
	
	// Load kernel
	if(!boot1_read_nand(kernel_dest, header->kernel_size, current_nand_offset, 70, 100, progress_callback))
		fail("Couldn't read kernel!\n");
	
	// Prepare kernel call
        *(volatile uint32_t*)0x900B0018 = 0;
        *(volatile uint32_t*)0x900B0020 = 0;
	
	asm volatile(
		"prepare:\n"
		"mrc p15, 0, r15, c7, c10, 3 @ test and clean DCache\n"
		"bne prepare\n"
		"mov r0, #0\n"
		"mcr p15, 0, r0, c7, c7, 0 @ invalidate ICache and DCache\n"
		"mrc p15, 0, r0, c1, c0, 0\n"
		"bic r0, r0, #0x5 @ Disable D-Cache and MMU\n"
		"mcr p15, 0, r0, c1, c0, 0\n"
	 : : : "r0", "cc");
	
	void (*kernel)(uint32_t n, uint32_t mach_id, const void *dtb_addr) = kernel_dest;
	kernel(0, ~0, dtb_dest);
}

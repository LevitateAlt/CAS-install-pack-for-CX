#define NAND_PAGE_SIZE		(has_colors?0x800:0x200)
#define NAND_PAGE_BLOCK		(has_colors?0x40:0x20)
#define MODEL_OFFSET		0x804
#define LOCAL_OFFSET		0x808
#define LOCAL_SIZE			8
#define PART_TABLE_OFFSET	0x818
#define PART_TABLE_ID		"\x91\x5F\x9E\x4C"
#define PART_TABLE_SIZE		4
#define CR4_OFFSET			0x81d
#define DIAGS_OFFSET_OFFSET	0x82c
#define BOOT2_OFFSET_OFFSET	0x830
#define BOOTD_OFFSET_OFFSET	0x834
#define FILES_OFFSET_OFFSET	0x838
#define SDRAM_OFFSET		0x840
#define SPIC_OFFSET			0x844
#define SPID_OFFSET			0x848
#define SPID_SIZE			0x40

#define BOOTG_SIZE_OFFSET	0x924

#define MANUF_PAGE_OFFSET	0x000
#define BOOT2_PAGE_OFFSET	0x020
#define BOOTD_PAGE_OFFSET	0xA80
#define DIAGS_PAGE_OFFSET 	0xB00
#define FILES_PAGE_OFFSET	(has_colors?0x800:0x1000)
#define NDEND_PAGE_OFFSET	0x10000

#define DGSTA_PAGE_OFFSET	(has_colors?0x780:0xF80)

#define MANUF_PAGES_NUM		(BOOT2_PAGE_OFFSET-MANUF_PAGE_OFFSET)
#define BOOT2_PAGES_NUM		(BOOTD_PAGE_OFFSET-BOOT2_PAGE_OFFSET)
#define BOOTD_PAGES_NUM		(DIAGS_PAGE_OFFSET-BOOTD_PAGE_OFFSET)
#define DIAGS_PAGES_NUM		(FILES_PAGE_OFFSET-DIAGS_PAGE_OFFSET)
#define FILES_PAGES_NUM		(NDEND_PAGE_OFFSET-FILES_PAGE_OFFSET)

#define DGSTA_PAGE_NUM		(FILES_PAGE_OFFSET-DGSTA_PAGE_OFFSET)

static const unsigned read_nand_31_addrs[]			= {	0x10071F5C, 0x10071EC4, 0x10071658, 0X100715E8, 0x1006E0AC, 0x1006E03C};
static const unsigned write_nand_31_addrs[]			= {	0x10072298, 0x10072200, 0x10071994, 0x10071924, 0x1006E3E8, 0x1006E378};
static const unsigned nand_erase_range_31_addrs[]	= {	0x100724FC, 0x10072464, 0x10071BF8, 0x10071B88, 0x1006E64C, 0x1006E5DC};

#define read_nand_31		SYSCALL_CUSTOM(read_nand_31_addrs ,void,  void* dest, int size, int offset, int, int percent_max, void *progress_cb )
#define write_nand_31		SYSCALL_CUSTOM(write_nand_31_addrs,int,  void *source,int size, unsigned int endoffset)
#define nand_erase_range_31	SYSCALL_CUSTOM(nand_erase_range_31_addrs,int, int offset,int end)

#define NLANG 16
#define LANG_NONE	0
#define LANG_ZHCN	1
#define LANG_AR		2
#define LANG_FR		3
#define LANG_DA		4
#define LANG_DE		5
#define LANG_EN		6
#define LANG_ENGB	7
#define LANG_ES		8
#define LANG_FI		9
#define LANG_IT		10
#define LANG_NL		11
#define LANG_NLBE	12
#define LANG_NO		13
#define LANG_PT		14
#define LANG_SV		15
#define LANG_ZHTW	16

#define LOCAL_CHINESE	"zh_CN\x00\x00\x00"
#define LOCAL_INTERNA	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"

int getLocale(char* buf);
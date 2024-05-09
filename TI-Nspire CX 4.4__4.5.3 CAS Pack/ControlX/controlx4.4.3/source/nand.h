#define NAND_BLOCK_PAGES	0x40
#define NAND_PAGE_SIZE	0x800
#define NAND_BLOCK_SIZE	(NAND_BLOCK_PAGES*NAND_PAGE_SIZE)

#define MODEL_OFFSET		0x804
#define LOCAL_OFFSET		0x808
#define PART_TABLE_OFFSET	0x818
#define HW_OFFSET			0x81C

#define PART_TABLE_ID		"\x91\x5F\x9E\x4C"
#define PART_TABLE_SIZE		4

#define DIAGS_OFFSET_OFFSET	0x82c
#define BOOT2_OFFSET_OFFSET	0x830
#define BOOTD_OFFSET_OFFSET	0x834
#define FILES_OFFSET_OFFSET	0x838

#define SDRAM_OFFSET		0x840

#define FILES_PAGE_OFFSET	0x800
#define DGSTA_PAGE_OFFSET	0x780

#define DGSTA_PAGE_NUM		(FILES_PAGE_OFFSET-DGSTA_PAGE_OFFSET)

#define VERSION_SIZE 11
#define TYPE_SIZE 10
#define FLAGS_SIZE 8
#define ID_SIZE 3

#define MAX_IMAGES 11

typedef struct {
	unsigned char id;
	char type[TYPE_SIZE];
	char version[VERSION_SIZE];
	long int totalsize;
	long int appendsize;
	int offset;
	int compressed;
	long int base;
	long int size8070;
} image_info;

#define LONGLOCAL_SIZE 8
#define SHORTLOCAL_SIZE 5
char locales[][LONGLOCAL_SIZE+SHORTLOCAL_SIZE]  = {
	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	'n',	'o',	'n',	'e',	0,
	'a',	'r',	0,		0,		0,		0,		0,		0,		'a',	'r',	' ',	' ',	0,
	'd',	'a',	0,		0,		0,		0,		0,		0,		'd',	'a',	' ',	' ',	0,
	'd',	'e',	0,		0,		0,		0,		0,		0,		'd',	'e',	' ',	' ',	0,
	'e',	'n',	0,		0,		0,		0,		0,		0,		'e',	'n',	' ',	' ',	0,
	'e',	'n',	'_',	'G',	'B',	0,		0,		0,		'e',	'n',	'G',	'B',	0,
	'e',	's',	0,		0,		0,		0,		0,		0,		'e',	's',	' ',	' ',	0,
	'f',	'i',	0,		0,		0,		0,		0,		0,		'f',	'i',	' ',	' ',	0,
	'f',	'r',	0,		0,		0,		0,		0,		0,		'f',	'r',	' ',	' ',	0,
	'i',	't',	0,		0,		0,		0,		0,		0,		'i',	't',	' ',	' ',	0,
	'n',	'l',	0,		0,		0,		0,		0,		0,		'n',	'l',	' ',	' ',	0,
	'n',	'l',	'_',	'B',	'E',	0,		0,		0,		'n',	'l',	'B',	'E',	0,
	'n',	'o',	0,		0,		0,		0,		0,		0,		'n',	'o',	' ',	' ',	0,
	'p',	't',	0,		0,		0,		0,		0,		0,		'p',	't',	' ',	' ',	0,
	's',	'v',	0,		0,		0,		0,		0,		0,		's',	'v',	' ',	' ',	0,
	'z',	'h',	'_',	'C',	'N',	0,		0,		0,		'z',	'h',	'C',	'N',	0,
	'z',	'h',	'_',	'T',	'W',	0,		0,		0,		'z',	'h',	'T',	'W',	0,
};
#define NLANG sizeof(locales)/(LONGLOCAL_SIZE+SHORTLOCAL_SIZE)

#define nextLang(id) (((id)<NLANG-1)?(id)+1:0)
#define nextModel(id) (((id)<0x12)?(id)+1:0x0F)
#define dispManufFamily(id) disp("CX\0CM"+(((id)<=0x10)?0:3))
#define dispManufType(id) disp("CAS\0num"+(mod((id),2)?0:4))

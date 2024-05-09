#include <libfdt.h>
#include "boot1.h"
#include "console.h"
#include "imgdump.h"
#include "keys.h"
#include "linuxloader.h"
#include "nand.h"
#include "screen.h"
#include "types.h"
#include "tools.h"

extern uint8_t databuffer[];

char textbuffer[8];

#define I_MANUF		0
#define I_BOOT2		1
#define I_BOOTD		2
#define I_DIAGS		3
#define I_FILES		4
#define I_ENDFL		5
#define I_EXTND		6
#define I_BOOT15	7

#define NPARTS	5

#define SIZE_SIZE 4

#define SAFE_RAM_LOW_PTR	0x10700000
#define SAFE_RAM_HIGH_PTR	0x11E00000

#define NO_OPTION 0xFF

#define putnum(i) outhex((i),ID_SERIAL)

/* SDRAM map

1st bank:
0x10000000 					: OS					[raw:25MiB]
0x10100000 (offset+=1MiB)	: Linux3-Kernel
0x10700000 (offset+=6MiB)	: ControlX low buffer	[25MiB]
0x11200000 (offset+=11MiB)	: Boot1.5/INSTALL			[raw:14MiB/comp:11MiB]
0x11800000 (offset+=6MiB)	: Boot2					[raw:8MiB/comp:17MiB]
0x11F00000 (offset+=8MiB)	: Linux1-DTB			[1MiB]

2nd bank:
0x12000000					: Linux2-Initrd
*/

#define MAX_IMAGES 12

uint32_t offsets_offsets[NPARTS]={0,BOOT2_OFFSET_OFFSET,BOOTD_OFFSET_OFFSET,DIAGS_OFFSET_OFFSET,FILES_OFFSET_OFFSET};
uint32_t pgoffsets[NPARTS+1]={0,0,0,0,0,0};
uint32_t pgsizes[NPARTS]={0,0,0,0,0};

uint32_t extpgoffset = (DGSTA_PAGE_OFFSET+DGSTA_PAGE_NUM);
uint32_t extpgsize = 0;

uint8_t modelid=0;
uint8_t asicflags;
uint8_t bootkeys=0; // 0=prod, 1=dev
uint8_t localid=0;
uint8_t osmajor=0;
uint8_t osminor=0;
uint16_t osbuild=0;
uint8_t sdram=32;
uint8_t hw;


#define SPLASH_HEIGHT	184
#define SPLASH_POSY		10

#define STATUS_SPACER	"|"
#define STATUS_SIZE MAX_COL
#define NAME "CtrlX"
#define VERSION "4.4.3"
char status[STATUS_SIZE+1];

char* flashbuf = SAFE_RAM_LOW_PTR;
uint8_t flashtype=NO_OPTION;

uint32_t bootdlastpageoffset=-1;

uint8_t startstatus;
uint8_t maintenance=0;
uint32_t autooption=0xFFFFFFFF;

void main()
{ // local functions used only in main() - assembler error if too many of them are outside of main()
	void fullsdram() { // Enable access to second 32MiB of SDRAM
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
	}
	void load100000XX(const void* dest, const void* src, uint32_t size) {
		uint8_t* cdest=dest;
		uint8_t* csrc=(uint8_t*)src;
		uint32_t i;
		for(i=0;i<size;i++)
			cdest[i]=csrc[i];
		if(dest==0x10000034)
			asm(
				".arm \n"
				"ldr pc, =0x10000034 @ jump to nboot-image\n"
			);
		else {
			// Killing all interrupts is necessary (exercise for the reader: why ?)
	//		*((uint32_t *)0xDC00000C) = 0xFFFFFFFF;

			// Useful ?
	/*		for(i=0;i<0x40;i++)
				*(uint8_t*)i=cdest[i];
	*/
			// This is also necessary (exercise for the reader: why ?)
			*((uint32_t *)0xC000001C) &= ~((1 << 0) | (1 << 11));

			// Launch the OS.
			asm(
				".arm \n"
				"mov lr, pc\n"
				"ldr pc, =0x10000000 @ jump to os\n"
			);
		}
	}
	uint8_t selectImage(image_info* data_info, uint8_t nimage, uint32_t tsize) {
		uint8_t option=NO_OPTION;
		dispStatus("",0);
		dispDataInfo(data_info, nimage, tsize);
		whileKeyPressed();
		while(option==NO_OPTION || option!=NO_OPTION && option>nimage)
			option=getMenuKey();
		return option;
	}
	uint32_t validateAuto(uint32_t auto0) {
		uint32_t autotemp=auto0;
		uint8_t i, opt;
		for(i=0;i<8;i++) {
			opt=autotemp&0xF;
			autotemp=autotemp>>4;
			if(	 opt!=0xF &&
				(	(i<6)&&(opt<1 || opt>=2 && opt<=4 || opt>9) ||
					(i==6)&&(opt<2 || opt>4) ||
					(i==7)&&(!opt || opt>MAX_IMAGES)))
						return 0xFFFFFFFF;
		}
		return auto0;
	}
	uint8_t getFlashBootD(uint8_t pmin, uint8_t pmax) {
		uint32_t size=pgsizes[I_BOOTD]*NAND_PAGE_SIZE;
		if(flashtype!=I_BOOTD) {
			bootdlastpageoffset=size;
			flashtype=I_BOOTD;
			fullStatus("Processing BootD partition...");
			dispProgress();
			read_nand(flashbuf,pgsizes[I_BOOTD]*NAND_PAGE_SIZE,pgoffsets[I_BOOTD]*NAND_PAGE_SIZE,pmin,pmax,progress_callback);
			int32_t offset=size-NAND_PAGE_SIZE;
			while(offset>=0) { // proceed page by page
				if(*(uint32_t*)(flashbuf+offset)==0x928CC6AA)
					break;
				offset-=NAND_PAGE_SIZE;
			}
			if(offset>=0)
				bootdlastpageoffset=offset;
		}
		if(bootdlastpageoffset>=size)
			fullStatus("Empty or invalid BootD !");
		return bootdlastpageoffset<size;
	}
	uint8_t countFlashImages(image_info* flashimages, uint32_t* flashdatasize) {
		uint8_t nflash=0;
		if(flashdatasize) *flashdatasize=0;
		while(nflash<MAX_IMAGES && flashimages[nflash].totalsize) {
			nflash++;
			if(flashdatasize) (*flashdatasize)+=flashimages[nflash].totalsize;
		}
		return nflash;
	}
	uint8_t searchFlashImage(image_info* flashimages,char* type, char* version) {
		uint8_t nflash=0,i,j=0;
		while(j<3) {
			switch(j) {
				case 0:
					getFlashPart(I_BOOT2,flashimages,maintenance?50:66,100);
					break;
				case 1:
					getFlashPart(I_EXTND,flashimages,maintenance?50:66,100);
					break;
				default:
					getFlashPart(I_DIAGS,flashimages,maintenance?50:66,100);
					break;
			}
			nflash=countFlashImages(flashimages,NULL);
			for(i=0;i<nflash;i++)
				if((!type || !memcmp(flashimages[i].type,type,strlen(type))) && (!version || !memcmp(flashimages[i].version,version,strlen(version))))
					return i;
			j++;
		}
		return -1;
	}
	uint8_t getLocalID(uint8_t* local) {
		uint8_t id=NLANG-1;
		while(id>0 && memcmp(&(locales[id]),local,LONGLOCAL_SIZE))
			id--;
		return id;
	}
	void safeWriteNand(uint8_t* ptr, uint32_t size, uint32_t offset, uint8_t part) {
		if(	offset>=pgoffsets[part]*NAND_PAGE_SIZE && offset+size<=(pgoffsets[part]+pgsizes[part])*NAND_PAGE_SIZE) { 	// in partition bounds
			nand_erase_range(offset,offset+size-1);
			write_nand(ptr,size,offset);
		}
	}

	int _fdt_make_node(void *fdt, int parentoffset, const char *name) {
		int e = fdt_subnode_offset(fdt, parentoffset, name);
		return (e==FDT_ERR_NOTFOUND)?fdt_add_subnode(fdt, parentoffset, name):e;
	}

	void ut_disable_watchdog(void) {
		// Disable the watchdog on CX that may trigger a reset
		*(volatile unsigned*)0x90060C00 = 0x1ACCE551; // enable write access to all other watchdog registers
		*(volatile unsigned*)0x90060008 = 0; // disable reset, counter and interrupt
		*(volatile unsigned*)0x90060C00 = 0; // disable write access to all other watchdog registers
	}

	void writeBootD() {
		uint32_t offsetpage=bootdlastpageoffset;
		uint32_t offset=pgoffsets[I_BOOTD]*NAND_PAGE_SIZE;
		uint8_t* ptr = flashbuf;
		while(offsetpage>=NAND_BLOCK_SIZE) {
			offsetpage-=NAND_BLOCK_SIZE;
			offset+=NAND_BLOCK_SIZE;
			ptr+=NAND_BLOCK_SIZE;
		}
		safeWriteNand(ptr,NAND_BLOCK_SIZE,offset,I_BOOTD);
	}

	void writeBootDdefault(uint32_t def) {
		if(def)
			def=validateAuto(def);
		if(autooption!=def) {
			if(getFlashBootD(0,200)) {
				*(uint32_t*)(flashbuf+bootdlastpageoffset+0x10)=def;
				autooption=def;
				writeBootD();
			}
		}
	}
	void dispVer(uint8_t osmajor, uint8_t osminor, uint16_t osbuild) {
		itoa(osmajor,textbuffer,10);
		disp(textbuffer);
		disp(".");
		if(osminor<10)
			disp("0");
		itoa(osminor,textbuffer,10);
		disp(textbuffer);
		disp(".");
		itoa(osbuild,textbuffer,10);
		disp(textbuffer);
	}
	void dispBack() {
		clearLines(SCREEN_BASE_ADDR,SPLASH_POSY+SPLASH_HEIGHT,SCREEN_HEIGHT-1);
		memcpy(SCREEN_BASE_ADDR+SCREEN_WIDTH*SPLASH_POSY*2,databuffer,SCREEN_WIDTH*SPLASH_HEIGHT*2);
		setLine(MAX_LGN-2);
		setCol(0);
		setCurColorRGB(255,0,255);
		disp("reset");
		setCurColorRGB(0,0,0);
		disp("+[");
		setCurColorRGB(255,0,0);
		disp("ctrl");
		setCurColorRGB(0,0,0);
		displn("]=menu ");
		disp(NAME);
		setCurColorRGB(0,127,0);
		disp(VERSION);
		if(modelid) {
			setCol(14);
			setCurColorRGB(0,0,0);
			disp(bootkeys?"TI-XXXXXX":"TI-Nspire");
			setCurColorRGB(255,0,255);
			dispManufFamily(modelid);
			dispManufType(modelid);
			setCurColorRGB(0,0,0);
			disp(".");
			setCurColorRGB(0,0,255);
			disp(locales[localid]+LONGLOCAL_SIZE);
			setCurColorRGB(255,0,255);
			disp((hw==0x85)?"CR":"Napoca");
			setCurColorRGB(0,0,255);
			disp((sdram==64)?"64":"32");
			setCurColorRGB(0,0,0);
			disp("M ");
			setCurColorRGB(0,127,0);
			dispVer(osmajor,osminor,osbuild);
			setCurColorRGB(0,0,0);
			disp("+");
		}
	}
	int i,j;
	uint8_t uninstalled=0;
	ut_disable_watchdog();
	uint8_t menu=(is_key_pressed(KEY_CTRL)) || (is_key_pressed(KEY_ESC) && is_key_pressed(KEY_MENU) && is_key_pressed(KEY_MINUS));
	if(is_key_pressed(KEY_DOC) && is_key_pressed(KEY_ENTER) && is_key_pressed(KEY_EE))
		maintenance=1;
	startstatus=1;
	memset(status,' ',STATUS_SIZE);
	status[STATUS_SIZE]=0;

	asicflags = ((*(volatile unsigned int*) 0x900A002C)>>26)&0b11111;
	
	init_screen();
	clearLines(SCREEN_BASE_ADDR,0,SPLASH_POSY-1);
	dispBack();	
	
	// get NAND partition table
	getFlashPart(I_MANUF,NULL,0,menu?100:maintenance?50:33);
	char local[LONGLOCAL_SIZE];
	memcpy(local,(uint8_t*)(SAFE_RAM_LOW_PTR+LOCAL_OFFSET),LONGLOCAL_SIZE);
	localid=getLocalID(local);
	modelid=*(uint8_t*)(SAFE_RAM_LOW_PTR+MODEL_OFFSET);
	bootkeys = 0;
	if(0xFC018012==*(uint32_t*)(SAFE_RAM_LOW_PTR+SDRAM_OFFSET))
		sdram=64;
	hw=*(uint8_t*)(SAFE_RAM_LOW_PTR+HW_OFFSET);
	pgoffsets[I_MANUF]=0;
	for(i=1;i<NPARTS;i++)
		pgoffsets[i]=(*(uint32_t*)(SAFE_RAM_LOW_PTR+offsets_offsets[i]))/NAND_PAGE_SIZE;
	pgoffsets[I_ENDFL]=0x10000;
	for(i=0;i<NPARTS;i++)
		pgsizes[i]=pgoffsets[i+1]-pgoffsets[i];
	i=0;
	while(pgoffsets[i+1]<DGSTA_PAGE_OFFSET)
		i++;
	pgsizes[i]=DGSTA_PAGE_OFFSET-pgoffsets[i];
	while(pgoffsets[i]<extpgoffset)
		i++;
	extpgsize=pgoffsets[i]-extpgoffset;

	if(getFlashBootD(33,66)) {
		if(!maintenance)
			autooption=*(uint32_t*)(flashbuf+bootdlastpageoffset+0x10);
		uint8_t* osminptr=flashbuf+bootdlastpageoffset+4;
		osmajor=*(uint8_t*)(osminptr+3);
		osminor=*(uint8_t*)(osminptr+2);
		osbuild=*(uint16_t*)(osminptr);
	}
	
	uint8_t option=NO_OPTION,confirm=0;

	if(menu) {
		dispBack();
		dispStatus("",0);
	}
	uint32_t tautooption=validateAuto(autooption);
	while(option) {
		uint8_t option2=0, iflash=NO_OPTION;
		dispBack();
		option=NO_OPTION;
		if(!menu) {
			option=tautooption&0xF;
			if(option!=0xF)
				tautooption=(tautooption&0xFF000000)|0x00F00000|((tautooption&0x00FFFFF0)>>4);
			else {
				option=(tautooption&0x0F000000)>>24;
				tautooption=(tautooption&0xF0FFFFFF)|0x0F000000;
				if(option==0xF)
					option=NO_OPTION;
			}
		}
		else {
			confirm=0;
			while(option==NO_OPTION || !confirm) {
				setLine(MAX_LGN-6);
				setCol(0);
				setCurColorRGB(0,0,0);	disp("[");			setCurColorRGB(255,0,0);	disp("shift");	setCurColorRGB(0,0,0);	disp("]+["); setCurColorRGB(255,0,0);	disp("key");	setCurColorRGB(0,0,0);	disp("]="); setCurColorRGB(0,127,0); displn("help    ");
				setCurColorRGB(0,0,0);	disp(" [");		setCurColorRGB(255,0,0);	disp("ctrl");	setCurColorRGB(0,0,0);	disp("]+["); setCurColorRGB(255,0,0);	disp("key");	setCurColorRGB(0,0,0);	disp("]="); setCurColorRGB(255,0,255); disp("default ");	setCurColorRGB(0,0,0);	disp("{");
				setCurColorRGB(255,0,0);
				if(autooption==0xFFFFFFFF)
					disp("best Boot2");
				else if(!autooption)
					disp("Boot2");
				else {
					uint32_t autotemp=((autooption&0x00FFFFFF)<<8)|((autooption&0xFF000000)>>24);
					uint8_t opt;
					for(uint8_t i=0;i<8;i++) {
						if(i==6)
							disp(" ");
						opt=(autotemp&0xF0000000)>>28;
						autotemp=autotemp<<4;
						if(opt!=0xF)
							dispMenu(opt);
					}
				}
				setCurColorRGB(0,0,0);	disp("}\n(");	setCurColorRGB(255,0,0);	disp("1");	setCurColorRGB(0,0,0);	disp(")");	setCurColorRGB(0,0,255);	disp  ("Boot1  ");
				setCurColorRGB(0,0,0);	disp("<");		setCurColorRGB(255,0,0);	disp("2");	setCurColorRGB(0,0,0);	disp(">");	setCurColorRGB(0,0,255);	disp  ("Boot2  ");
				setCurColorRGB(0,0,0);	disp("<"); 		setCurColorRGB(255,0,0);	disp("3");	setCurColorRGB(0,0,0);	disp(">");	setCurColorRGB(0,0,255);	disp  ("Diags  ");
				setCurColorRGB(0,0,0);	disp("<"); 		setCurColorRGB(255,0,0);	disp("4");	setCurColorRGB(0,0,0);	disp(">");	setCurColorRGB(0,0,255);	disp  ("ExtnD   ");
				setCurColorRGB(0,0,0);	disp("(");		setCurColorRGB(255,0,0);	disp("5");	setCurColorRGB(0,0,0);	disp(")");	setCurColorRGB(0,0,255);	displn("min=0.0.0");
				setCurColorRGB(0,0,0);	disp(" ");		setCurColorRGB(255,0,0);	disp(" ");	setCurColorRGB(0,0,0);	disp(" ");	setCurColorRGB(0,0,255); 	disp  ("       ");
				setCurColorRGB(0,0,0);	disp("(");		setCurColorRGB(255,0,0);	disp("7");	setCurColorRGB(0,0,0);	disp(")");	setCurColorRGB(0,0,255); 	disp  ("fam=");
				dispManufFamily((modelid>=0x11)?modelid-2:modelid+2);
				setCurColorRGB(0,0,0);	disp(" (");		setCurColorRGB(255,0,0);	disp("8");	setCurColorRGB(0,0,0);	disp(")");	setCurColorRGB(0,0,255); 	disp  ("typ=");
				dispManufType(mod(modelid,2)?modelid+1:modelid-1);
				setCurColorRGB(0,0,0);	disp("(");		setCurColorRGB(255,0,0);	disp("9");	setCurColorRGB(0,0,0);	disp(")");	setCurColorRGB(0,0,255);	disp  ("keys=");
				if(bootkeys)
					disp("REL");
				else
					disp("DEV");
				setCurColorRGB(0,0,0);	disp("[");		setCurColorRGB(255,0,0);	disp("L");	setCurColorRGB(0,0,0);	disp("]");	setCurColorRGB(0,0,255);	disp("zone=");
				displn(locales[nextLang(localid)]+LONGLOCAL_SIZE);
				setCurColorRGB(0,0,0);	disp("[");		setCurColorRGB(255,0,0);	disp("0");	setCurColorRGB(0,0,0);	disp("]");	setCurColorRGB(0,0,255);	disp  ("Reset  ");
				setCurColorRGB(0,0,0);	disp("["); 		setCurColorRGB(255,0,0);	disp("X");	setCurColorRGB(0,0,0);	disp("]");	setCurColorRGB(0,0,255);	disp  ("boot=");
				if(autooption)	disp("Boot2");
				else			disp("Diags");
				setCurColorRGB(0,0,0);	disp("       [");		setCurColorRGB(255,0,0);	disp("Z");	setCurColorRGB(0,0,0);	disp("]");	setCurColorRGB(0,0,255);	displn("Uninstall");
				setCurColorRGB(0,0,0);
				while(getMenuKey()!=NO_OPTION);
				while((option=getMenuKey())==NO_OPTION);
				if(is_key_pressed(KEY_SHIFT)) {
					if(option==0)		fullStatus("[0]Reset");
					else if(option==1)	fullStatus("[1]Resume Boot1 with Boot2 partition boot");
					else if(option==2)	fullStatus("[2]Run image in Boot2 partition");
					else if(option==3)	fullStatus("[3]Run image in Diags partition");
					else if(option==4)	fullStatus("[4]Run image in ExtnD partition");
					else if(option==5)	fullStatus("[5]Fix minOS limit in BootD partition");
					else if(option==7)	fullStatus("[7]Switch model family ( CX / CM )");
					else if(option==8)	fullStatus("[8]Switch model type ( CAS / numeric )");
					else if(option==9)	fullStatus("[9]Switch boot RSA keys ( release / development )");
					else if(option==21)	fullStatus("[L]Switch model zone (default lang + some features)");
					else if(option==33)	fullStatus("[X]En/dis-able ControlX ( [esc][menu][-] to bypass )");
					else if(option==35)	fullStatus("[Z]Uninstall nBoot");
				}
				else if(is_key_pressed(KEY_CTRL)) {
					if(option<0xF) {
						writeBootDdefault((autooption&0xFF000000)|((autooption<<4)&0x00FFFFF0)|option);
						dispBack();
					}
				}
				else
					confirm=1;
			}
		}
		uint32_t size;
		switch(option) {
			case 7: // switch model family
				modelid=(modelid<=0x10)?modelid+2:modelid-2;
				dispStatus((modelid<=0x10)?"CX":"CM",1);
				break;
			case 8: // switch model type
				modelid=mod(modelid,2)?modelid+1:modelid-1;
				dispStatus(mod(modelid,2)?"CAS":"num",1);
				break;
			case 9: // switch boot keys
				bootkeys=!bootkeys;
				dispStatus(bootkeys?"DEV":"REL",1);
				break;
			case 21:// switch lang
			case 35:// uninstall nBoot
				getFlashPart(I_MANUF,NULL,0,200);
				if(option==21) {
					localid=nextLang(localid);
					memcpy((uint8_t*)(SAFE_RAM_LOW_PTR+LOCAL_OFFSET),&(locales[localid]),LONGLOCAL_SIZE);
				}
				else if(option==35) {
					fullStatus("Uninstalling nBoot ...");
					uint8_t unsafe=0,spicount=0,complete=0;
					for(i=0x848;i<0x888 && !unsafe;i+=4) {
						if(!complete && (!flashbuf[i] || flashbuf[i+1] || flashbuf[i+2] || flashbuf[i+3]))
							complete=1;
						if(complete && (memcmp(flashbuf+i,"\xFF\xFF\xFF\xFF",4) || memcmp(flashbuf+i+4,"\xFF\xFF\xFF\xFF",4)))
							unsafe=1;
						i+=4;
						if(!complete && (!flashbuf[i] || flashbuf[i+1] || flashbuf[i+2] || flashbuf[i+3]))
							unsafe=1;
						if(!complete && !unsafe)
							spicount++;
					}
					if(flashbuf[0x844]>spicount) {
						flashbuf[0x844]=spicount;
						itoa(spicount,textbuffer,10);
						dispStatus("SPIcount=",1);
						dispStatus(textbuffer,0);
						uninstalled=1;
					}
				}
				nand_erase_range(0,NAND_BLOCK_SIZE-1);
				write_nand(flashbuf,NAND_BLOCK_SIZE,0);
				break;
			case 6:
				nand_erase_range(pgoffsets[I_BOOTD]*NAND_PAGE_SIZE,(pgoffsets[I_BOOTD]+pgsizes[I_BOOTD])*NAND_PAGE_SIZE-1);
				if(flashtype==I_BOOTD)
					flashtype=NO_OPTION;
				bootdlastpageoffset=pgsizes[I_BOOTD]*NAND_PAGE_SIZE;
				osmajor=osminor=osbuild=0;
				break;
			case 5:// kill minOS 
			case 33:// 1st boot = Boot2/Diags
				if(getFlashBootD(0,200)) {
					if(option==5) {
						uint8_t tmp[4];
						uint32_t* osminptr=flashbuf+bootdlastpageoffset+4;
						if(*osminptr) {
							memcpy(tmp,osminptr,4);
							*osminptr=0; // kill minOS
							writeBootD();
							dispStatus("min=0.0.0",1);
							osmajor=osminor=osbuild=0;
						}
					}
					else {
						if(autooption) {
							writeBootDdefault(0); // 1st boot = Boot2
							dispStatus("Boot=Boot2",1);
						}
						else {
							writeBootDdefault(0xFFFFFFFF); // 1st boot = Diags
							dispStatus("Boot=Diags",1);
						}
					}
				}
		}
		image_info flashimages[MAX_IMAGES];
		uint8_t nflash=0;
		uint32_t flashdatasize=0;

		// run image in Boot2/Diags partition			
		if(!menu && option==NO_OPTION) {
			if(!maintenance)
				iflash=searchFlashImage(flashimages,"BOOT2","4.3.49");						// Boot2 version 4.0.3.49 supports OS patcher
			if(iflash==NO_OPTION) iflash=searchFlashImage(flashimages,"BOOT2",NULL);		// any other Boot2
		}
		if(option==2 || option==3 || option==4) {
			if(option==2)		getFlashPart(I_BOOT2,(image_info*)flashimages,menu?0:66,menu?200:100);
			else if(option==3)	getFlashPart(I_DIAGS,(image_info*)flashimages,menu?0:66,menu?200:100);
			else				getFlashPart(I_EXTND,(image_info*)flashimages,menu?0:66,menu?200:100);
			nflash=countFlashImages(flashimages,&flashdatasize);
			if(!menu) {
				if(((tautooption&0xFF000000)>>24)!=NO_OPTION) {
					option2=(tautooption&0xF0000000)>>28;
					iflash=(option2>MAX_IMAGES)?NO_OPTION:option2-1;
					tautooption=(tautooption&0xFFFF)|0xFFFF0000;
				}
			}
			else {
				dispBack();
				setCol(0);
				setLine(MAX_LGN-5);	
				disp("Partition: ");
				setCurColorRGB(0,0,255);
				if(option==2)		disp("Boot2");
				else if(option==3)	disp("Diags");
				else				disp("ExtnD");
				setCurColorRGB(255,0,0);
				disp("  0");
				setCurColorRGB(0,0,0);
				displn(")Back");
				option2=selectImage(flashimages,nflash,flashdatasize);
				if(is_key_pressed(KEY_CTRL)) {
					if(option2!=NO_OPTION)
						writeBootDdefault(((autooption&0x00FFFFFF)|(((uint32_t)option)<<24)|(((uint32_t)option2)<<28)));
				}
				else
					iflash=(option2==NO_OPTION)?NO_OPTION:option2-1;
			}
		}
		// run image in Boot2/Diags partition
		if(iflash!=NO_OPTION) {
			if(flashimages[iflash].totalsize) { // Boot2 image found
				uint8_t type=0;
				if(!memcmp(flashimages[iflash].type,"BOOT2",5))
					type=I_BOOT2;
				else if(!memcmp(flashimages[iflash].type,"DIAGS",5))
					type=I_DIAGS;
				else if(!memcmp(flashimages[iflash].type,"BOOT1.5",7))
					type=I_BOOT15;
				if(type!=I_DIAGS) { // DIAGS doesn't like overclocking
					*(volatile uint32_t*)0x900B0000 = 0x0030A002; // safe overclock to base=198MHz cpu=198MHz ahb=66MHz
					*(volatile uint32_t*)0x900B000C = 0x00000004;
					dispStatus("CPU=198",1);
				}
				
				uint32_t base = flashimages[iflash].base;
				dispStatus((flashimages[iflash].id<=0x10)?"CX":"CM",1);
				dispStatus(flashimages[iflash].type,0);
				dispStatus(" ",0);
				dispStatus(flashimages[iflash].version,0);
				
				unsigned char* buf=SAFE_RAM_LOW_PTR+flashimages[iflash].offset;
				char* data8000=0;
				char* field8000=0;
				int size8000=0;
				int size8070=0;
				size8000=getFieldData(buf,0x8000,0,flashimages[iflash].totalsize,&data8000,&field8000);
				if(field8000!=buf)
					size8000=0;
				if(size8000)
					size8070=getFieldDataSize(data8000, 0x8070, 0, size8000);
				if(flashimages[iflash].appendsize) { // Linux image
					fullsdram();
					memmove(SAFE_RAM_LOW_PTR,SAFE_RAM_LOW_PTR+flashimages[iflash].offset+flashimages[iflash].totalsize-flashimages[iflash].appendsize-sizeof(LLHeader),flashimages[iflash].appendsize+sizeof(LLHeader));
					LLHeader *header = (LLHeader*)(SAFE_RAM_LOW_PTR);
					if(header->signature == LLHeader_sig) {
						// Load fdt first, use kernel region for temporary storage
						void *fdt = (uint32_t)(header)+sizeof(LLHeader);						
						if(fdt_open_into(fdt, dtb_dest, 0x10000) < 0)
							fullStatus("FDT invalid!");
						else {
							int chosen = _fdt_make_node(dtb_dest, 0, "chosen");
							if(chosen < 0)
								fullStatus("FDT fail");
							else {
								if(fdt_setprop_cell(dtb_dest, chosen, "linux,initrd-start", (uint32_t) initrd_dest) < 0
									|| fdt_setprop_cell(dtb_dest, chosen, "linux,initrd-end", (uint32_t)(initrd_dest) + header->initrd_size) < 0)
									fullStatus("FDT fail 2");
								else {
									fdt_pack(dtb_dest);
									// Load initrd
									memcpy(initrd_dest, fdt+header->dtb_size, header->initrd_size);
									// Load kernel
									memcpy(kernel_dest, fdt+header->dtb_size+header->initrd_size, header->kernel_size);
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
									// should never reach this	
								}
							}
						}
					}
				}
				else {
					uint8_t* data_in_ptr=SAFE_RAM_LOW_PTR+flashimages[iflash].offset;
					uint8_t* data_out_ptr=base;
					if(base<0x11200000) {
						data_out_ptr=SAFE_RAM_HIGH_PTR;
						fullsdram();
					}
					if(flashimages[iflash].compressed) {
						if(base>=0x11200000) {
							memmove(SAFE_RAM_LOW_PTR,data_in_ptr,flashimages[iflash].totalsize);
							data_in_ptr=SAFE_RAM_LOW_PTR;
						}
						else {
							memmove(SAFE_RAM_LOW_PTR,data_in_ptr,flashimages[iflash].totalsize);
							data_in_ptr=SAFE_RAM_LOW_PTR;
							data_out_ptr=SAFE_RAM_HIGH_PTR;
						}
					}
puts("Decompressing image...\n");
					uint32_t code_size=decompressFiles(data_in_ptr,flashimages[iflash].totalsize,data_out_ptr);
puts("Image decompressed !\n");
					if(type==I_BOOT2)
						patch_Boot2();
					if(type==I_BOOT15)
						dispProgress();
					if(base==0x11200000 || base==0x11800000) {
						// put your patches here
						if(ndless_loader(flashimages + iflash))
							dispStatus("OS patcher",1);
						if(base==0x11200000)
						{	asm(
								".arm \n"
								"ldr pc, =0x11200000 @ jump to boot1.5\n"
							);
						}
						else
						{	asm(
								".arm \n"
								"ldr pc, =0x11800000 @ jump to boot2\n"
							);
						}
						// should never reach this	
					}
					else if(base==0x10000000 || base==0x10000034) {
						base=!(flashimages[iflash].size8070)?0x10000034:base;
						if(base==0x10000000)
							patch_OS(data_out_ptr);
						uint32_t load100000XX_size = 0x100000;
						void (*load100000XX_copy_ptr)(const void*,const void*, uint32_t);
						load100000XX_copy_ptr=0x13F00000;
						memcpy(load100000XX_copy_ptr,&load100000XX,load100000XX_size);
						(*load100000XX_copy_ptr)(base,data_out_ptr,code_size);
					}
				}
				dispStatus(" unsupported",0);
			}
		}
		if(option==1) {
			dispStatus("Boot Boot2 via Boot1",1);
			//dispProgress(); // crashes Boot1 on the next line (Kernel error)
			load_start_boot2();
			// should never reach this	
		}
	}
	reset();
	// should never reach this	
	return 0;
}

int is_key_pressed(key_t key) {
	return is_any_key_pressed()?is_rc_pressed(key.row,key.col):0;
}

void fullStatus(char* msg) {
	setLine(0);
	setCol(0);
	clearLines(SCREEN_BASE_ADDR,0,CHAR_HEIGHT-1);
	setCurColorRGB(0,127,0);
	disp(msg);
	puts(msg);
	puts("\n");
	setCurColorRGB(0,0,0);
}

void truncSpace(char* str, uint8_t size) {
	while(size && *str!=' ') {
		size--;
		str++;
	}
	*str=0;
}
	
void getBufDataInfo(char* data_buf, uint32_t size, image_info* data_info, uint8_t lim)
{	uint32_t i, rsize=0;
	if(!lim) lim=MAX_IMAGES;
	memset(data_info,0,lim*sizeof(image_info));
	for(i=0;i<lim;i++)
	{
//		memset(data_info->version,0,VERSION_SIZE);
		char* data8000=0;
		char* field8000=0;
		uint32_t size8000=0;
		if(*data_buf)
			size8000=getFieldData(data_buf,0x8000,0,size-rsize,&data8000,&field8000);
		if(field8000!=data_buf)
			size8000=0;
//		data_info->size8070=0;
		if(size8000) {
			data_info->size8070=getFieldDataSize(data8000,0x8070,0,size8000);
			char data_id[ID_SIZE];
			memset(data_id,0,ID_SIZE);
			copyFieldData(data8000, 0x8010, 1, size8000, data_id, ID_SIZE);
			data_info->id=chex2int(data_id[1])*16+chex2int(data_id[2]);
			copyFieldData(data8000, 0x8020, 0, size8000, data_info->version, VERSION_SIZE);
			truncSpace(data_info->version,VERSION_SIZE);
//			memset(data_info->type,0,TYPE_SIZE);
			copyFieldData(data8000, 0x8040, 0, size8000, data_info->type, TYPE_SIZE);
			truncSpace(data_info->type,TYPE_SIZE);
			char data_flags[FLAGS_SIZE];
			memset(data_flags,0,FLAGS_SIZE);
			copyFieldData(data8000, 0x8080, 0, size8000, data_flags, FLAGS_SIZE);
			data_info->compressed = data_flags[FLAGS_SIZE-1];
			data_info->base=(((long int)data_flags[0])<<24)+(((long int)data_flags[1])<<16)+(((int)data_flags[2])<<8)+data_flags[3];
//			data_info->totalsize=0;
			data_info->totalsize=getTotalFieldsSize(data_buf,size,&(data_info->appendsize));
			rsize+=data_info->totalsize;
			data_buf+=data_info->totalsize;
		}
		if(!(data_info->totalsize) || rsize>=size)
			break;
		else {
			data_info++;
			if(i<lim-1)
				data_info->offset=rsize;
		}
	}
}

void dispProgress() {
	uint16_t i;
	for(i=100;i<=139;i++) {
		if(i>=102 && i<=137)
			drawFastHoriz(SCREEN_BASE_ADDR,i,22,297,0xFF);
		else
			drawFastHoriz(SCREEN_BASE_ADDR,i,20,299,0);
	}
	for(i=20;i<=299;i++) {
		if(i==22) i=298;
		drwBufVert(SCREEN_BASE_ADDR,i,100,139);
	}
}
	
void getFlashDataInfo(char* data_buf, int nand_offset, int size, image_info* data_info, uint8_t pmin, uint8_t pmax) {
	memset(data_buf,0,size);
	dispProgress();
	read_nand(data_buf,size,nand_offset,pmin,pmax,progress_callback);
	memset(data_info,0,sizeof(image_info)*MAX_IMAGES);
	getBufDataInfo(data_buf, size, data_info,0);
}

void getFlashPart(uint8_t ipart, image_info* imginfos, uint8_t pmin, uint8_t pmax) {
	if(flashtype!=ipart) {
		flashtype=ipart;
		fullStatus("Scanning partition...");
		if(ipart==I_MANUF) {
			dispProgress();
			read_nand(flashbuf,NAND_BLOCK_SIZE,pgoffsets[ipart]*NAND_PAGE_SIZE,pmin,pmax,progress_callback);
		}
		else if(ipart==I_EXTND)
			getFlashDataInfo(flashbuf,extpgoffset*NAND_PAGE_SIZE,extpgsize*NAND_PAGE_SIZE,imginfos,pmin,pmax);
		else
			getFlashDataInfo(flashbuf,pgoffsets[ipart]*NAND_PAGE_SIZE,pgsizes[ipart]*NAND_PAGE_SIZE,imginfos,pmin,pmax);
	}
}

void dispStatus(char* msg, int spacestatus) {
	uint32_t l = strlen(msg);
	uint32_t i;
	if(!startstatus && spacestatus)
		dispStatus(STATUS_SPACER,0);
	if(l)
		startstatus=0;
	setLine(0);
	setCurColorRGB(0,127,0);
	for(i=0;i<l;i++) {
		setCol(0);
		disp(status);
		memcpy(status,status+1,STATUS_SIZE-1);
		status[STATUS_SIZE-1]=msg[i];
	}
	setCol(0);
	disp(status);
	puts(msg);
	puts("\n");
	setCurColorRGB(0,0,0);
}

int mod(int a, int b) {
	while(a>=b)
		a-=b;
	return a;
}

void dispDataInfo(image_info* data_info, uint8_t max, uint32_t tsize)
{	uint32_t i, ccol = 0, last=1;
	max=(max>MAX_IMAGES)?MAX_IMAGES:max;
	setCurColorRGB(0,0,0);
	clearLines(SCREEN_BASE_ADDR,(MAX_LGN-3)*CHAR_HEIGHT,SCREEN_HEIGHT-1);
	for(i=0;i<max;i++)
	{
		if(!(data_info->totalsize))
			break;
		if(i && !(mod(i,4)))
			ccol+=MAX_COL/3+1;
		setLine(MAX_LGN-4+(mod(i,4)));
		setCol(ccol);
		setCurColorRGB(255,0,0);
		dispMenu(i+1);
		setCurColorRGB(0,0,0);
		disp(")");
		setCurColorRGB(0,127,0);
		switch(data_info->id) {
			case 0x0F:
			case 0x10:
				disp("CX");
				break;
			case 0x11:
			case 0x12:
				disp("CM");
				break;
			default:
				disp("  ");
		}
		setCurColorRGB(0,0,255);
		disp(data_info->type);
		setCol(getCol()-strlen(data_info->type)+(TYPE_SIZE-4));
		setCurColorRGB(255,0,255);
		disp(data_info->version);
		tsize-=data_info->totalsize;
		data_info++;
		setCurColorRGB(0,0,0);
	}
}

#include "ndless_loader.c"
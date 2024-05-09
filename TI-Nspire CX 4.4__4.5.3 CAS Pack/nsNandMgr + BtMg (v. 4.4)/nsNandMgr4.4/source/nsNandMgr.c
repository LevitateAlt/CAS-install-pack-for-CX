// Requires Ndless 3.1 or compatible
// Caution: This program has system routines capable of bricking the calculator, if used for other purposes.
// An inexpensive RS232-USB module might help to recover from some such incidents.

#include <os.h>
#include "charmaps.h"
#include "console.h"
#include "screen.h"
#include "imgdump.h"
#include "nand.h"
#include "tools.h"

#define NPARTS 5
#define VERSION_SIZE 11
#define TYPE_SIZE 10
#define FLAGS_SIZE 8
#define ID_SIZE 3
#define SIZE_SIZE 4
#define MAX_IMAGES	12
#define EDISP_IMAGES	9
#define DISP_IMAGES	3

#define HW_NAPOCA	1
#define HW_CXCR		2
#define HW_CXCR4	3
#define HW_NONE		0

#define IMGINFO_WIDTH	((MAX_COL-3-2*PG_V_WIDTH)/2)
#define PG_H_WIDTH	6
#define PG_V_WIDTH	3

#define PATH_SIZE 128

#ifdef MULTI
char imgpath[PATH_SIZE];
#endif
char* manufpath[PATH_SIZE];
char* bootdpath[PATH_SIZE];

typedef struct {
	char type[TYPE_SIZE];
	char version[VERSION_SIZE];
	long int size;
	int offset;
	int compressed;
	int id;
	int id2;
	int hw;
	long int base;
} image_info;

long int offsets_offsets[NPARTS]={0,BOOT2_OFFSET_OFFSET,BOOTD_OFFSET_OFFSET,DIAGS_OFFSET_OFFSET,FILES_OFFSET_OFFSET};
long int pgoffsets[NPARTS+1]={MANUF_PAGE_OFFSET,0,0,0,0,NDEND_PAGE_OFFSET};
long int pgsizes[NPARTS]={0,0,0,0,0};

long int extpgoffset = 0;
long int extpgsize = 0;

extern char locales[NLANG+1][LOCAL_SIZE];
extern char dlocales[NLANG+1][9];

unsigned int modeltype;
unsigned int manufflashid;
int asichw;

int manuf_nand_offset() { return pgoffsets[0]*NAND_PAGE_SIZE; }
int boot2_nand_offset() { return pgoffsets[1]*NAND_PAGE_SIZE; }
int bootd_nand_offset() { return pgoffsets[2]*NAND_PAGE_SIZE; }
int diags_nand_offset() { return pgoffsets[3]*NAND_PAGE_SIZE; }
int files_nand_offset() { return pgoffsets[4]*NAND_PAGE_SIZE; }
int ndend_nand_offset() { return pgoffsets[5]*NAND_PAGE_SIZE; }
int ext_nand_offset() { return extpgoffset*NAND_PAGE_SIZE; }

int manuf_pages_num() { return pgsizes[0]; }
int boot2_pages_num() { return pgsizes[1]; }
int bootd_pages_num() { return pgsizes[2]; }
int diags_pages_num() { return pgsizes[3]; }
int files_pages_num() { return pgsizes[4]; }
int ext_pages_num() { return extpgsize; }

int manuf_size() { return manuf_pages_num()*NAND_PAGE_SIZE; }
int boot2_size() { return boot2_pages_num()*NAND_PAGE_SIZE; }
int bootd_size() { return bootd_pages_num()*NAND_PAGE_SIZE; }
int diags_size() { return diags_pages_num()*NAND_PAGE_SIZE; }
int files_size() { return files_pages_num()*NAND_PAGE_SIZE; }
int ext_size() { return ext_pages_num()*NAND_PAGE_SIZE; }

int manuf_nand_endoffset() { return manuf_nand_offset()+manuf_size()-1; }
int boot2_nand_endoffset() { return boot2_nand_offset()+boot2_size()-1; }
int bootd_nand_endoffset() { return bootd_nand_offset()+bootd_size()-1; }
int diags_nand_endoffset() { return diags_nand_offset()+diags_size()-1; }
int files_nand_endoffset() { return files_nand_offset()+files_size()-1; }
int ext_nand_endoffset() { return ext_nand_offset()+ext_size()-1; }

long int countSPI(char* databuffer) {
	uint8_t unsafe=0,spicount=0,complete=0;
	int i;
	for(i=SPID_OFFSET;i<SPID_OFFSET+SPID_SIZE && !unsafe;i+=4) {
		if(!complete && (!databuffer[i] || databuffer[i+1] || databuffer[i+2] || databuffer[i+3]))
			complete=1;
		if(complete && (memcmp(databuffer+i,"\xFF\xFF\xFF\xFF",4) || memcmp(databuffer+i+4,"\xFF\xFF\xFF\xFF",4)))
			unsafe=1;
		i+=4;
		if(!complete && (!databuffer[i] || databuffer[i+1] || databuffer[i+2] || databuffer[i+3]))
			unsafe=1;
		if(!complete && !unsafe)
			spicount++;
	}
	return spicount;
}

void dispmodel(int id, int dmodel, int dtype)
{	setCurColorRGB(0,0,255);
	switch(id)
	{	case 0x0E:
			disp("Nspire",0,1);
		case 0x0C:
		case 0x0D:
		case 0x0F:
		case 0x10:
		case 0x11:
		case 0x12:
			if(dmodel)
			{	switch(id)
				{	case 0x0F:
					case 0x10:
						disp("CX",0,1);
						break;
					case 0x11:
					case 0x12:
						disp("CM",0,1);
						break;
					case 0x0D:
						disp("Lab",0,1);
						break;
				}
			}
			if(dtype)
			{	switch(id)
				{	case 0x0C:
					case 0x0F:
					case 0x11:
						disp("CAS",0,1);
						break;
				}
			}
			break;
	}
	setCurColorRGB(0,0,0);
}

void dispmodelasic()
{	if(modeltype>=0 && modeltype<=3)
	{	int id=0,id2=0;
		switch(modeltype)
		{	case 0:
				id=0x0E; // non-CAS
				id2=0x10; // CX
				break;
			case 1:
				id=0x0C; // CAS
				id2=0x0F; // CX CAS
				break;
			case 2:
				id=0x12; // CM non-CAS
				break;
			case 3:
				id=0x11; // CM CAS
				break;
		}
		dispmodel(id,1,1);
		if(id2)
		{	disp("+",0,1);
			dispmodel(id2,1,1);
		}
	}
	else
		if(modeltype==0b11111)
			disp("NspireDVT",0,1);
		else dispi(modeltype,0,1);
}

void derror(char* str)
{	dispBack();
	resetConsole();
	displn(str,1,1);
	displn("Press a key...",0,1);
	while(any_key_pressed());
	while(!any_key_pressed());
}

int warning(char* str)
{	dispBack();
	resetConsole();
	displn(str,1,1);
	disp("Press y/n...",0,1);
	if(!waitConfirm())
	{	dispBack();
		return 0;
	}
	return 1;
}

void truncSpace(char* str, int size)
{	int i;
	for(i=0;i<size;i++)
	{	if(str[i]==' ')
			str[i]=0;
		if(str[i]==0)
			return;
	}
}

void getBufDataInfo(char* data_buf, int size, image_info* data_info, int lim)
{	int i;
	int rsize=0;
	if(!lim) lim=MAX_IMAGES;
	memset(data_info,0,(lim+(lim>1))*sizeof(image_info));
	for(i=0;i<lim;i++)
	{
		data_info->hw=HW_NONE;
		memset(data_info->version,0,VERSION_SIZE);
		char* data8000=0;
		char* field8000=0;
		int size8000=0;
		if(*data_buf)
			size8000=getFieldData(data_buf,0x8000,0,size-rsize,&data8000,&field8000);
		if(field8000!=data_buf)
			size8000=0;
		if(size8000)
			copyFieldData(data8000, 0x8020, 0, size8000, data_info->version, VERSION_SIZE);
		truncSpace(data_info->version,VERSION_SIZE);
		memset(data_info->type,0,TYPE_SIZE);
		if(size8000>0)
			copyFieldData(data8000, 0x8040, 0, size8000, data_info->type, TYPE_SIZE);
		truncSpace(data_info->type,TYPE_SIZE);
		char data_flags[FLAGS_SIZE];
		memset(data_flags,0,FLAGS_SIZE);
		if(size8000>0)
			copyFieldData(data8000, 0x8080, 0, size8000, data_flags, FLAGS_SIZE);
		data_info->compressed = data_flags[FLAGS_SIZE-1];
		data_info->base=(((long int)data_flags[0])<<24)+(((long int)data_flags[1])<<16)+(((int)data_flags[2])<<8)+data_flags[3];
		data_info->size=0;
		if(size8000>0) data_info->size=getTotalFieldsSize(data_buf,size,0);
		char data_id[ID_SIZE];
		memset(data_id,0,ID_SIZE);
		if(size8000>0)
			copyFieldData(data8000, 0x8010, 1, size8000, data_id, ID_SIZE);
		data_info->id=chex2int(data_id[1])*16+chex2int(data_id[2]);
		memset(data_id,0,ID_SIZE);
		if(size8000>0)
			copyFieldData(data8000, 0x8010, 2, size8000, data_id, ID_SIZE);
		data_info->id2=chex2int(data_id[1])*16+chex2int(data_id[2]);
		if((data_info->id==0x0F || data_info->id==0x10 || data_info->id==0x11 || data_info->id==0x12) && !strcmp(data_info->type,"BOOT2")) { // CX/CM Boot2 image
			data_info->hw=HW_NAPOCA;
			if(memcmp(data_info->type,"BOOT",4) && memcmp(data_info->type,"DIAGS",5)
				||
				(data_info->version[1]!='.' || data_info->version[0]>'3'
				|| (data_info->version[0]=='3' && data_info->version[2]>'2')
				|| (data_info->version[0]=='3' && data_info->version[2]=='2' && data_info->version[3]>'3')
				|| (data_info->version[0]=='3' && data_info->version[2]=='2' && data_info->version[3]=='3')))
				data_info->hw=HW_NONE;
		}
		rsize+=data_info->size;
		data_buf+=data_info->size;
		if(!(data_info->size) || rsize>=size) {
			data_info++;
			data_info->offset=rsize;
			break;
		}
		else {
			data_info++;
			data_info->offset=rsize;
		}
	}
}

void getFilesDataInfo(char** data_buf, char* file, int* size, image_info* data_info, int* pfsize)
{	FILE* h = fopen(file,"rb");
	int fsize=0;
	memset(data_info,0,sizeof(image_info)*MAX_IMAGES);
	if(h)
	{	fsize=filelength(h);
#ifdef MULTI
		if(fsize && fsize!=*size)
#else
		if(fsize>*size)
#endif
		{
			*data_buf=realloc(*data_buf,fsize);
			if(*data_buf)
				*size=fsize;
		}
	}
	memset(*data_buf,0,*size);
	if(h) {
		if(*size>=fsize)
			fread(*data_buf,1,fsize,h);
		fclose(h);
	}
	if(*size>=fsize)
		getBufDataInfo(*data_buf, *size, data_info,0);
	if(pfsize) *pfsize=fsize;
}

void getFlashDataInfo(char* data_buf, int nand_offset, int size, image_info* data_info)
{	memset(data_buf,0,size);
	bc_read_nand(data_buf,size,nand_offset,0,0,NULL);
	memset(data_info,0,sizeof(image_info)*MAX_IMAGES);
	getBufDataInfo(data_buf, size, data_info,0);
}

void dispManufInfo(unsigned char* manufdata)
{	int ccol = getCol();
	int id=manufdata[MODEL_OFFSET];
	int localid=getLocale(manufdata+LOCAL_OFFSET);
	int ram=manufdata[SDRAM_OFFSET];
	int rom=manufdata[PART_TABLE_OFFSET];
	int spi=manufdata[SPIC_OFFSET];
	long int rspi=countSPI(manufdata);
	switch(id)
	{	case 0x0C:
		case 0x0D:
		case 0x0E:
		case 0x0F:
		case 0x10:
		case 0x11:
		case 0x12:
			setCol(ccol+1);
			setCurColorRGB(0,0,0);
			disp("RAM:",0,1);
			setCurColorRGB(159,0,191);
			if(ram==0x12)					disp("64M",0,1);
			else if(ram==0x11 || ram==0xFF)	disp("32M",0,1);
			else							dispi(ram,0,1);
			setCurColorRGB(0,0,0);
			setCol(ccol+10);
			disp("ROM:",0,1);
			setCurColorRGB(159,0,191);
			if(rom==0x91)					disp("128M",0,1);
			else if(rom==0xFF)				disp("32M",0,1);
			else							dispi(rom,0,1);
			setCurColorRGB(0,0,0);
			setCol(ccol+18);
			displn("",0,1);

			setCol(ccol+1);
			dispmodel(id,1,1);
			setCol(ccol+10);

			setCurColorRGB(159,0,191);
			if(id>=0x0F) {
				if(manufdata[CR4_OFFSET])
					disp("CR4",0,1);
				else if(manufdata[CR4_OFFSET-1]&0xF0)
					disp("CR",0,1);
				else
					disp("Napoca",0,1);
			}
			setCurColorRGB(0,0,0);
			displn("",0,1);
			setCol(ccol+1);
			if(id>=0x0F) {
				disp("SPI:",0,1);
				setCurColorRGB(159,0,191);
				dispi(spi,0,1);
				setCurColorRGB(0,0,0);
				disp("/",0,1);
				setCurColorRGB(159,0,191);
				dispi(rspi,0,1);
			}
			setCurColorRGB(0,0,0);
			setCol(ccol+9);
			setCol(ccol+10);
			disp("Loc:",0,1);
			setCurColorRGB(159,0,191);
			if(localid<0xFF) {
				if(localid>=0 && localid<=NLANG) {
					disp(dlocales[localid],0,1);
				}
				else
					dispi(localid,0,1);
			}
			setCurColorRGB(0,0,0);

			setCol(ccol);
		default:
			displn("",0,1);
	}
	setCurColorRGB(0,0,0);
}

void dispDataInfo(image_info* data_info, int max, int tsize)
{	int i;
	int ccol = getCol();
	setCol(ccol+1);
	int last=1;
	for(i=0;i<max;i++)
	{
		if(data_info->size)
		{
			drwBufBox(getScreen(),ccol*CHAR_WIDTH-1,getLine()*CHAR_HEIGHT-(getLine()>0),(ccol+IMGINFO_WIDTH)*CHAR_WIDTH-1,(getLine()+2)*CHAR_HEIGHT-1,0);
			setCurColorRGB(0,0,0);
			disp(data_info->type,0,1);
			setCol(ccol+1+TYPE_SIZE-2);
			setCurColorRGB(255,127,0);
			if(data_info->compressed)	disp("cmp",0,1);
			else				disp("raw",0,1);
			disp(" ",0,1);
			setCurColorRGB(191,0,159);
			disp(data_info->version,0,1);
			displn("",0,1);
			setCol(ccol+1);
			setCurColorRGB(0,0,0);
			setCol(ccol+1);
			setCurColorRGB(0,0,0);
			dispmodel(data_info->id,1,1);
			if(data_info->id2)
			{
				disp("+",0,1);		
				dispmodel(data_info->id2,1,1);		
			}
			setCol(ccol+10);
			setCurColorRGB(159,0,191);
			if(data_info->id>=0x0F) {
				if(data_info->hw==HW_CXCR4)			disp("CR4",0,1);
				else if(data_info->hw==HW_CXCR)			disp("CR",0,1);
				else if(data_info->hw==HW_NAPOCA)	disp("Nap",0,1);
				else if(data_info->hw==HW_NONE)	{
					disp("Nap",0,1);
				setCurColorRGB(0,0,0);
					disp("+",0,1);
				setCurColorRGB(159,0,191);
					disp("CR",0,1);
				}
				else dispi(data_info->hw,0,1);
			}
			setCol(ccol+IMGINFO_WIDTH-1-PG_H_WIDTH);
			dispPages(data_info->size,1);
			tsize-=data_info->size;
			displn("",0,1);
			setCol(ccol+1);
			data_info++;
		}
		else if(last) {
			last=0;
			disp("Free space",0,1);
			setCol(ccol+IMGINFO_WIDTH-1-PG_H_WIDTH);
			dispPages(tsize);
		}
		else
			break;
	}
	setCol(ccol);
}

void dispPass() {
	setCurColorRGB(0,127,0);
	displn("OK",0,1);
	setCurColorRGB(0,0,0);	
}

void dispFail() {
	setCurColorRGB(255,0,0);displn("Fail",0,1);setCurColorRGB(0,0,0);
}

int isCM() {
	return modeltype&0b10;
}

int isCas()
{	return modeltype&0b1;
}

int isNCas()
{	return modeltype>=4 || !(modeltype&0b1);
}

int isValidManufImage(unsigned char* flashdata) {
	int newid=flashdata[MODEL_OFFSET];
	int spi=flashdata[SPIC_OFFSET];
	long int rspi=countSPI(flashdata);
	return	(spi<=rspi || asichw<HW_CXCR4) &&
			((manufflashid==0x0E && newid==0x0C && isCas())		// Nspire -> Nspire CAS
			|| (manufflashid==0x0C && newid==0x0E && isNCas())	// Nspire CAS -> Nspire
			|| (manufflashid==0x0F && newid==0x11 && isCM())	// Nspire CX CAS -> Nspire CM CAS
			|| (manufflashid==0x11 && newid==0x0F && isCM())	// Nspire CM CAS -> Nspire CX CAS
			|| (manufflashid==0x10 && newid==0x12 && isCM())	// Nspire CX -> Nspire CM
			|| (manufflashid==0x12 && newid==0x10 && isCM())	// Nspire CM -> Nspire CX
			|| (manufflashid==0x10 && newid==0x0F && isCas())	// Nspire CX -> Nspire CX CAS
			|| (manufflashid==0x0F && newid==0x10 && isNCas())	// Nspire CX CAS -> Nspire CX
			|| (manufflashid==0x12 && newid==0x11 && isCas())	// Nspire CM -> Nspire CM CAS
			|| (manufflashid==0x11 && newid==0x12 && isNCas())	// Nspire CM CAS -> Nspire CM
			|| (manufflashid==newid))
			;
}

int isValidFlash(unsigned char* data, int size)
{	image_info data_info;
	getBufDataInfo(data, size, &data_info,1);
	return	(data_info.hw==HW_NONE || data_info.hw==asichw) &&
			(((manufflashid==0x0E || manufflashid==0x0C) && (data_info.id==0x0E || data_info.id==0x0C))
#ifdef MULTI
				|| (manufflashid>=0x0F && manufflashid<=0x12 && data_info.id>=0x0F && data_info.id<=0x12)
#else
				|| ((manufflashid==0x0F || manufflashid==0x10) && (data_info.id==0x0F || data_info.id==0x10))
				|| ((manufflashid==0x11 || manufflashid==0x12) && (data_info.id==0x11 || data_info.id==0x12))
#endif
				|| (manufflashid==data_info.id));
}

int isValidRun(unsigned char* data, int size)
{	image_info data_info;
	getBufDataInfo(data, size, &data_info,1);
	if(!memcmp(data_info.type,"BOOT",4))
		return isValidFlash(data,size);
	return	((manufflashid==0x0E || manufflashid==0x0C || manufflashid==0x0D) && (data_info.id==0x0E || data_info.id==0x0C || data_info.id==0x0D))
			|| ((manufflashid==0x0F || manufflashid==0x10 || manufflashid==0x11 || manufflashid==0x12) && (data_info.id==0x0F || data_info.id==0x10 || data_info.id==0x11 || data_info.id==0x12))
			|| (manufflashid==data_info.id);
}

void dispPages(int pages, int unit) {
	int p=(pages/NAND_PAGE_SIZE)+((pages%NAND_PAGE_SIZE)>1);
	setCurColorRGB(255,0,127);
	if(!(getMode()&VERT_MODE))
		setCol(getCol()+((p<10000)?1+((p<1000)?1+((p<100)?1+((p<10)?1:0):0):0):0));
	dispi(p,0,1);
	if(unit)
		disp("P",0,1);
	setCurColorRGB(0,0,0);
}

char* flashdata=0;
char* filesdata=0;
image_info boot2flash_info[MAX_IMAGES+1];
image_info diagsflash_info[MAX_IMAGES+1];
image_info extflash_info[MAX_IMAGES+1];
int filebufsize=0;
#ifdef MULTI
image_info files_info[MAX_IMAGES+1];
#else
image_info boot2files_info[MAX_IMAGES+1];
image_info diagsfiles_info[MAX_IMAGES+1];
image_info extfiles_info[MAX_IMAGES+1];
int boot2filessize=0;
int diagsfilessize=0;
int extfilessize=0;
#endif
int manuffilessize;

void getFlashManuf() {
	bc_read_nand(flashdata,max(NAND_PAGE_BLOCK*NAND_PAGE_SIZE,manuf_size()),manuf_nand_offset(),0,0,NULL);
	manufflashid = flashdata[MODEL_OFFSET];
}

void getFlashBoot2() {
	getFlashDataInfo(flashdata,boot2_nand_offset(),boot2_size(),boot2flash_info);
}

void getFlashBootD() {
	bc_read_nand(flashdata,bootd_size(),bootd_nand_offset(),0,0,NULL);
}

void getFlashDiags() {
	getFlashDataInfo(flashdata,diags_nand_offset(),diags_size(),diagsflash_info);
}

void getFlashExt() {
	getFlashDataInfo(flashdata,ext_nand_offset(),ext_size(),extflash_info);
}

#ifdef MULTI
void getFiles() {
	getFilesDataInfo(&filesdata,imgpath,&filebufsize,files_info,NULL);
}
#else
void getFilesBoot2() {
	getFilesDataInfo(&filesdata,"/documents/ndless/boot2.img.tns",&filebufsize,boot2files_info,&boot2filessize);
}

void getFilesDiags() {
	getFilesDataInfo(&filesdata,"/documents/ndless/diags.img.tns",&filebufsize,diagsfiles_info,&diagsfilessize);
}

void getFilesExt() {
	getFilesDataInfo(&filesdata,"/documents/ndless/extnd.img.tns",&filebufsize,extfiles_info,&extfilessize);
}
#endif

void getFilesManuf() {
	FILE* h = fopen(manufpath,"rb");
	if(h)
	{	manuffilessize=filelength(h);
#ifdef MULTI
		if(manuffilessize && manuffilessize!=filebufsize)
#else
		if(manuffilessize>filebufsize)
#endif
		{
			filesdata=realloc(filesdata,manuffilessize);
			if(filesdata)
				filebufsize=manuffilessize;
		}
	}
	memset(filesdata,0,filebufsize);
	if(h) {
		if(filebufsize>=manuffilessize)
			fread(filesdata,1,manuffilessize,h);
		fclose(h);
	}
}

extern uint8_t sscreen[SCREEN_SIZE];
void dispBack()
{	memcpy(getScreen(),sscreen,SCREEN_SIZE);
}

int main(int argc, char** argv) {
	strcpy(manufpath,"/documents/ndless/manuf.img.tns");
	strcpy(bootdpath,"/documents/ndless/bootd.img.tns");
#ifdef MULTI
	strcpy(imgpath,"/documents/ndless/dump.img.tns");
	*(strrchr(argv[0],'.'))=0;
	cfg_register_fileext("img", strrchr(argv[0],'/')+1);
#endif
	if(argc>=2) {
		FILE* h = fopen(argv[1],"rb");
		if(h) {
			uint32_t id;
			if(1==fread(&id,4,1,h)) {
				if(id==0x796EB03C)
					strcpy(manufpath,argv[1]);
				else if(id==0x928CC6AA)
					strcpy(bootdpath,argv[1]);
#ifdef MULTI
				else if((id&0xFFFF)==0x0F80)
					strcpy(imgpath,argv[1]);
#endif
			}
			fclose(h);
		}
	}
	flashdata=malloc(NAND_PAGE_BLOCK*NAND_PAGE_SIZE);
	filesdata=malloc(NAND_PAGE_BLOCK*NAND_PAGE_SIZE);
	int i;
	getFlashManuf();
	// TI-Nspire CX/CM partition table
	pgoffsets[1]=BOOT2_PAGE_OFFSET;
	pgoffsets[2]=BOOTD_PAGE_OFFSET;
	pgoffsets[3]=DIAGS_PAGE_OFFSET;
	pgoffsets[4]=FILES_PAGE_OFFSET;
	extpgoffset = DGSTA_PAGE_OFFSET+DGSTA_PAGE_NUM;
	if(!memcmp(flashdata+PART_TABLE_OFFSET,PART_TABLE_ID,PART_TABLE_SIZE)) {
		pgoffsets[0]=0;
		for(i=1;i<NPARTS;i++)
			pgoffsets[i]=*((long int*)(flashdata+offsets_offsets[i]))/NAND_PAGE_SIZE;
		pgoffsets[5]=NDEND_PAGE_OFFSET;
	}
	for(i=0;i<NPARTS;i++)
		pgsizes[i]=pgoffsets[i+1]-pgoffsets[i];
	i=0;
	while(pgoffsets[i+1]<DGSTA_PAGE_OFFSET)
		i++;
	pgsizes[i]=DGSTA_PAGE_OFFSET-pgoffsets[i];
	while(pgoffsets[i]<extpgoffset)
		i++;
	extpgsize=pgoffsets[i]-extpgoffset;
	
	free(flashdata);
	flashdata=0;
	flashdata=malloc(max(manuf_size(),max(bootd_size(),max(ext_size(),max(diags_size(),boot2_size())))));
	char downgradeversion[VERSION_SIZE];

	initScreen();
	startScreen();
	convertRGB565(sscreen);
	dispBack();
	if((!flashdata && max(manuf_size(),max(bootd_size(),max(ext_size(),max(diags_size(),boot2_size())))))) {
			setCurColorRGB(0,0,0);
			displn("**************** ERROR ***************",0,1);
			displn("",0,1);
			displn("Not enough memory.",0,1);
			displn("",0,1);
			int size=0, total=0;
			total=max(manuf_size(),max(bootd_size(),max(ext_size(),max(boot2_size(),diags_size()))));
#ifndef MULTI
			total+=max(manuf_size(),max(ext_size(),max(boot2_size(),diags_size())));
#endif
			if(flashdata) 	   size+=max(manuf_size(),max(bootd_size(),max(ext_size(),max(boot2_size(),diags_size()))));
			disp("Total needed : ",0,1);dispi(total/1000000,0,1);	disp(".",0,1);dispi(total/10000%10,0,1);displn("MB",0,1);
			disp("Allocated    : ",0,1);dispi(size/1000000,0,1);	disp(".",0,1);dispi(size/10000%10,0,1);	displn("MB",0,1);
			displn("Reset or downgrade and try again.",1,1);
			displn("",0,1);
			disp("Press a key...",0,1);
			while(any_key_pressed());
			while(!any_key_pressed());
	}
	else {
	int menu=0;
	int menu2=0;
	int option=0;
	char* file;
	int error=0;
	char* flashbuf;
	char* filesbuf;
	int size, offset, endoffset;
	int bootorder;
	modeltype = *(volatile unsigned int*) 0x900A002C;
	modeltype = (modeltype>>(58-32))&(0b11111);
	int update=1;
	int bootdusedpages=0;
	int enabledoptions[4];
	while(menu>=0)
	{
		if(update)
		{	disp("Refreshing...",0,1);
			getFlashManuf();
			if(modeltype&0b10) asichw=HW_NAPOCA;
			if(!(modeltype&0b10) && (manufflashid==0x0F || manufflashid==0x10)) {
				if(flashdata[CR4_OFFSET])
					asichw=HW_CXCR4;
				else if(flashdata[CR4_OFFSET-1]&0xF0)
					asichw=HW_CXCR;
				else
					asichw=HW_NAPOCA;
			}
			getFlashBoot2();
			getFlashDiags();
			getFlashExt();
#ifdef MULTI
			getFiles();
#else
			getFilesBoot2();
			getFilesDiags();
			getFilesExt();
#endif
			bootorder=1;
			*downgradeversion=0;
			bootdusedpages=0;
			getFlashBootD();
			for(i=0;i<bootd_pages_num();i++)
			{	char* tmp=flashdata+i*NAND_PAGE_SIZE;
				char tmp2[5];
				memcpy(tmp2,tmp,4);
				tmp2[4]=0;
				if(!strcmp(tmp2,"\xAA\xC6\x8C\x92"))
				{	sprintf(downgradeversion,"%i.%i.%i.%i",tmp[7],tmp[6]/10,tmp[6]%10,tmp[5]*256+tmp[4]);
					bootorder=tmp[0x10]|tmp[0x11]|tmp[0x12]|tmp[0x13];
					bootdusedpages++;
				}
			}
			update=0;
			dispBack();
			resetConsole();
		}
		while(!option && menu>=0)
		{	dispBack();
			resetConsole();
			setCurColorRGB(0,0,0);
			int ccol = 3;
			setCol(ccol+1);
			int ccol2 = (MAX_COL-ccol)/2+ccol;
			disp("Flash partitions",0,1);
			setCol(ccol2+1);
			displn("FileSystem images",0,1);
			drwBufHoriz(getScreen(),getLine()*CHAR_HEIGHT-1,0,SCREEN_WIDTH-1,0);

			int cline0=getLine();
			int cline=cline0;
			if(!menu || menu==1 || menu==4) {
				setCol(1);
				setMode(1);
				disp("MNF",0,1);
				setLine(cline);
				setCol(ccol2-PG_V_WIDTH+1);
				dispPages(manuf_size(),1);
				setLine(cline);
				setCol(MAX_COL-PG_V_WIDTH+1);
				dispPages(manuffilessize,1);
				setMode(0);
				setCol(ccol);
				setLine(cline);
				getFlashManuf();
				dispManufInfo(flashdata);
				setLine(cline);
				displn("",0,1);
				setLine(cline);
				setCol(ccol2);
				getFilesManuf();
				dispManufInfo(filesdata);
				setLine(cline+2);
				displn("",0,1);
				cline=getLine();
				drwBufHoriz(getScreen(),cline*CHAR_HEIGHT-1,0,SCREEN_WIDTH-1,0);
			}
#ifdef MULTI
			setCol(MAX_COL-PG_V_WIDTH+1);
			setMode(1);
			dispPages(filebufsize,1);
			setMode(0);
			setLine(cline);
			setCol(ccol2);
			if(filebufsize) dispDataInfo(files_info,(menu==2)?EDISP_IMAGES:DISP_IMAGES*2,filebufsize);
			setLine(cline);
#endif
			if(!menu || menu==2) {
				drwBufHoriz(getScreen(),cline*CHAR_HEIGHT-1,0,ccol2*CHAR_WIDTH-1,0);
				setCol(1);
				setMode(1);
				disp("BOOT2",0,1);
				setLine(cline);
				setCol(ccol2-PG_V_WIDTH+1);
				dispPages(boot2_size(),1);
				setLine(cline);
#ifndef MULTI
				setCol(MAX_COL-PG_V_WIDTH+1);
				dispPages(boot2filessize,1);
#endif
				setMode(0);
				setCol(ccol);
				setLine(cline);
				dispDataInfo(boot2flash_info,(!menu)?DISP_IMAGES:EDISP_IMAGES,boot2_size());
#ifndef MULTI
				setCol(ccol2);
				setLine(cline);
				if(boot2filessize) dispDataInfo(boot2files_info,(!menu)?DISP_IMAGES:EDISP_IMAGES,boot2filessize);
#endif
				if(!menu) {
					cline=cline+2*DISP_IMAGES;
					setLine(cline);
				}
			}
			if(!menu || menu==3) {
#ifdef MULTI
				drwBufHoriz(getScreen(),cline*CHAR_HEIGHT-1,0,ccol2*CHAR_WIDTH-1,0);
#else
				drwBufHoriz(getScreen(),cline*CHAR_HEIGHT-1,0,SCREEN_WIDTH-1,0);
#endif
				setCol(1);
				setMode(1);
				disp("DIAG",0,1);
				setLine(cline);
				setCol(ccol2-PG_V_WIDTH+1);
				dispPages(diags_size(),1);
				setLine(cline);
#ifndef MULTI
				setCol(MAX_COL-PG_V_WIDTH+1);
				dispPages(diagsfilessize,1);
#endif
				setMode(0);			
				setCol(ccol);
				setLine(cline);
				dispDataInfo(diagsflash_info,(!menu)?DISP_IMAGES-1:EDISP_IMAGES,diags_size());
#ifndef MULTI
				setCol(ccol2);
				setLine(cline);
				if(diagsfilessize) dispDataInfo(diagsfiles_info,(!menu)?DISP_IMAGES-1:EDISP_IMAGES,diagsfilessize);
#endif
				if(!menu) {
					cline=cline+2*(DISP_IMAGES-1);
					setLine(cline);
				}
			}
			if(!menu || menu==6) {
#ifdef MULTI
				drwBufHoriz(getScreen(),cline*CHAR_HEIGHT-1,0,ccol2*CHAR_WIDTH-1,0);
#else
				drwBufHoriz(getScreen(),cline*CHAR_HEIGHT-1,0,SCREEN_WIDTH-1,0);
#endif
				setCol(1);
				setMode(1);
				disp("EXTND",0,1);
				setLine(cline);
				setCol(ccol2-PG_V_WIDTH+1);
				dispPages(ext_size(),1);
				setLine(cline);
#ifndef MULTI
				setCol(MAX_COL-PG_V_WIDTH+1);
				dispPages(extfilessize,1);	
#endif
				setMode(0);		
				setCol(ccol);
				setLine(cline);
				dispDataInfo(extflash_info,(!menu)?DISP_IMAGES:EDISP_IMAGES,ext_size());
#ifndef MULTI
				setCol(ccol2);
				setLine(cline);
				if(extfilessize) dispDataInfo(extfiles_info,(!menu)?DISP_IMAGES:EDISP_IMAGES,extfilessize);
#endif
			}
			cline=cline0+EDISP_IMAGES*2+(!menu || menu==1 || menu==4);
			setLine(cline);
			setCol(0);
			drwBufHoriz(getScreen(),cline*CHAR_HEIGHT-1,0,SCREEN_WIDTH-1,0);
			cline=getLine();
			setCol(1);
			setMode(1);
			disp("BD",0,1);
			setLine(cline);
			setCol(ccol2-PG_V_WIDTH+1);
			dispPages(bootd_size(),0);
			setMode(0);
			setLine(cline);
			setCol(ccol+1);
			disp("min =",0,1);
			setCurColorRGB(159,0,191);
			disp(downgradeversion,0,1);
			setCurColorRGB(0,0,0);
			setCol(ccol2+1);
			disp("ASIC   :",0,1);
			dispmodelasic();
			displn("",0,1);
			setCol(ccol+1);
			disp("boot=",0,1);
			setCurColorRGB(159,0,191);
			if(bootorder)	disp("Diags",0,1);
			else			disp("Boot2",0,1);
			setCurColorRGB(0,0,0);
			setCol(ccol2-PG_V_WIDTH-PG_H_WIDTH-1);
			dispPages(bootdusedpages*NAND_PAGE_SIZE,1);
			setCol(ccol2+1);			
			disp("HW     :",0,1);
			setCurColorRGB(159,0,191);
			if(asichw==HW_NAPOCA) disp("Nap",0,1);
			else if(asichw==HW_CXCR) disp("CR",0,1);
			else if(asichw==HW_CXCR4) disp("CR4",0,1);
			setCurColorRGB(0,0,0);
			displn("",0,1);
			drwBufVert(getScreen(),0,getLine()*CHAR_HEIGHT-2,CHAR_HEIGHT,0);
			drwBufHoriz(getScreen(),0,3*CHAR_WIDTH,SCREEN_WIDTH-1);
			drwBufHoriz(getScreen(),getLine()*CHAR_HEIGHT-1,0,SCREEN_WIDTH-1,0);
			drwBufVert(getScreen(),ccol*CHAR_WIDTH-2,0,getLine()*CHAR_HEIGHT-2,0);
			drwBufVert(getScreen(),ccol*CHAR_WIDTH-1,0,getLine()*CHAR_HEIGHT-2,0);
			drwBufVert(getScreen(),(ccol2-PG_V_WIDTH)*CHAR_WIDTH-1,CHAR_HEIGHT,getLine()*CHAR_HEIGHT-2,0);
			drwBufVert(getScreen(),ccol2*CHAR_WIDTH-2,0,getLine()*CHAR_HEIGHT-2,0);
			drwBufVert(getScreen(),ccol2*CHAR_WIDTH-1,0,getLine()*CHAR_HEIGHT-2,0);
			drwBufVert(getScreen(),(MAX_COL-PG_V_WIDTH)*CHAR_WIDTH-1,CHAR_HEIGHT,(getLine()-2)*CHAR_HEIGHT-2,0);
			drwBufVert(getScreen(),SCREEN_WIDTH-1,0,getLine()*CHAR_HEIGHT-2,0);
			if(menu==0)
			{	displn("1) Manuf options 2) Boot2 area 3) Diags area",0,1);
				disp  ("4) Manuf area    5) BootD area 6) Ext   area",0,1);
			}
			else if(menu==1) {
				enabledoptions[0]=0;
				enabledoptions[1]=0;
				enabledoptions[2]=0;
				enabledoptions[3]=0;
				if(menu2==1) {
					disp ("Manuf hwID : ",0,1);
					if(isCas() && isNCas()) {
						displn("1) Switch CAS/NCAS",0,1);
						enabledoptions[0]=1;
					}
					if(isCM()) {
						displn("             2) Switch CX/CM",0,1);
						enabledoptions[1]=1;
					} 
				}
				else if(menu2==2)
				{	displn(  "Manuf lang : 1)GB 2)BE 3)TW 4)-- 5)CN 6)ar 7)da 8)de",0,1);
					disp  (  "        9)en A)es B)fi C)fr D)it E)nl F)no G)pt H)sv",0,1);
				}
				else {
					getFlashManuf();
					if(countSPI(flashdata)==flashdata[SPIC_OFFSET]) {
						if(manufflashid>=0x0F && memcmp(flashdata+BOOTG_SIZE_OFFSET,"\0\0\0",4))
							enabledoptions[2]=1;
					}
					else
						if (manufflashid>=0x0F)
							enabledoptions[3]=1;
					disp("Manuf opts : 1) Model ID  ",0,1);
					if(enabledoptions[2])
						disp("7) Remove Boot graphics",0,1);
					displn("",0,1);
					disp("             2) Language  ",0,1);
					if(enabledoptions[3])
						disp("8) Fix SPI count",0,1);
				}
			}
			else if(menu==2)
			{	displn("Boot2 area : 1) Dump         2) Flash",0,1);
				disp  ("             3) Run          4) Run   6) Trim",0,1);
			}
			else if(menu==3)
			{	displn("Diags area : 1) Dump         2) Flash",0,1);
				disp  ("             3) Run          4) Run   6) Trim",0,1);
			}
			else if(menu==6)
			{	displn("ExtnD area : 1) Dump         2) Flash",0,1);
				disp  ("             3) Run          4) Run   6) Trim",0,1);
			}
			else if(menu==4)
			{	displn("Manuf area : 1) Dump         2) Flash",0,1);
			}
			else if(menu==5)
			{	displn("BootD opts : 1) Dump 7) minOS=0",0,1);
				disp  ("             8) boot = Boot2/Diags",0,1);
			}
			if(!menu)
				disp(" 0) Exit",0,1);
			resetConsole();
			setCurColorRGB(255,127,0);
			disp("4.3",0,1);
			setCurColorRGB(0,0,0);
			while(!any_key_pressed());
			if(isKeyPressed(KEY_NSPIRE_ESC) || isKeyPressed(KEY_NSPIRE_0))
			{	if(menu2>0) menu2=0;
				else if(menu>0) menu=0;
				else	   menu=-1;
			}
			else {
				if(isKeyPressed(KEY_NSPIRE_1)) option=1;
				if(isKeyPressed(KEY_NSPIRE_2)) option=2;
				if(isKeyPressed(KEY_NSPIRE_3)) option=3;
				if(isKeyPressed(KEY_NSPIRE_4)) option=4;
				if(isKeyPressed(KEY_NSPIRE_5)) option=5;
				if(isKeyPressed(KEY_NSPIRE_6)) option=6;
				if(isKeyPressed(KEY_NSPIRE_7)) option=7;
				if(isKeyPressed(KEY_NSPIRE_8)) option=8;
				if(isKeyPressed(KEY_NSPIRE_9)) option=9;
				if(isKeyPressed(KEY_NSPIRE_A)) option=10;
				if(isKeyPressed(KEY_NSPIRE_B)) option=11;
				if(isKeyPressed(KEY_NSPIRE_C)) option=12;
				if(isKeyPressed(KEY_NSPIRE_D)) option=13;
				if(isKeyPressed(KEY_NSPIRE_E)) option=14;
				if(isKeyPressed(KEY_NSPIRE_F)) option=15;
				if(isKeyPressed(KEY_NSPIRE_G)) option=16;
				if(isKeyPressed(KEY_NSPIRE_H)) option=17;
				if(!menu) {
					menu=option;
					menu2=0;
					option=0;
				}
				if(menu==1 && !menu2 && option<=2) {
					menu2=option;
					option=0;
				}
			}
			while(any_key_pressed());
		}

		if(menu>=2 && menu<=6)
		{
			if(menu==4) file=manufpath;
#ifdef MULTI
			else if(menu==2 || menu==3 || menu==6) file=imgpath;
#else
			else if(menu==2) file="/documents/ndless/boot2.img.tns";
			else if(menu==3) file="/documents/ndless/diags.img.tns";
			else if(menu==6) file="/documents/ndless/extnd.img.tns";
#endif
			else if(menu==5) file=bootdpath;
		}
				
		// MANUF/BOOT2/DIAGS/BOOTD
		if(	(menu==1 && ((menu2==1 && ((option==1 && enabledoptions[0]) || (option==2 && enabledoptions[1])))
						|| (menu2==2 && option>=1) || (option==7 && enabledoptions[2]) || (option==8 && enabledoptions[3]))) 
			|| ((menu==2 || menu==3 || menu==6) && (option>=1 && option<=4 || option==6))
			|| (menu==4 && (option>=1 && option<=2)) || (menu==5 && (option==1 || option==5 || (option>=7 && option<=8))))
		{	update=1;
			dispBack();
			int filesize=0, flashsize=0;
			FILE* hfile;
			
			if(menu==2)			{
				getFlashBoot2();
				flashbuf=flashdata;
				flashsize=boot2_size();
				offset=boot2_nand_offset();
				endoffset=boot2_nand_endoffset();
				filesbuf=filesdata;
#ifdef MULTI
				getFiles();
				filesize=filebufsize;
#else
				getFilesBoot2();
				filesize=boot2filessize;
#endif
			}
			else if(menu==3)	{
				getFlashDiags();
				flashbuf=flashdata;
				flashsize=diags_size();
				offset=diags_nand_offset();
				endoffset=diags_nand_endoffset();
				filesbuf=filesdata;
#ifdef MULTI
				getFiles();
				filesize=filebufsize;
#else
				getFilesDiags();
				filesize=diagsfilessize;
#endif
			}
			else if(menu==6)	{
				getFlashExt();
				flashbuf=flashdata;
				flashsize=ext_size();
				offset=ext_nand_offset();
				endoffset=ext_nand_endoffset();
				filesbuf=filesdata;
#ifdef MULTI
				getFiles();
				filesize=filebufsize;
#else
				getFilesExt();
				filesize=extfilessize;
#endif
			}
			else if(menu==5)	{
				getFlashBootD();
				flashbuf = flashdata;
				flashsize=bootd_size();
				offset=bootd_nand_offset();
				endoffset=bootd_nand_endoffset();
				filesbuf=flashdata;
				filesize=bootd_size();
			}
			else if(menu==1)	{
				getFlashManuf();
				getFilesManuf();
				flashbuf = flashdata;
				flashsize=NAND_PAGE_BLOCK*NAND_PAGE_SIZE;
				offset=manuf_nand_offset();
				endoffset=offset+flashsize-1;
				filesize=manuffilessize;
			}
			else if(menu==4)	{
				getFlashManuf();
				getFilesManuf();
				flashbuf = flashdata;
				flashsize=NAND_PAGE_BLOCK*NAND_PAGE_SIZE;
				offset=manuf_nand_offset();
				endoffset=offset+flashsize-1;
				filesbuf=filesdata;
				filesize=manuffilessize;
			}
			char* databuf=0;
							
			// flashing/running/cleaning BOOT2/DIAGS file
			if((menu==2 || menu==3 || menu==6 || menu==4) && (option==2 || option==4 || option==6) && (!filebufsize || !*filesbuf)) {
				char* tmp="I/O error on file \0                              ";
				strcat(tmp,file);
				derror( tmp );
				option=0;
			}
			
			int nflash=0,nfiles=0;
			int iflash=0,ifiles=0;
			image_info* flash_info=0;
#ifndef MULTI
			image_info* files_info=0;
#endif
			// Boot2/Diags image selection
			if(menu==2 || menu==3 || menu==6) {
				flash_info=(menu==2)?&boot2flash_info:(menu==3)?&diagsflash_info:&extflash_info;
#ifndef MULTI
				files_info=(menu==2)?&boot2files_info:(menu==3)?&diagsfiles_info:&extfiles_info;
#endif
				i=0;
				int flashdatasize=0,filesdatasize=0;
				while(i<MAX_IMAGES && flash_info[i].size) {
					nflash++;
					flashdatasize+=flash_info[i].size;
					i++;
				}
				i=0;
				while(i<MAX_IMAGES && files_info[i].size) {
					nfiles++;
					filesdatasize+=files_info[i].size;
					i++;
				}
				if((option==1 || option==2 || option==3 || option==4)) {
					int nsource=(option==1 || option==3)?nflash:nfiles;
					int ndest=(option==2)?nflash:((option==1)?nfiles:nsource);
					int isource=0,idest=0;
#ifdef MULTI
					resetConsole();
					dispBack();
					int col=2,col2=(MAX_COL-col)/2+col;
					image_info* source_info=(option==1 || option==3)?flash_info:files_info;
					image_info* dest_info=(option==2)?flash_info:((option==1)?files_info:source_info);
					char* type;
					if(nsource>1) {
						setLine(0);
						setCol(col);
						if(option==1 || option==3)
							type="Flash ";
						else
							type="FileS ";
						isource = selectImage(source_info, nsource, col, 0,"Select source image",type,(option==1 || option==3)?flashsize:filesize,NULL,0);
					}
					else if(!nsource)
						isource=-1;
					iflash=ifiles=isource;
					if((option==1 || option==2) && isource>=0) {
						if(ndest>=1) {
							setLine(0);
							setCol(col2);
							if(option==2)
								type="Flash ";
							else
								type="FileS ";
							idest = selectImage(dest_info, ndest, col2, 1,"Select dest location",type,(option==1)?filesize:flashsize,&(source_info[isource]),col);
						}
					}
#endif
					if(idest<0 || isource<0) {
						option=0;	
						resetConsole();
						dispBack();
					}
					else {
#ifdef MULTI
						int insert=(idest+1)%2;
						idest/=2;					
						iflash=(option==1 || option==3 || option==4)?isource:idest;
						ifiles=(option==2 || option==3 || option==4)?isource:idest;
#endif
						if(option==1 || option==2) {
							char* destbuf=(option==1)?filesbuf:flashbuf;
							char* sourcebuf=(option==2)?filesbuf:flashbuf;
#ifdef MULTI
							int imgdsize=(option==1)?flash_info[iflash].size:files_info[ifiles].size;
							int imgoffset=source_info[isource].offset;

							char* imgptr=sourcebuf+source_info[isource].offset;
							char* injectptr=destbuf+dest_info[idest].offset;
							int backupsize=0;//((option==1)?filessize:size)-dest_info[idest].offset-(insert?0:dest_info[idest].size);


							// images to backup size
							i=idest+(insert?0:1);
							while(dest_info[i].size && i<ndest) {
								backupsize+=dest_info[i].size;
								i++;
							}
#else
							int imgdsize=(option==1)?flashsize:filesize;
							int destsize=(option==2)?flashsize:filebufsize;
							filesize=imgdsize;
#endif
							// not enough space for backup
#ifdef MULTI
							if((injectptr+imgdsize-destbuf+backupsize)>((option==1)?filesize:flashsize))
#else
							if(imgdsize>destsize)
#endif
							{
								if(option==1) { // resize dump file buf
#ifdef MULTI
									int tsize=(injectptr+imgdsize-destbuf)+backupsize;
									filesdata=realloc(filesdata,tsize);
									destbuf=filesbuf=filesdata;
									injectptr=destbuf+dest_info[idest].offset;
									if(filesdata)
										filesize=tsize;
									else
										filesize=0;
#else
									filesize=imgdsize;
									filesdata=realloc(filesdata,filesize);
									destbuf=filesbuf=filesdata;
									if(!filesdata)
										filesize=0;
									filebufsize=filesize;
									destsize=filesize;

#endif
								}
#ifdef MULTI
								else // truncates Flash backup
									backupsize=flashsize-(injectptr-destbuf)-imgdsize;//((option==1)?filessize:size)-dest_info[idest].offset-(insert?imgdsize:max(dest_info[idest].size,imgdsize));
#endif
							}
#ifdef MULTI
							if(backupsize>=0) {
								int validsize=0;
								i=idest+(insert?0:1);
								while(validsize+dest_info[i].size<=backupsize && dest_info[i].size && i<ndest) {
									validsize+=dest_info[i].size;
									i++;
								}
								backupsize=validsize;
								memmove(	injectptr+imgdsize,
										injectptr+(insert?0:dest_info[idest].size),
										backupsize);
								memcpy(injectptr,sourcebuf+imgoffset,imgdsize);
							}
#else
							if(imgdsize<=destsize)
								memcpy(destbuf,sourcebuf,imgdsize);
#endif
							else {
								derror("Source image exceeds area and has been  rejected. Choose higher dest location.");
								option=0;
							}
						}
					}
				}
				else if(option==6) {
					flashsize=filesdatasize;					
				}
			}
			if(menu==1) // MANUF
			{	int code = -1;
				if(menu2==1) // model ID
				{
					switch(option)
					{	case 1:
							switch(manufflashid) {
								case 0x0E:
									code=0x0C;
									break;
								case 0x10:
								case 0x12:
									code=manufflashid-1;
									break;
								case 0x0C:
									code=0x0E;
									break;
								case 0x0F:
								case 0x11:
									code=manufflashid+1;
									break;
							}
							break;
						case 2:
							switch(manufflashid) {
								case 0x0F:
								case 0x10:
									code=manufflashid+2;
									break;
								case 0x11:
								case 0x12:
									code=manufflashid-2;
									break;
							}
							break;
						default:
							break;
					}
					flashbuf[MODEL_OFFSET]=code;
					option=2;
				}
				else if(menu2==2)
				{	int localid=-1;
					switch(option)
					{	case 1:
							localid=LANG_ENGB;
							break;
						case 2:
							localid=LANG_NLBE;
							break;
						case 3:
							localid=LANG_ZHTW;
							break;
						case 4:
							localid=LANG_NONE;
							break;
						case 5:
							localid=LANG_ZHCN;
							break;
						case 6:
							localid=LANG_AR;
							break;
						case 7:
							localid=LANG_DA;
							break;
						case 8:
							localid=LANG_DE;
							break;
						case 9:
							localid=LANG_EN;
							break;
						case 10:
							localid=LANG_ES;
							break;
						case 11:
							localid=LANG_FI;
							break;
						case 12:
							localid=LANG_FR;
							break;
						case 13:
							localid=LANG_IT;
							break;
						case 14:
							localid=LANG_NL;
							break;
						case 15:
							localid=LANG_NO;
							break;
						case 16:
							localid=LANG_PT;
							break;
						case 17:
							localid=LANG_SV;
							break;
						default:
							break;
					}
					if(localid>=0) {
						memcpy(flashbuf+LOCAL_OFFSET,locales[localid],LOCAL_SIZE);
						option=2;
					}
				}
				else if(option==7) {
					memset(flashbuf+BOOTG_SIZE_OFFSET,0,4);
					option=2;
				}
				else if(option==8) {
					long int rspi = countSPI(flashbuf);
					memcpy(flashbuf+SPIC_OFFSET,&rspi,4);
					option=2;					
				}
			}
			else if(menu==4 && option==2) // FLASH MANUF zone
				memcpy(flashbuf+SPIC_OFFSET,filesbuf+SPIC_OFFSET,NAND_PAGE_BLOCK*NAND_PAGE_SIZE-SPIC_OFFSET);
			else if(menu==5 && (option==7 || option==8)) // BOOTD: remove downgrade protection or switch boot order
			{	int found = 0;
				for(i=0;i<bootd_pages_num();i++)
				{	char* tmp=flashbuf+i*NAND_PAGE_SIZE;
					char tmp2[5];
					memcpy(tmp2,tmp,4);
					tmp2[4]=0;
					if(!strcmp(tmp2,"\xAA\xC6\x8C\x92"))
					{	found = 1;
						if(option==7)	tmp[7]=tmp[6]=tmp[5]=tmp[4]=0;
						else
						{	tmp[0x13]=tmp[0x12]=tmp[0x11]=0;
							if(bootorder)	tmp[0x10]=0;
							else			tmp[0x10]=1;
						}
					}
				}
				if(!found)		derror("Boot Data invalid: reset and try again.");
				else			option=2;
			}
			
			if((option==3 || option==4)) {					// running BOOT2/DIAGS
				int base=(option==3)?flash_info[iflash].base:files_info[ifiles].base;
				if(!isValidRun((option==3)?flashbuf+flash_info[iflash].offset:filesbuf+files_info[ifiles].offset,(option==3)?flash_info[iflash].size:files_info[ifiles].size))
				{	derror("Image rejected as determined unsafe if run on this model" );
					option=0;
				}
				else
				{	int s=0;
					char* rawfilesdata = 0;
//					unsigned intmask = TCT_Local_Control_Interrupts(0);
					if(base==0x11200000 || base==0x11800000) {
						s = decompressFiles((option==3)?flashbuf+flash_info[iflash].offset:filesbuf+files_info[ifiles].offset,(option==3)?flash_info[iflash].size:files_info[ifiles].size,&rawfilesdata);
						if(s) {
							memcpy((void*)base,rawfilesdata,s);
							if(base==0x11200000)
							{
								asm(
									".arm \n"
									"ldr pc, =0x11200000 @ jump to boot2\n"
								);
							}
							else if(base==0x11800000)
							{
								asm(
									".arm \n"
									"ldr pc, =0x11800000 @ jump to boot2\n"
								);
							}
							// should never reach this
						}
						free(rawfilesdata);
					}
					else {
						derror("Unsupported image" );
					}
				}
			}
			
			
			else if(option==1 || option==6 && (menu==2 || menu==3 || menu==6 || menu==5))				// dumping/cleaning BOOT2/DIAGS/MANUF
			{	resetConsole();
				dispBack();
				disp("Writing to file... ",0,1);
				if(option==6)
					remove(file);
				hfile = fopen(file,"w+b");
				if(!hfile)
				{	option=0;
					dispFail();
					derror("Error opening file" );
				}
				else
				{	if(menu==4) {
						memcpy(filesbuf,flashbuf,NAND_PAGE_BLOCK*NAND_PAGE_SIZE);
						filesize=NAND_PAGE_BLOCK*NAND_PAGE_SIZE;
					}
					else if(option==6)
						filesize=flashsize;
					int wsize=0;
					wsize=fwrite(filesbuf,1,filesize,hfile);
					if(wsize!=filesize)
					{	option=0;
						dispFail();
						derror("Error writing file" );
					}
					else {
						dispPass();
					}
					fclose(hfile);
				}
			}
			
			
			else if((option==2 || option==5))	// flashing/cleaning BOOT2/DIAGS/BOOTD/MANUF
			{	resetConsole();
				dispBack();
				disp("Validating data... ",0,1);
				if(((menu==1 || menu==4) && !isValidManufImage(flashbuf))
#ifdef MULTI
					|| ((menu==2 || menu==3 || menu==6) && !isValidFlash(flashbuf+flash_info[iflash].offset,files_info[ifiles].size))) {
#else
					|| ((menu==2 || menu==3 || menu==6) && !isValidFlash(flashbuf,flashsize))) {
#endif
						option=0;
						dispFail();
						derror("Image rejected as determined unsafe if flashed on this model");
				}
				else {
					dispPass();
					displn("NAND reflashing in progress - do not reset !",0,1);
					disp("Erasing NAND area... ",0,1);
					error = bc_nand_erase_range(offset,endoffset);
					if(error) {
						dispFail();
						derror("Error erasing NAND" );
					}
					else if(option==2)			// FLASH BOOT2/DIAGS
					{	dispPass();
						disp("Flashing NAND area... ",0,1);
						error = bc_write_nand(flashbuf,flashsize,offset);
						if(error) {
							dispFail();
							derror("Error writing to NAND" );
						}
						else {
							dispPass();
						}
					}
				}
			}
		}
		option=0;
		error=0;
	}
	}
	if(flashdata)	   free(flashdata);
	if(filesdata)	   free(filesdata);
	stopScreen();
    return 0;                         
}
#ifdef MULTI
int selectImage(image_info* data_info, int nimage, int col, int mode, char* invite, char* type, int tsize, image_info* last_info, int coll) {
	int line0=getLine();
	int iimage=0,i,oldiimage,size;
	int nitems = nimage*(mode+1)+mode;
	if(mode) nitems=min(nitems,2*MAX_IMAGES);
	int nditems=nitems+(mode==0);
	if(!mode) nditems=min(nditems,MAX_IMAGES);	
	if(nimage>1-mode) {
		while(any_key_pressed());
		while(!isKeyPressed(KEY_NSPIRE_ENTER)) {
			dispBack();
			resetConsole();
			setCol(col-2);
			setMode(1);
			disp(invite,0,1);
			resetConsole();
			setCol(col+IMGINFO_WIDTH+1);
			disp(type,0,1);
			dispPages(tsize,1);
			setMode(0);
			resetConsole();
			displn("",0,1);
			setLine(line0);
			if(last_info) {
				setCol(coll);
				dispDataInfo(last_info,1,size);
				setLine(getLine()-2);
			}
			setCol(col);
			size=tsize;
			for(i=0;i<nditems;i++) {
				if(!mode) {
					dispDataInfo(&(data_info[i]),1,size);
					size-=data_info[i].size;
				}
				else {
					if(i%2 || i==2*nimage) {
						dispDataInfo(&(data_info[i/2]),1,size);
						size-=data_info[i/2].size;
					}
					setCol(col);
				}
			}
			setLine(MAX_LGN);
			drwBufHoriz(getScreen(),getLine()*CHAR_HEIGHT-(getLine()>0),col*CHAR_WIDTH-1,(col+IMGINFO_WIDTH)*CHAR_WIDTH-1,0);
			drwBufVert(getScreen(),col*CHAR_WIDTH-1,line0*CHAR_HEIGHT,getLine()*CHAR_HEIGHT-2,0);
			drwBufVert(getScreen(),(col+IMGINFO_WIDTH)*CHAR_WIDTH-1,line0*CHAR_HEIGHT,getLine()*CHAR_HEIGHT-2,0);
			drwBufBox(getScreen(),col*CHAR_WIDTH-2,((mode?(iimage-(iimage%2)):iimage*2)+line0)*CHAR_HEIGHT-((mode?(iimage-(iimage%2)):iimage)>0?2:0),(col+IMGINFO_WIDTH)*CHAR_WIDTH-1+1,((mode?iimage+(iimage%2):iimage*2+2)+line0)*CHAR_HEIGHT+(!line0 && !iimage && mode),0);
			oldiimage=iimage;
			while(any_key_pressed());
			while(!any_key_pressed());
			iimage=iimage-(isKeyPressed(KEY_NSPIRE_UP)?1:0)+(isKeyPressed(KEY_NSPIRE_DOWN)?1:0);
			if(iimage<0) iimage=nitems-1;
			else if(iimage>=nitems) iimage=0;
			if(isKeyPressed(KEY_NSPIRE_ESC)) {
				iimage=-1;
				break;
			}
		}
	}
	return iimage;
}
#endif
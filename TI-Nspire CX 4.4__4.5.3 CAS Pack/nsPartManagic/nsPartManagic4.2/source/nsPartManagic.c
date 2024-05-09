#include <os.h>
#include "nand.h"
#include "screen.h"
#include "charmaps.h"

#define CHIP_FACTOR 7
#define CHIP_HEIGHT 23*CHIP_FACTOR
#define CHIP_WIDTH (SCREEN_WIDTH-SCREEN_WIDTH%CHIP_FACTOR)

#define NPARTS 7
#define NAND_PAGE_INCREMENT 0x40
int reds[NPARTS]={  0xFF,0   ,0   ,0   ,0xFF,0   ,0xBF};
int greens[NPARTS]={0xFF,0   ,0xBF,0xFF,0   ,0xFF,0xBF};
int blues[NPARTS]={ 0x00,0xFF,0   ,0xFF,0   ,0   ,0xBF};
char* titles[NPARTS]={"Manuf","Boot2","BootD","Diags","DgSta","ExtnD","FileS"};
long int offsets_offsets[NPARTS]={0,BOOT2_OFFSET_OFFSET,BOOTD_OFFSET_OFFSET,DIAGS_OFFSET_OFFSET,0,0,FILES_OFFSET_OFFSET};
long int pgoffsets[NPARTS+1]={MANUF_PAGE_OFFSET,0,0,0,0,0,0,NDEND_PAGE_OFFSET};
long int pgsizes[NPARTS]={0,0,0,0,0,0,0};
long int newpgoffsets[NPARTS+1];
long int newpgsizes[NPARTS]={0,0,0,0,0,0,0};
char* data[NPARTS-1];
long int criticdatasize[NPARTS]={0,0,0,0,0,0,0};
short int offscreen[SCREEN_WIDTH*SCREEN_HEIGHT];

void writeWord(unsigned char* buf, long int val)
{	memset(buf,0,4);
	while(val)
	{	*buf=val%256;
		buf++;
		val/=256;
	}
}

int getCriticDataSize(int n)
{	if(n==0)
		return 2*NAND_PAGE_SIZE;
	if(n==1 || n==3 || n==NPARTS-2) {
		int i=0;
		int s=0, ss=0, maxsize=(pgoffsets[n+1]-pgoffsets[n])*NAND_PAGE_SIZE;
		char* ptr=data[n];
		while(maxsize>0 && (ss=getTotalFieldsSize(ptr,maxsize)) && ss<=maxsize && i<2) {
			s+=ss;
			maxsize-=ss;
			ptr+=ss;
			i++;
		}
		if(n==1) s=max(11*NAND_PAGE_INCREMENT,s);
		return s;
	}
	if(n==2) return NAND_PAGE_SIZE;
	if(n==NPARTS-1) return 32*1024*1024;
	return 0;
}

int isEmpty(int n, int offset, int size)
{	int i;
	if(n==NPARTS-3) return 0;
	char* buf=data[n]+offset;
	if(offset>=pgsizes[n]*NAND_PAGE_SIZE)
		size=0;
	else if(offset+size>=pgsizes[n]*NAND_PAGE_SIZE)
		size=pgsizes[n]*NAND_PAGE_SIZE-offset;
	for(i=0;i<size;i++)
		if(buf[i]!=0xFF && buf[i]!=0x00) return 0;
	return 1;
}

void dispChip()
{	int i,j,r,g,b,k=0,s=0,r2,g2,b2;
	int curpg=0, cursz=0;
	int x0=(SCREEN_WIDTH-CHIP_WIDTH)/2, y0=1;
	setCurColorRGB(255,255,255);
	drwBufBox(offscreen,x0-1,y0-1,x0+CHIP_WIDTH+1,y0+CHIP_HEIGHT+1);
	for(j=0;j<CHIP_HEIGHT && s<NPARTS;j+=CHIP_FACTOR)
		for(i=0;i<CHIP_WIDTH && s<NPARTS;i+=CHIP_FACTOR)
		{	r=reds[s];
			g=greens[s];
			b=blues[s];
			if(s<NPARTS-1)
			{	if(cursz>=criticdatasize[s]
				   && (isEmpty(s,cursz,NAND_PAGE_SIZE*NAND_PAGE_INCREMENT)
				      || (curpg>=pgsizes[s])))
				{	r=0xFF;
					g=0xFF;
					b=0xFF;
				}
			}
			r2=0xFF;
			b2=0;
			g2=0;
			setCurColorRGB(r,g,b);
			drawBufFullBox(offscreen,x0+i+1,y0+j+1,x0+i+CHIP_FACTOR-1,y0+j+CHIP_FACTOR-1);
			if(cursz<criticdatasize[s]) {
				if(curpg<CHIP_WIDTH*NAND_PAGE_INCREMENT/CHIP_FACTOR) {
					setCurColorRGB(r2,g2,b2);
					drwBufHoriz(offscreen,y0+j+1,x0+i+1,x0+i+CHIP_FACTOR-1);
					if(i>0 && curpg>0)
						setBufPixelRGB(offscreen,x0+i,y0+j+1,r2,g2,b2);
				}
				if(i>0 && curpg==CHIP_WIDTH*NAND_PAGE_INCREMENT/CHIP_FACTOR) {
					setCurColorRGB(r2,g2,b2);
					drwBufHoriz(offscreen,y0+j+1,x0+i+1,x0+i);
					setBufPixelRGB(offscreen,x0+i+1,y0+j,r2,g2,b2);
				}
				if(criticdatasize[s]-curpg*NAND_PAGE_SIZE<=CHIP_WIDTH*NAND_PAGE_INCREMENT*NAND_PAGE_SIZE/CHIP_FACTOR) {
					setCurColorRGB(r2,g2,b2);
					drwBufHoriz(offscreen,y0+j+CHIP_FACTOR-1,x0+i+1,x0+i+CHIP_FACTOR-1);
					if(i>0 && curpg>0)
						setBufPixelRGB(offscreen,x0+i,y0+j+CHIP_FACTOR-1,r2,g2,b2);
				}
				else if(i==0)
					setBufPixelRGB(offscreen,x0+i+1,y0+j+CHIP_FACTOR,r2,g2,b2);
				else if(i==CHIP_WIDTH-CHIP_FACTOR)
					setBufPixelRGB(offscreen,x0+i+CHIP_FACTOR-1,y0+j+CHIP_FACTOR,r2,g2,b2);
				if(i==0 || !curpg) {
					setCurColorRGB(r2,g2,b2);
					drwBufVert(offscreen,x0+i+1,y0+j+1,y0+j+CHIP_FACTOR-1);
				}
				if(i>=CHIP_WIDTH-CHIP_FACTOR || (curpg+NAND_PAGE_INCREMENT)*NAND_PAGE_SIZE>=criticdatasize[s]) {
					setCurColorRGB(r2,g2,b2);
					drwBufVert(offscreen,x0+i+CHIP_FACTOR-1,y0+j+1,y0+j+CHIP_FACTOR-1);
				}
			}
			k+=NAND_PAGE_INCREMENT;
			curpg+=NAND_PAGE_INCREMENT;
			while(k==newpgoffsets[s+1]) {
				s++;
				curpg=0;
			}
			cursz=curpg*NAND_PAGE_SIZE;
		}
}

int main()
{	char tmp[64];
	data[0]=malloc(FILES_OFFSET_OFFSET+4);
	int i;
	bc_read_nand(data[0],FILES_OFFSET_OFFSET+4,0,0,0,0);
	initScreen();
	startScreen();
	resetConsole();
	if(!memcmp(data[0]+PART_TABLE_OFFSET,PART_TABLE_ID,PART_TABLE_SIZE))
	{	pgoffsets[0]=0;
		pgoffsets[NPARTS-3]=DGSTA_PAGE_OFFSET;
		pgoffsets[NPARTS-2]=DGSTA_PAGE_OFFSET+DGSTA_PAGE_NUM;
		for(i=1;i<NPARTS;i++)
			if(offsets_offsets[i])
				pgoffsets[i]=*((long int*)(data[0]+offsets_offsets[i]))/NAND_PAGE_SIZE;
		pgoffsets[NPARTS]=NDEND_PAGE_OFFSET;
		free(data[0]);
		for(i=0;i<NPARTS;i++)
			pgsizes[i]=pgoffsets[i+1]-pgoffsets[i];
		for(i=0;i<NPARTS-1;i++)
		{	if(pgsizes[i])
			{	data[i]=malloc(pgsizes[i]*NAND_PAGE_SIZE);
				bc_read_nand(data[i],pgsizes[i]*NAND_PAGE_SIZE,pgoffsets[i]*NAND_PAGE_SIZE,0,0,0);
			}
		}
		int filestates[pgoffsets[5]/NAND_PAGE_INCREMENT];
		for(i=0;i<NPARTS;i++)
			criticdatasize[i]=min(getCriticDataSize(i),pgsizes[i]*NAND_PAGE_SIZE);
		for(i=0;i<NPARTS+1;i++)
			newpgoffsets[i]=pgoffsets[i];
		int m=0;
		while(m>=0)
		{	resetConsole();
			for(i=0;i<NPARTS;i++)
				newpgsizes[i]=newpgoffsets[i+1]-newpgoffsets[i];
			setCurColorRGB(0,0,0);
			drawBufFullBox(offscreen,0,0,SCREEN_WIDTH-1,SCREEN_HEIGHT-1);
			dispChip();
			memcpy(getScreen(),offscreen,SCREEN_WIDTH*SCREEN_HEIGHT*2);
			for(i=0;i<CHIP_HEIGHT/CHAR_HEIGHT+1;i++)
				displn("",0,1);
			char tmp[32];
			for(i=0;i<NPARTS;i++)
			{	if(i==0)
					setCurColorRGB(0xFF,0xFF,0xFF);
				else
					setCurColorRGB(reds[i],greens[i],blues[i]);
				if(m==0)
					switch(i) {
						case 0:
							disp("0) Terminate",0,1);
							break;
						case 1:
							disp("1) Relocate Boot2",0,1);
							break;
						case 2:
							disp("2) Relocate BootD",0,1);
							break;
						case 3:
							disp("3) Relocate Diags",0,1);
							break;
						case 6:
							disp("6) Relocate FileS [wipe data]",0,1);
				}
				if(i==0) setCurColorRGB(reds[i],greens[i],blues[i]);
				if(newpgoffsets[i+1]>newpgoffsets[i])
				{	if(m>0 && i==m) disp(" <- relocate ->",0,1);
					sprintf(tmp,"%s: %04x-%04x(%04x)",titles[i],newpgoffsets[i],newpgoffsets[i]+newpgsizes[i]-1,newpgsizes[i]);
					setCol(31);
					disp(tmp,0,1);
				}
				displn("",0,1);
			}
			if(m<1)
				while(any_key_pressed());
			while(!any_key_pressed());
			if(m==0)
			{	if(isKeyPressed(KEY_NSPIRE_1))
					m=1;
				if(isKeyPressed(KEY_NSPIRE_2))
					m=2;
				if(isKeyPressed(KEY_NSPIRE_3))
					m=3;
				if(isKeyPressed(KEY_NSPIRE_6))
					m=6;
				if(isKeyPressed(KEY_NSPIRE_0) || isKeyPressed(KEY_NSPIRE_ESC))
					m=-1;
			}
			else
			{	if(isKeyPressed(KEY_NSPIRE_PLUS) || isKeyPressed(KEY_NSPIRE_RIGHT) || isKeyPressed(KEY_NSPIRE_DOWN))
				{	if(newpgoffsets[m]<=newpgoffsets[m+1]-NAND_PAGE_INCREMENT && (newpgoffsets[m+1]-newpgoffsets[m]-NAND_PAGE_INCREMENT)*NAND_PAGE_SIZE>=criticdatasize[m])
						newpgoffsets[m]+=NAND_PAGE_INCREMENT;
				}
				if(isKeyPressed(KEY_NSPIRE_MINUS) || isKeyPressed(KEY_NSPIRE_LEFT) || isKeyPressed(KEY_NSPIRE_UP))
				{	if(newpgoffsets[m]>=newpgoffsets[m-1]+NAND_PAGE_INCREMENT && (newpgoffsets[m]-newpgoffsets[m-1]-NAND_PAGE_INCREMENT)*NAND_PAGE_SIZE>=criticdatasize[m-1])
						newpgoffsets[m]-=NAND_PAGE_INCREMENT;
				}
				if(isKeyPressed(KEY_NSPIRE_0) || isKeyPressed(KEY_NSPIRE_ESC))
					m=0;
			}
		}
		int ok=0;
		for(i=1;i<NPARTS;i++)
			if(newpgoffsets[i]!=pgoffsets[i])
			{	ok=1;
				//*((long int*)&(data[0][offsets_offsets[i]]))=newpgoffsets[i];
				if(offsets_offsets[i])
					writeWord(data[0]+offsets_offsets[i],newpgoffsets[i]*NAND_PAGE_SIZE);
			}
		if(ok)
		{	setCurColorRGB(0xFF,0xFF,0xFF);
			clrBuf(getScreen());
			resetConsole();
			displn("Do you want to apply your changes? (y/n)(this may permanently brick your Nspire)",1,1);
			waitKey();
			if(isKeyPressed(KEY_NSPIRE_Y))
			{	for(i=NPARTS-2;i>=0 && ok;i--)
					if(i==0 || newpgoffsets[i]!=pgoffsets[i])
					{	sprintf(tmp,"Erasing area %06x[%03x]-%06x[%03x]...",newpgoffsets[i]*NAND_PAGE_SIZE,newpgoffsets[i],newpgoffsets[i+1]*NAND_PAGE_SIZE,newpgoffsets[i+1]);
						displn(tmp,0,1);
						if(newpgoffsets[i]<newpgoffsets[i+1])
						{	if(!bc_nand_erase_range(newpgoffsets[i]*NAND_PAGE_SIZE,newpgoffsets[i+1]*NAND_PAGE_SIZE-1))
							{	sprintf(tmp,"Flashing %s...",titles[i]);
								displn(tmp,0,1);
								if(bc_write_nand(data[i],min((newpgoffsets[i+1]-newpgoffsets[i]),(pgoffsets[i+1]-pgoffsets[i]))*NAND_PAGE_SIZE,newpgoffsets[i]*NAND_PAGE_SIZE))
									ok=0;
							}
							else
								ok=0;
						}
					}
				if(!ok)
				{	displn("",0,1);
					displn("ERROR: Your Nspire may die permanently  on the next reboot.",1,1);
					displn("Plug it to an USB power source to prevent it from losing its charge & rebooting.DON'T TRY ANYTHING ON IT.",1,1);
					displn("Ask for assistance on a TI forum.",1,1);
				}
				waitKey(1);
			}
		}
		for(i=1;i<NPARTS-1;i++)
			free(data[i]);
	}
	else
	{	displn("ABORT: Your Manuf zone doesn't include aNAND partition table",1,1);
		waitKey(1);
	}
	stopScreen();
	free(data[0]);
}
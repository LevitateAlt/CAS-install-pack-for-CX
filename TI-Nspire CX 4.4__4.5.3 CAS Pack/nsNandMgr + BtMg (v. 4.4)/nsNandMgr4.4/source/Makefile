DEBUG = FALSE

GCC = nspire-gcc
AS  = nspire-as
GXX = nspire-g++
LD  = nspire-ld
GENZEHN = genzehn

GCCFLAGS = -Wall -W -marm
LDFLAGS =
ZEHNFLAGS = --240x320-support true

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
endif

EXE1 = nsNandMgr
EXE2 = BtMg
OBJS = $(patsubst %.c, %.o, $(shell find . -not -name s_screen.c -not -name $(EXE1).c -a -name \*.c))
OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))
OBJS += $(patsubst %.S, %.o, $(shell find . -name \*.S))
DISTDIR = .
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all: $(EXE1).tns $(EXE2).tns

$(EXE2).o: $(EXE1).c
	$(GCC) $(GCCFLAGS) -DMULTI -c $< -o $@

$(EXE1).o: $(EXE1).c
	$(GCC) $(GCCFLAGS) -c $< -o $@
	
s_screen2.o: s_screen.c
	$(GCC) $(GCCFLAGS) -DMULTI -c $< -o $@

s_screen1.o: s_screen.c
	$(GCC) $(GCCFLAGS) -c $< -o $@
	
%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

%.o: %.cpp
	$(GXX) $(GCCFLAGS) -c $< -o $@
	
%.o: %.S
	$(AS) -c $< -o $@

$(EXE1).elf: $(OBJS) $(EXE1).o s_screen1.o
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $@ $(LDFLAGS)

$(EXE2).elf: $(OBJS) $(EXE2).o s_screen2.o
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $@ $(LDFLAGS)

$(EXE1).tns: $(EXE1).elf
	$(GENZEHN) --input $^ --output $@.zehn $(ZEHNFLAGS) --name "$(EXE1)"
	make-prg $@.zehn $@
	rm $@.zehn

$(EXE2).tns: $(EXE2).elf
	$(GENZEHN) --input $^ --output $@.zehn $(ZEHNFLAGS) --name "$(EXE2)"
	make-prg $@.zehn $@
	rm $@.zehn

clean:
	rm -f $(OBJS) $(EXE1).o $(EXE2).o s_screen*.o $(DISTDIR)/$(EXE).tns $(DISTDIR)/$(EXE).elf $(DISTDIR)/$(EXE).zehn

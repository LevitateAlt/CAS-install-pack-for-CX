CFLAGS := -Os -I libfdt -mcpu=arm926ej-s -std=gnu11
LDFLAGS = -nostdlib -T ldscript

OBJS = $(patsubst %.c, %.o, $(shell find . -not -name main.c -not -name ndless_loader.c -a -name \*.c))

all: controlx.img.tns

ndless_loader:
	make -C ndless_loader

ndless_loader_cx_cas.bin: ndless_loader
ndless_loader_cx_num.bin: ndless_loader
ndless_loader_cx_cas_dvt.bin: ndless_loader
ndless_loader_cx_num_dvt.bin: ndless_loader
ndless_loader_cm_cas.bin: ndless_loader
ndless_loader_cm_num.bin: ndless_loader

main.o: main.c ndless_loader.c ndless_loader_cx_cas.bin ndless_loader_cx_cas_dvt.bin ndless_loader_cx_num.bin ndless_loader_cx_num_dvt.bin ndless_loader_cm_cas.bin ndless_loader_cm_num.bin
	arm-none-eabi-gcc -c $< -o $@ $(CFLAGS)

%.o: %.S
	arm-none-eabi-gcc -c $^ -o $@

%.o: %.c
	arm-none-eabi-gcc -c $^ -o $@ $(CFLAGS)

controlx.elf: main.o $(OBJS)
	arm-none-eabi-ld $(LDFLAGS) $^ -o $@
#	arm-none-eabi-gcc $^ -o $@ -Wl,-Tldscript,--gc-sections -nostdlib

controlx.img.tns: controlx.elf
	arm-none-eabi-objcopy -O binary $^ $@

clean:
	make -C ndless_loader clean
	rm -f controlx.img.tns controlx.elf $(OBJS) main.o

.PHONY: clean ndless_loader
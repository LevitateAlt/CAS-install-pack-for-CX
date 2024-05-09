CFLAGS := -Os -I libfdt -ffunction-sections

OBJS = $(patsubst %.c, %.o, $(shell find . -name \*.c))
OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))
OBJS += $(patsubst %.S, %.o, $(shell find . -name \*.S))

all: loader.bin

%.o: %.S
	arm-none-eabi-gcc -c $^ -o $@

%.o: %.c
	arm-none-eabi-gcc -c $^ -o $@ $(CFLAGS)

loader.elf: $(OBJS)
	arm-none-eabi-gcc $^ -o $@ -Wl,-Tldscript,--gc-sections -nostdlib -lc

loader.bin: loader.elf
	arm-none-eabi-objcopy -O binary $^ $@

clean:
	rm -f loader.bin loader.elf $(OBJS)

.PHONY: clean
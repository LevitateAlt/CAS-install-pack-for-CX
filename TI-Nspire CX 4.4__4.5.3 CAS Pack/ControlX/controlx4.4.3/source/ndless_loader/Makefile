GCC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
GCCFLAGS = -Os -Wall -W -marm -mcpu=arm926ej-s -s -fPIE -ffreestanding -std=c11
LDFLAGS = -nostdlib -nostartfiles -e ndless_loader -PIE -T ldscript
OBJCOPY := "$(shell which arm-elf-objcopy 2>/dev/null)"
ifeq (${OBJCOPY},"")
	OBJCOPY := arm-none-eabi-objcopy
endif
BINCXC = ../ndless_loader_cx_cas.bin
BINCXN = ../ndless_loader_cx_num.bin
BINCXCD = ../ndless_loader_cx_cas_dvt.bin
BINCXND = ../ndless_loader_cx_num_dvt.bin
BINCMC = ../ndless_loader_cm_cas.bin
BINCMN = ../ndless_loader_cm_num.bin

# WARNING : Boot2 4.0.3 payload current max size is 0x3D4.
# DO NOT include all OS patches !
PATCHES = -DOS_300_0 -DOS_300_1045 -DOS_300_1503 -DOS_310_0 -DOS_310_157 -DOS_310_236 -DOS_310_392 -DOS_320_776 -DOS_320_1030 -DOS_320_1180 -DOS_320_1212 -DOS_320_1219 -DOS_324 -DOS_330_218 -DOS_330_538 -DOS_360_427 -DOS_360_521 -DOS_360_546_550 -DOS_440_532
# list of OS patches :
#test 1: -DOS_300_0 -DOS_300_1045 -DOS_300_1503 -DOS_301 -DOS_302_1791 -DOS_302_1793 -DOS_310_0 -DOS_310_157 -DOS_310_236 -DOS_310_392 -DOS_320_776 -DOS_320_1030 -DOS_320_1180 -DOS_320_1212 -DOS_320_1219 -DOS_323 -DOS_324 -DOS_330_218 -DOS_330_538 -DOS_360_427 -DOS_360_521 -DOS_360_546_550
#test 2: -DOS_390_461 -DOS_390_463 -DOS_391 -DOS_400 -DOS_402
#test 3: -DOS_403 -DOS_42 -DOS_430_547 -DOS_430_702 -DOS_440_531
#test 4: -DOS_440_532
#prod : -DOS_300_0 -DOS_300_1045 -DOS_300_1503 -DOS_310_0 -DOS_310_157 -DOS_310_236 -DOS_310_392 -DOS_320_776 -DOS_320_1030 -DOS_320_1180 -DOS_320_1212 -DOS_320_1219 -DOS_324 -DOS_330_218 -DOS_330_538 -DOS_360_427 -DOS_360_521 -DOS_360_546_550 -DOS_440_532

# -DOS_300_0 -DOS_300_1045 -DOS_300_1503 -DOS_301 -DOS_302_1791 -DOS_302_1793
# -DOS_310_0 -DOS_310_157 -DOS_310_236 -DOS_310_392
# -DOS_320_776 -DOS_320_1030 -DOS_320_1180 -DOS_320_1212 -DOS_320_1219 -DOS_323 -DOS_324
# -DOS_330_218 -DOS_330_538
# -DOS_360_427 -DOS_360_521 -DOS_360_546_550
# -DOS_390_461 -DOS_390_463 -DOS_391
# -DOS_400 -DOS_402 -DOS_403
# -DOS_42
# -DOS_430_547 -DOS_430_702
# -DOS_440_531 -DOS_440_532

all: $(BINCXC) $(BINCXCD) $(BINCXN) $(BINCXND) $(BINCMC) $(BINCMN)

ndless_loader_cm_cas.o: ndless_loader.c
	$(GCC) $(GCCFLAGS) -o $@ -DCM -DCAS $(PATCHES) -c $<

ndless_loader_cm_num.o: ndless_loader.c
	$(GCC) $(GCCFLAGS) -o $@ -DCM $(PATCHES) -c $<
	
ndless_loader_cx_cas.o: ndless_loader.c
	$(GCC) $(GCCFLAGS) -o $@ -DCAS $(PATCHES) -c $<

ndless_loader_cx_cas_dvt.o: ndless_loader.c
	$(GCC) $(GCCFLAGS) -o $@ -DDVT -DCAS $(PATCHES) -c $<

ndless_loader_cx_num.o: ndless_loader.c
	$(GCC) $(GCCFLAGS) -o $@ $(PATCHES) -c $<

ndless_loader_cx_num_dvt.o: ndless_loader.c
	$(GCC) $(GCCFLAGS) -o $@ -DDVT $(PATCHES) -c $<

$(BINCXC): ndless_loader_cx_cas.o
	$(LD) $(LDFLAGS) $^ -o $(<:.o=.elf)
	$(OBJCOPY) -O binary $(<:.o=.elf) $@

$(BINCXCD): ndless_loader_cx_cas_dvt.o
	$(LD) $(LDFLAGS) $^ -o $(<:.o=.elf)
	$(OBJCOPY) -O binary $(<:.o=.elf) $@

$(BINCXN): ndless_loader_cx_num.o
	$(LD) $(LDFLAGS) $^ -o $(<:.o=.elf)
	$(OBJCOPY) -O binary $(<:.o=.elf) $@

$(BINCXND): ndless_loader_cx_num_dvt.o
	$(LD) $(LDFLAGS) $^ -o $(<:.o=.elf)
	$(OBJCOPY) -O binary $(<:.o=.elf) $@

$(BINCMC): ndless_loader_cm_cas.o
	$(LD) $(LDFLAGS) $^ -o $(<:.o=.elf)
	$(OBJCOPY) -O binary $(<:.o=.elf) $@
	
$(BINCMN): ndless_loader_cm_num.o
	$(LD) $(LDFLAGS) $^ -o $(<:.o=.elf)
	$(OBJCOPY) -O binary $(<:.o=.elf) $@
clean:
	rm -f *.o *.elf
	rm -f $(BINCXC) $(BINCXCD) $(BINCXN) $(BINCXND) $(BINCMC) $(BINCMN)
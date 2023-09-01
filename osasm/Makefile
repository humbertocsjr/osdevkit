include ../config.mk

EXES_Z80 = ../bin/osasmz80 ../bindos/osasmz80.exe ../binmsx/osasmz80.com ../binmsx2/osasmz80.com ../bincpm/osasmz80.com
EXES_80 = ../bin/osasm80 ../bindos/osasm80.exe ../binmsx/osasm80.com ../binmsx2/osasm80.com ../bincpm/osasm80.com

EXES = $(EXES_Z80) $(EXES_80)
SRC_Z80 = osasm.c osasmz80.c ../common/obj.c ../common/parse.c
SRC_80 = osasm.c osasm80.c ../common/obj.c ../common/parse.c

all: $(EXES)
	@echo > /dev/null

clean: 
	@-rm $(EXES) *.o
	
../bin/osasmz80: $(SRC_Z80)
	@$(CC) $(CFLAGS) $@ $^
	
../bincpm/osasmz80.com: $(SRC_Z80)
	@$(CC_CPM) $(CFLAGS_CPM) $@ $^
	
../binmsx/osasmz80.com: $(SRC_Z80)
	@$(CC_MSXDOS) $(CFLAGS_MSXDOS) $@ $^
	
../binmsx2/osasmz80.com: $(SRC_Z80)
	@$(CC_MSXDOS2) $(CFLAGS_MSXDOS2) $@ $^
	
../bindos/osasmz80.exe: $(SRC_Z80)
	@$(CC_MSDOS) $(CFLAGS_MSDOS) $@ $^
	
../bin/osasm80: $(SRC_80)
	@$(CC) $(CFLAGS) $@ $^
	
../bincpm/osasm80.com: $(SRC_80)
	@$(CC_CPM) $(CFLAGS_CPM) $@ $^
	
../binmsx/osasm80.com: $(SRC_80)
	@$(CC_MSXDOS) $(CFLAGS_MSXDOS) $@ $^
	
../binmsx2/osasm80.com: $(SRC_80)
	@$(CC_MSXDOS2) $(CFLAGS_MSXDOS2) $@ $^
	
../bindos/osasm80.exe: $(SRC_80)
	@$(CC_MSDOS) $(CFLAGS_MSDOS) $@ $^
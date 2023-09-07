include ../config.mk

EXES_0 = ../bin/oscc0 ../bindos/oscc0.exe ../binmsx/oscc0.com ../binmsx2/oscc0.com ../bincpm/oscc0.com
EXES_1 = ../bin/oscc1 ../bindos/oscc1.exe ../binmsx/oscc1.com ../binmsx2/oscc1.com ../bincpm/oscc1.com

EXES = $(EXES_0) $(EXES_1)
SRC_0 = oscc.c  oscc0.c ../common/ui32.c ../common/ui64.c ../common/obj.c ../common/parse.c
SRC_1 = oscc.c  oscc1.c ../common/ui32.c ../common/ui64.c ../common/obj.c ../common/parse.c

all: $(EXES)
	@echo > /dev/null

clean: 
	@-rm $(EXES) *.o
	
../bin/oscc0: $(SRC_0)
	@$(CC) $(CFLAGS) $@ $^
	
../bincpm/oscc0.com: $(SRC_0)
	@$(CC_CPM) $(CFLAGS_CPM) $@ $^
	
../binmsx/oscc0.com: $(SRC_0)
	@$(CC_MSXDOS) $(CFLAGS_MSXDOS) $@ $^
	
../binmsx2/oscc0.com: $(SRC_0)
	@$(CC_MSXDOS2) $(CFLAGS_MSXDOS2) $@ $^
	
../bindos/oscc0.exe: $(SRC_0)
	@$(CC_MSDOS) $(CFLAGS_MSDOS) $@ $^
	
../bin/oscc1: $(SRC_1)
	@$(CC) $(CFLAGS) $@ $^
	
../bincpm/oscc1.com: $(SRC_1)
	@$(CC_CPM) $(CFLAGS_CPM) $@ $^
	
../binmsx/oscc1.com: $(SRC_1)
	@$(CC_MSXDOS) $(CFLAGS_MSXDOS) $@ $^
	
../binmsx2/oscc1.com: $(SRC_1)
	@$(CC_MSXDOS2) $(CFLAGS_MSXDOS2) $@ $^
	
../bindos/oscc1.exe: $(SRC_1)
	@$(CC_MSDOS) $(CFLAGS_MSDOS) $@ $^
	
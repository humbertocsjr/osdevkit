include ../config.mk

EXES_0 = ../bin/oscc0 ../bindos/oscc0.exe ../binmsx/oscc0.com ../binmsx2/oscc0.com ../bincpm/oscc0.com

EXES = $(EXES_0)
SRC_0 = oscc.c  oscc0.c ../common/ui32.c ../common/ui64.c ../common/obj.c ../common/parse.c

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
	
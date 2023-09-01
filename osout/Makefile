include ../config.mk

EXES_BIN = ../bin/osoutbin ../bindos/osoutbin.exe ../binmsx/osoutbin.com ../binmsx2/osoutbin.com ../bincpm/osoutbin.com
EXES_COM = ../bin/osoutcom ../bindos/osoutcom.exe ../binmsx/osoutcom.com ../binmsx2/osoutcom.com ../bincpm/osoutcom.com
EXES_PCB = ../bin/osoutpcb ../bindos/osoutpcb.exe ../binmsx/osoutpcb.com ../binmsx2/osoutpcb.com ../bincpm/osoutpcb.com
EXES = $(EXES_BIN) $(EXES_COM) $(EXES_PCB)
SRC_BIN = osoutbin.c  osout.c ../common/obj.c ../common/parse.c
SRC_COM = osoutcom.c  osout.c ../common/obj.c ../common/parse.c
SRC_PCB = osoutpcb.c  osout.c ../common/obj.c ../common/parse.c

all: $(EXES)
	@echo > /dev/null

clean: 
	@-rm $(EXES) *.o
	
../bin/osoutbin: $(SRC_BIN)
	@$(CC) $(CFLAGS) $@ $^
	
../bincpm/osoutbin.com: $(SRC_BIN)
	@$(CC_CPM) $(CFLAGS_CPM) $@ $^
	
../binmsx/osoutbin.com: $(SRC_BIN)
	@$(CC_MSXDOS) $(CFLAGS_MSXDOS) $@ $^
	
../binmsx2/osoutbin.com: $(SRC_BIN)
	@$(CC_MSXDOS2) $(CFLAGS_MSXDOS2) $@ $^
	
../bindos/osoutbin.exe: $(SRC_BIN)
	@$(CC_MSDOS) $(CFLAGS_MSDOS) $@ $^
	
../bin/osoutcom: $(SRC_COM)
	@$(CC) $(CFLAGS) $@ $^
	
../bincpm/osoutcom.com: $(SRC_COM)
	@$(CC_CPM) $(CFLAGS_CPM) $@ $^
	
../binmsx/osoutcom.com: $(SRC_COM)
	@$(CC_MSXDOS) $(CFLAGS_MSXDOS) $@ $^
	
../binmsx2/osoutcom.com: $(SRC_COM)
	@$(CC_MSXDOS2) $(CFLAGS_MSXDOS2) $@ $^
	
../bindos/osoutcom.exe: $(SRC_COM)
	@$(CC_MSDOS) $(CFLAGS_MSDOS) $@ $^
	
../bin/osoutpcb: $(SRC_PCB)
	@$(CC) $(CFLAGS) $@ $^
	
../bincpm/osoutpcb.com: $(SRC_PCB)
	@$(CC_CPM) $(CFLAGS_CPM) $@ $^
	
../binmsx/osoutpcb.com: $(SRC_PCB)
	@$(CC_MSXDOS) $(CFLAGS_MSXDOS) $@ $^
	
../binmsx2/osoutpcb.com: $(SRC_PCB)
	@$(CC_MSXDOS2) $(CFLAGS_MSXDOS2) $@ $^
	
../bindos/osoutpcb.exe: $(SRC_PCB)
	@$(CC_MSDOS) $(CFLAGS_MSDOS) $@ $^
	
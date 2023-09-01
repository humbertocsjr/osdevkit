include ../config.mk

EXES = ../bin/oslink ../bindos/oslink.exe ../binmsx/oslink.com ../binmsx2/oslink.com ../bincpm/oslink.com
SRC = oslink.c ../common/obj.c ../common/parse.c

all: $(EXES)
	@echo > /dev/null

clean: 
	@-rm $(EXES) *.o
	
../bin/oslink: $(SRC)
	@$(CC) $(CFLAGS) $@ $^
	
../bincpm/oslink.com: $(SRC)
	@$(CC_CPM) $(CFLAGS_CPM) $@ $^
	
../binmsx/oslink.com: $(SRC)
	@$(CC_MSXDOS) $(CFLAGS_MSXDOS) $@ $^
	
../binmsx2/oslink.com: $(SRC)
	@$(CC_MSXDOS2) $(CFLAGS_MSXDOS2) $@ $^
	
../bindos/oslink.exe: $(SRC)
	@$(CC_MSDOS) $(CFLAGS_MSDOS) $@ $^
	
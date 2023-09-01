CC = gcc
CFLAGS = -I../common -o
CC_CPM = z88dk.zcc
CFLAGS_CPM = +cpm -clib=8080 -I../common -lm -o
CC_MSXDOS = z88dk.zcc
CFLAGS_MSXDOS = +msx -I../common -subtype=msxdos -o
CC_MSXDOS2 = z88dk.zcc
CFLAGS_MSXDOS2 = +msx -I../common -subtype=msxdos2 -o
CC_MSDOS = smlrcc
CFLAGS_MSDOS =  -doss -8086 -I../common -o
MAKE = make --no-print-directory

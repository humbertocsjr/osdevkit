Old-School DevKit
=================

This development kit is focused on retrocomputers, running natively on target machines as well as on modern computers.

Tools included
--------------

- OSASMZ80 \
    Old-School Assembler for Z80 (Beta)
- OSASM80 \
    Old-School Assembler for i80 (Beta)
- OSASM86 \
    Old-School Assembler for i86 (In planning)
- OSLINK \
    Old-School Multiplatform Linker (Pre-release)
- OSOUTBIN \
    Old-School Generic Binary Generator (Beta)
- OSOUTCOM \
    Old-School COM Executable Generator (Beta)
- OSOUTEXE \
    Old-School EXE Executable Generator (In planning)
- OSOUTPCB \
    Old-School PC Boot Sector Generator (Beta)
- OSOUTUZI \
    Old-School UZI Executable Generator (In planning)
- OSOUTUZX \
    Old-School UZIX Executable Generator (In planning)
- OSOUTMNX \
    Old-School Minix Executable Generator (In planning)
- OSBASZ80 \
    Old-School BASIC Compiler for Z80 (In planning)
- OSBAS80 \
    Old-School BASIC Compiler for i80 (In planning)
- OSBAS86 \
    Old-School BASIC Compiler for i86 (In planning)

File extensions
---------------

- .s: Old-School Assembler Source File
- .bas: Old-School BASIC Source File
- .obj: Old-School Generic Object File
- .lnk: Old-School Linked Object File

Tool Usage Steps
----------------

```
OSBASxxx --> OSASMxxx -\
             OSASMxxx --+--> OSLINK --> OSOUTxxx
             OSASMxxx -/
```

- OSASMxxx Gererates Object files from source files
- OSLINK Links Object files in one Linker Object File
- OSOUTxxx Converts Linker Object file to final executable

Test Machines
-------------

- 8080/Z80: \
    openMSX Emulator: Gradient Expert Plus + DDX 3.0 Floppy Cartridge

- 8086: \
    PC Pentium 100 with MS-DOS 6.22

Minimum Requirements
--------------------

**MS-DOS Distributions**

MS-DOS 3.0 or compatible \
192 KiB RAM \
360 KiB Floppy Disk

**CP/M or MSX-DOS**

48 KiB TPA (64 KiB RAM)
Floppy Disk

Distros
-------

- distro/msxdos1.dsk: \
    MSX-DOS 1 Distribution Disk (**Bootable**)
- distro/msxdos1alt.dsk: \
    CP/M i80 Distribution Disk for MSX-DOS (**Bootable**)
- distro/msxdos2.dsk: \
    MSX-DOS 2 Distribution Disk
- distro/msdosXXXX.img: \
    MS-DOS Distribution Disk

Minimum Requirements for Development
------------------------------------

- NASM
- (Smaller-C with 8086 target)[github.com/humbertocsjr/SmallerC]
- Z88dk
- GCC
- MTools

Ubuntu Development Env Install
------------------------------

Preparation script:

```sh
sudo apt install nasm build-essential mtools
sudo snap install --beta z88dk
git clone https://github.com/humbertocsjr/SmallerC.git
cd SmallerC
chmod +x ./configure
./configure
make 
sudo make install
cd ..
rm -R SmallerC
```

Old-School Generic Object File Format
=====================================

All object files are composed of records, all following a single pattern described below:

| Pos | Size | Description |
|-----|------|-------------|
| 000 | 0001 | Reg Type    |
| 001 | 0001 | Data Size   |
| 002 | ???? | Data        |

The data field can be up to 255 bytes long.

**TODO: Add Reg Type List**

Old-School Assembler
====================

This is a generic assembler framework, supporting multiple 8/16bit processors.

Common commands
---------------

- proc [PROCEDURE NAME] \
    This command mark the start of a internal procedure/function, is used by the linker to filter used functions from unused ones in the final executable.
- endp \
    This command mark the end of a procedure/function.
- use [PROCEDURE NAME] \
    This command informs the linker which functions will be used by current function, not being necessary in the case of directly using the CALL command (It varies in each processor)
- cpu [CPU model] \
    This command informs the target cpu of the source code.
- sector [CODE / DATA / BSS]
    This command tells which sector of the executable the next commands belong to.
- db [value1, value2, value3....]
    Output bytes directly to the final executable.
- dw [value1, value2, value3....]
    Output words (2 bytes) directly to the final executable.

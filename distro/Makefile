distro_zip: msxdos1.zip msxdos2.zip msdos.zip cpm.zip
	@echo >> /dev/null
distro_img: msxdos1.dsk msxdos1alt.dsk msxdos2.dsk msdos360.img msdos720.img msdos1440.img
	@echo >> /dev/null

distro: distro_zip distro_img
	@echo >> /dev/null

msdos.zip: $(wildcard ../bindos/*) $(wildcard test/*.s)
	@zip -jq $@ $^

msxdos1.zip: $(wildcard ../binmsx/*) $(wildcard test/*.s)
	@zip -jq $@ $^

msxdos2.zip: $(wildcard ../binmsx2/*) $(wildcard test/*.s)
	@zip -jq $@ $^

cpm.zip: $(wildcard ../bincpm/*) $(wildcard test/*.s)
	@zip -jq $@ $^

msxdos1.dsk: $(wildcard ../binmsx/*) $(wildcard test/*)
	@dd if=/dev/zero of=$@ bs=1024 count=720  status=none
	@mformat -f 720 -i $@
	@dd if=../external/msxbin/BOOT of=$@ bs=512 count=1 conv=notrunc status=none
	@mcopy -i $@ ../external/msxbin/MSXDOS.SYS ::/MSXDOS.SYS
	@mcopy -i $@ ../external/msxbin/COMMAND.COM ::/COMMAND.COM
	@mcopy -i $@ ../binmsx/* ::/
	@mcopy -i $@ ../test/*.s ::/

msxdos1alt.dsk: $(wildcard ../bincpm/*) $(wildcard test/*)
	@dd if=/dev/zero of=$@ bs=1024 count=720  status=none
	@mformat -f 720 -i $@
	@dd if=../external/msxbin/BOOT of=$@ bs=512 count=1 conv=notrunc status=none
	@mcopy -i $@ ../external/msxbin/MSXDOS.SYS ::/MSXDOS.SYS
	@mcopy -i $@ ../external/msxbin/COMMAND.COM ::/COMMAND.COM
	@mcopy -i $@ ../bincpm/* ::/
	@mcopy -i $@ ../test/*.s ::/

msxdos2.dsk: $(wildcard ../binmsx2/*) $(wildcard ../test/*)
	@dd if=/dev/zero of=$@ bs=1024 count=720  status=none
	@mformat -f 720 -i $@
	@dd if=../external/msx2bin/BOOT2 of=$@ bs=512 count=1 conv=notrunc status=none
	@mcopy -i $@ ../external/msx2bin/MSXDOS.SYS ::/MSXDOS.SYS
	@mcopy -i $@ ../external/msx2bin/NEXTOR.SYS ::/NEXTOR.SYS
	@mcopy -i $@ ../external/msx2bin/COMMAND.COM ::/COMMAND.COM
	@mcopy -i $@ ../external/msx2bin/COMMAND2.COM ::/COMMAND2.COM
	@mcopy -i $@ ../binmsx2/* ::/
	@mcopy -i $@ ../test/*.s ::/


msdos360.img: $(wildcard ../bindos/*) $(wildcard ../test/*)
	@dd if=/dev/zero of=$@ bs=1024 count=360  status=none
	@mformat -f 360 -i $@
	@mcopy -i $@ ../bindos/* ::/
	@mcopy -i $@ ../test/*.s ::/


msdos720.img: $(wildcard ../bindos/*) $(wildcard ../test/*)
	@dd if=/dev/zero of=$@ bs=1024 count=720  status=none
	@mformat -f 720 -i $@
	@mcopy -i $@ ../bindos/* ::/
	@mcopy -i $@ ../test/*.s ::/

msdos1440.img: $(wildcard ../bindos/*) $(wildcard ../test/*)
	@dd if=/dev/zero of=$@ bs=1024 count=1440  status=none
	@mformat -f 1440 -i $@
	@mcopy -i $@ ../bindos/* ::/
	@mcopy -i $@ ../test/*.s ::/
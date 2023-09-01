include config.mk

defaults: all

all clean install:  bin bindos bincpm binmsx binmsx2
	@(cd osasm; $(MAKE) $@);
	@(cd oslink; $(MAKE) $@);

distro: all
	@-rm distro/*.img distro/*.dsk
	@(cd distro; $(MAKE) $@)

test: all
	@(cd test; $(MAKE) $@);

bin:
	@mkdir bin

bincpm:
	@mkdir bincpm

bindos:
	@mkdir bindos

binmsx:
	@mkdir binmsx

binmsx2:
	@mkdir binmsx2
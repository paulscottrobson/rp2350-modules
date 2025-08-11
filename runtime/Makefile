include ../environment/system.make

#
#		This is the address of the app you want to run. Note, this can be relative to ${ROOTDIR} ${MODULEDIR} or anywhere.
#
APPLICATION = ${ROOTDIR}apps/bully

compile: 
	cd build ; make -j10

cmake:
	mkdir -p build
	rm -f build/CMakeCache.txt 
	cd build ; cmake -DMODULEDIR=$(MODULEDIR) -DROOTDIR=$(ROOTDIR) -DAPPDIR=${APPLICATION} ..

run: compile
	build/runtime

rebuilddvi:
	make -C ../modules/dvi compile upload
rebuildusb:
	make -C ../modules/usb compile upload	
include ../environment/system.make

compile: 
	cd build ; make -j10
cmake:
	mkdir -p build
	rm -f build/CMakeCache.txt 
	cd build ; cmake -DMODULEDIR=$(MODULEDIR) ..

run: compile
	build/runtime

rebuilddvi:
	make -C ../modules/dvi compile upload
rebuildusb:
	make -C ../modules/usb compile upload	
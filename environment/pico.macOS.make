# *******************************************************************************************
# *******************************************************************************************
#
#       Name :      pico.macosx.make
#       Purpose :   Pic makefile
#       Date :      5th July 2025
#		Author :    Paul Robson (paul@robsons.org.uk)
#       Modified by : Giovanni Pozzobon (giovanni.pozzobon@gmail.com)
#
# *******************************************************************************************
# *******************************************************************************************

# *******************************************************************************************
#
#									Target ARM or RISCV
#
# *******************************************************************************************

TARGET=ARM
#TARGET=RISCV


# *******************************************************************************************
#
#						Location of Pico SDK (if not set by default)
#
# *******************************************************************************************

export PICO_SDK_PATH=/Users/giovanni.pozzobon/Documents/pico/pico-sdk

# *******************************************************************************************
#
#				Select one or the other.  Note: if you switch cores, you will have
#				to upload a working core via UF2/Boot before the debug connection 
#				will work.
#
# *******************************************************************************************

ifeq ($(TARGET),ARM)
#
# 		Platform selection. (ARM)
#
PLATFORM = rp2350
else
#
#		Platform selection (RISCV)
#
PLATFORM = rp2350-riscv
export PICO_TOOLCHAIN_PATH=/Users/giovanni.pozzobon/Documents/pico/corev-openhw-gcc-macos-20240530
export PICO_RISCV_TOOLCHAIN_PATH=/Users/giovanni.pozzobon/Documents/pico/corev-openhw-gcc-macos-20240530
endif

#
#		Serial debugging port.
#
PICO_SERIAL_PORT = /dev/tty.usbmodem2102
PICO_SERIAL_BAUD_RATE = 115200
#
#		Debug upload
#
UPLOADER = openocd 
UPCONFIG = -f interface/cmsis-dap.cfg -f target/$(PLATFORM).cfg 
UPCOMMANDS = -c "adapter speed 5000" -c "program build/$(APPNAME).elf verify reset exit"

compile: 
	cd build ; make -j10

cmake:
	cp $(PICO_SDK_PATH)/external/pico_sdk_import.cmake .
	mkdir -p build
	rm -f build/CMakeCache.txt 
	rm -rf build/CMakeFiles build/generated build/pico-sdk build/pioasm build/pioasm-install
	rm -f build/CMakeDoxy* build/pico_flash*
	cd build ; cmake -DPICO_PLATFORM=$(PLATFORM) -DMODULEDIR=$(MODULEDIR) -DDVIDIR=${ROOTDIR}picodvi/software/libdvi -DROOTDIR=$(ROOTDIR) ..


upload: compile
	$(UPLOADER) $(UPCONFIG) $(UPCOMMANDS)

serial:
	picocom -b $(PICO_SERIAL_BAUD_RATE) $(PICO_SERIAL_PORT)

all: compile upload serial

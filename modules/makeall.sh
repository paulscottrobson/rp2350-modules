#
#       Check everything builds.
#
make -C dvi cmake comp
make -C graphics cmake compile
make -C input cmake compile
make -C machine cmake compile
make -C memory cmake compile
make -C psram cmake compile
make -C screeneditor cmake compile
make -C usb cmake compile

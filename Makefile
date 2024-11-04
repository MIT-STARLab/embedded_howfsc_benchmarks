# Top-Level Makefile
.PHONY: local local_clean T1040 T1040_clean

local:
	echo "Building the local target"
	make -f Makefile.local

local_clean:
	echo "Cleaning the local target"
	make -f Makefile.local clean

T1040:
	echo "Building for T1040"
	make -f Makefile.T1040

T1040_clean:
	echo "Cleaning for T1040"
	make -f Makefile.T1040 clean

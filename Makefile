##############################################################
#               CMake Project Wrapper Makefile               #
############################################################## 

SHELL := /bin/sh
RM    := rm -rf

all: ./build/Makefile
	@ $(MAKE) -C build

./build/Makefile:
	@ (cd build >/dev/null 2>&1 && cmake ..)

distclean:
	@- (cd build >/dev/null 2>&1 && cmake .. >/dev/null 2>&1)
	@- $(MAKE) --silent -C build clean || true
	@- $(RM) ./info.log
	@- $(RM) ./errors.log
	@- $(RM) ./build/Makefile
	@- $(RM) ./build/doc
	@- $(RM) ./build/examples
	@- $(RM) ./build/include
	@- $(RM) ./build/src
	@- $(RM) ./build/test
	@- $(RM) ./build/Testing
	@- $(RM) ./build/CMake*
	@- $(RM) ./build/cmake.*
	@- $(RM) ./build/*.cmake
	@- $(RM) ./build/*.tcl
	@- $(RM) ./build/*.txt
	@- $(RM) ./cmake/CMakeFiles
	@- $(RM) ./doc/api
	@- $(RM) ./doc/doxyfile
	@- $(RM) ./include/teuthid/config.hpp
	@- $(RM) ./README
	
ifeq ($(findstring distclean,$(MAKECMDGOALS)),)

    $(MAKECMDGOALS): ./build/Makefile
	@ $(MAKE) -C build $(MAKECMDGOALS)

endif

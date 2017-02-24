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
	@- $(RM) ./cl_info.log
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
	@- $(RM) ./build/*.txt
	
ifeq ($(findstring distclean,$(MAKECMDGOALS)),)

    $(MAKECMDGOALS): ./build/Makefile
	@ $(MAKE) -C build $(MAKECMDGOALS)

endif

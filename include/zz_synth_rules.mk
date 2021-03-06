#=============================================================
# FILE:
#   inclue/zz_synth_rules.mk 
#
# AUTHOR:
#   zimzum@github 
#
# DESCRIPTION:
#   Implicit (and directly related variables) rules used in the
#   build system for the zz_synth library. Include this file
#	after the definition of source_files. Otherwise the implict
#	rules will be expanded with object_files, which depend on
#	source_files, yet undefined.
#
#	When building targets pass COMPILE_FLAGS=PEDANTIC option to
#	turn on very strict compiler checks.
#
# License: GNU GPL v2.0 
#=============================================================
#-----------------------
#  COMPILATION VARIABLES 
#-----------------------
CPPFLAGS = $(OPTFLAG) -std=c++11 -lstdc++
#CXX      = g++ -fdiagnostics-show-option
CXX      = clang

ifeq "$(CXX)" "clang"
  include $(ZZINC)/compiler_flags_clang.mk
else
  include $(ZZINC)/compiler_flags_gcc.mk
endif

ifeq "$(COMPILE_FLAGS)" "PEDANTIC"
  CPPFLAGS += $(CPPFLAGS_PEDANTIC)
  $(info Pedantic compile flags...)
else ifeq "$(COMPILE_FLAGS)" "DEBUG"
  CPPFLAGS += $(CPPFLAGS_DEBUG)
  $(info Debuggin compile flags...)
endif

INCLUDES    = -I $(ZZINC) -I /usr/lib/x86_64-linux-gnu \
			  -I $(ZZDIR)/unit_tests/googletest/googletest/include \
			  -I $(ZZDIR)/unit_tests/googletest/googlemock/include
COMPILE.cpp = $(CXX) $(CFLAGS) $(INCLUDES) $(CPPFLAGS) $(TARGET_ARCH) -c

LIBS = -lm -lstdc++

# The following are extra include flags needed by the Google Unit Testing
# framework.
LIBS_UNIT_TESTS = -L $(ZZDIR)/unit_tests/googletest/googletest/make/ -lgtest -lpthread

#-----------------------
#  SHELL COMMANDS 
#-----------------------
RANLIB  = ranlib
AR      = ar rv
RM      = rm -rf
MKDIR   = mkdir -p

#-----------------------
#  IMPLICIT RULES 
#-----------------------
%.o: %.cc
	@echo Compiling $<
	$(COMPILE.cpp) $<

$(ZZBIN)/%.o: %.cc
	@echo Compiling $<
	$(COMPILE.cpp) $< -o $@

$(library): $(object_files)
	@echo $<
	@echo $(source_files)
	$(AR) $@ $^
	@$(RANLIB) $@

$(testbench): $(object_files) $(libraries)
	@echo $(LINK.cc)
	$(LINK.cc) $(LIBS) $^ $(LIBS_UNIT_TESTS) -o $@
	-rm $(object_files)

$(example): $(object_files) $(libraries)
	@echo ---------------------------------------
	@echo --==LIBRARIES==--
	@echo $(libraries)
	@echo ---------------------------------------
	$(LINK.cc) $(LIBS_UNIT_TESTS) $(LIBS) $^ -o $@
	-rm $(object_files)

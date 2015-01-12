#=============================================================
# FILE:
#   Makefile
#
# AUTHOR:
#   zimzum@github 
#
# DESCRIPTION:
#   The master Makefile
#
# License: GNU GPL v2.0 
#=============================================================
ZZDIR = ~/github/zz_synth
include include/zz_synth.mk

lib_common  := src/common
libraries   := $(lib_common)
lib_common_bin := $(lib_common)/libcommon.a

unit_test_rww := unit_tests/read_write_wav

.PHONY:				\
	all				\
	$(libraries)	\
	$(lib_common)	\
	TAGS			\
	clean			\
	$(unit_test_rww)

#-----------------------
#  TARGETS 
#-----------------------
#clean:
	#@printf "#\n# [Make:] Root directory, nothing to clean.\n#\n"

$(lib_common):
	@$(build-msg)
	$(MAKE) --directory=$@ $(TARGET)
	$(if $(TARGET), $(MAKE) $(TARGET))

$(unit_test_rww): $(libraries)
	@$(build-msg)
	$(MAKE) --directory=$@ $(TARGET)
	$(if $(TARGET), $(MAKE) $(TARGET))

$(lib_common_bin): $(lib_common)

TAGS: 
	@$(build-msg)
	@find . -type f -name "*.h" -or -name "*.cc" > files_ctags
	@ctags -L files_ctags --language-force=C++ --C++-kinds=+pxf > tags

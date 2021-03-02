######################################################################
#
#                       Author: Hannah Pan
#                       Date:   02/25/2021
#
######################################################################

CC = clang
CFLAGS = -Wall -Werror -O3
CPPFLAGS = -DNDEBUG

PROG = penn-mmu
MAIN = framework/main.c
SUBDIRS = framework submission tests
OBJS = $(addsuffix /*.o, $(SUBDIRS))

.PHONY : $(SUBDIRS) clean

$(PROG) : $(SUBDIRS)
	$(CC) -lm -o $@ $(MAIN) $(OBJS) && mkdir -p logs/perf temp

$(SUBDIRS) :
	$(MAKE) -C $@

clean :
	$(RM) $(OBJS) $(PROG) *ation_*ed_Tests.txt
	rm -rf logs temp .pytest_cache/ __pycache__/

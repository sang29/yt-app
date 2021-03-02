CC = clang
CFLAGS = -Wall -Werror -O3
CPPFLAGS = -DNDEBUG

OBJS = CuTest.o test_allocation_subroutines.o test_common_subroutines.o \
	test_framework.o test_translation_subroutines.o

.PHONY : all clean

all : $(OBJS)

clean :
	$(RM) $(OBJS)

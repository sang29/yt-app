######################################################################
#
#                       Author: Hannah Pan
#                       Date:   02/25/2021
#
######################################################################

CC = clang

# Replace -O3 with -g for a debug version during development
CFLAGS = -Wall -Werror -O3

# Comment out the line below during development if assert(3) is used
CPPFLAGS = -DNDEBUG

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.PHONY : all clean

all : $(OBJS)

clean :
	$(RM) $(OBJS)

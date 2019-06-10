# @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2008-05-28 07:34:59 $
#  @version 1.0
#  @note Editor: Vim 6.1, Gcc 4.0.2, tab=4
#  @note Platform: Linux

include ../../tsf4g.mk

PCRE_CFILE=$(wildcard *.c)
PCRE_OBJ=$(PCRE_CFILE:.c=.o)
PCRE_LIB=$(TSF4G_LIB)/libpcre.a
PCRE_SO=$(TSF4G_LIB)/libpcre.so

.PHONY: all clean

all: $(PCRE_LIB) $(PCRE_SO)

-include $(PCRE_CFILE:.c=.d)

%.d: %.c
	$(CC) -MM $(CFLAGS) $< -o $@

%.o: %.c %.d
	$(CC) -c $(CFLAGS) $(DEBUG) $<

$(PCRE_LIB):	$(PCRE_OBJ)
	ar r $(PCRE_LIB) $^

$(PCRE_SO): $(PCRE_OBJ)
	$(CC) -o $(PCRE_SO) $(SHARED) $(CFLAGS) $(DEBUG) $^

clean:
	$(RM) *.o  *.d
	$(RM) $(PCRE_LIB)


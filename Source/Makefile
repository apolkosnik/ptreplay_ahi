# $Id: Makefile,v 1.8 2009/02/24 00:10:11 itix Exp $

ROOTPATH=./
include $(ROOTPATH)Makefile.global

CFLAGS+=-Iinclude -DAROS_ALMOST_COMPATIBLE -O3 -finline-functions -mcpu=750 -mmultiple -mstring

include makefile.objs

CATALOGS =
TARGET = ptreplay.library.elf

CATCOMP    = catcomp.pl

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINKECHO)
	$(LINKPREFIX)$(CC) -noixemul -nostartfiles $(OBJS) -o $@.db
	$(LINKPREFIX)$(STRIP) $(STRIPFLAGS) $@.db -o $@
	$(LINKPREFIX)chmod u+x $@

clean:
	rm -f $(TARGET) $(TARGET).db $(TARGET).s $(OBJS)

dump:
	objdump --disassemble-all --reloc $(TARGET).db >$(TARGET).s

install: all
	cp $(TARGET) /Libs/ptreplay.library
	flushlib $(TARGET) QUIET

dep:
	echo >Makefile.dep
	$(CC) -MM $(CFLAGS) `ls *.c` >>Makefile.dep

include Makefile.dep

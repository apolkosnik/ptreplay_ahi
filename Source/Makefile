# Project files

###########################################################
# VBCC
###########################################################

CC			=	/opt/amiga-cc/vbcc/bin/vc
CDEFS		=	+aos68k -DAROS_ALMOST_COMPATIBLE
#CDEFS		=	-DAROS_ALMOST_COMPATIBLE
CWARN		=	
CFLAGS	=	$(CDEFS) $(CWARN) +aos68k -O3 -c -c99 -schedule -maxoptpasses=40 -sc -cpu=68020 -I/opt/amiga-cc/vbcc/targets/m68k-amigaos -I/opt/amiga-cc/vbcc/targets/m68k-amigaos/include -I/usr/local/include -I/usr/include -Iinclude -D__VBCC__ -DAMIGA -I/opt/amiga-cc/vbcc/targets/m68k-amigaos/include  -k -size -inline-size=20 -unroll-size=20 -schedule
LDFLAGS	=  -nostdlib -final
TARGET	=	ptreplay.library

ECHO = echo
ECHE = echo -e
BOLD = \033[1m
NRML = \033[22m

COMPILING = @$(ECHE) "compiling $(BOLD)$@$(NRML)..."
LINKING = @$(ECHE) "linking $(BOLD)$@$(NRML)..."
STRIPPING = @$(ECHE) "stripping $(BOLD)$@$(NRML)..."
ARCHIVING = @$(ECHE) "archiving $(BOLD)$@$(NRML)..."

%.o: %.c
	$(COMPILING)
	@$(CC) $(CFLAGS) -o $@ $*.c

all	: $(TARGET)

-include makefile.objs
-include makefile.dep

###########################################################
# VBCC OS2
###########################################################

$(TARGET): $(OBJS)
		$(LINKING)
		@vc +aos68k -O3 $(LDFLAGS) -o $@ $(OBJS) -lzlib -ljpeg -lamiga -lvc

		#@vc +aos68k -O3 $(LDFLAGS) -o $@ $(OBJS) -lzlib -lamiga -lvc

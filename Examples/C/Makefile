#
#$VER: PTReplay Examples V2.1 (28.12.94)
#

OBJS68K	=	$(addprefix $(ODIR68K),$(OBJECTS))
OBJSGCC	=	$(addprefix $(ODIRGCC),$(OBJECTS))


###########################################################
# VBCC
###########################################################
CC			=	$(VBCC)/bin/vc
CDEFS		=	+aos68k -DAROS_ALMOST_COMPATIBLE

CWARN		=	
CFLAGS	=	$(CDEFS) $(CWARN) -D__VBCC__ -DAMIGA -DAMIGAOS +aos68k -O3 -c -c99 -schedule -maxoptpasses=40 -cpu=68020 -I$VBCC/targets/m68k-amigaos/include -Iinclude -k -inline-size=40 -unroll-size=40

LDFLAGS	=	+aos68k -lauto -lamiga -nostartfiles -final 
TARGET	=	PT-Player SmallPlay

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

all : $(TARGET)

###########################################################
# Dependencies and Objects
###########################################################
-include makefile.objs
-include makefile.dep

clean :
		rm *.asm
		rm *.o

###########################################################
# VBCC OS3
###########################################################

$(TARGET): $(OBJS)
		$(LINKING)
		@vc $(LDFLAGS) -o $@ $(OBJS)


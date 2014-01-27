################################################################################
# These are variables for the GBA toolchain build
# You can add others if you wish to
# Name: Yukang Yang
################################################################################

# The name of your desired GBA game
# This should be a just a name i.e MyFirstGBAGame
# No SPACES AFTER THE NAME.
PROGNAME = pacman

# The object files you want to compile into your program
# This should be a space (SPACE!) separated list of .o files
OFILES = main.o draw.o dijkstra.o text.o map.o object.o ghost.o redGhost.o blueGhost.o pacMan.o images.o start.o font.o

# The header files you have created.
# This is necessary to determine when to recompile for files.
# This should be a space (SPACE!) separated list of .h files
HFILES = header.h images.h start.h

################################################################################
# These are various settings used to make the GBA toolchain work
# DO NOT EDIT BELOW.
################################################################################

TOOLDIR  = /usr/local/cs2110-tools
ARMLIB   = $(TOOLDIR)/arm-thumb-eabi/lib
CFLAGS   = -Wall -Werror -std=c99 -pedantic 
CFLAGS   += -mthumb-interwork -mlong-calls -nostartfiles -MMD -MP -I $(TOOLDIR)/include
LDFLAGS = -L $(TOOLDIR)/lib \
		  -L $(TOOLDIR)/lib/gcc/arm-thumb-eabi/4.4.1/thumb \
		  -L $(ARMLIB) \
		  --script $(ARMLIB)/arm-gba.ld
LDDEBUG  = -L $(TOOLDIR)/lib -lgbaio
CDEBUG   = -g -DDEBUG
CRELEASE = -O2
CC       = $(TOOLDIR)/bin/arm-thumb-eabi-gcc
AS       = $(TOOLDIR)/bin/arm-thumb-eabi-as
LD       = $(TOOLDIR)/bin/arm-thumb-eabi-ld
OBJCOPY  = $(TOOLDIR)/bin/arm-thumb-eabi-objcopy
GDB      = $(TOOLDIR)/bin/arm-thumb-eabi-gdb
CFILES   = $(OFILES:.o=.c)

################################################################################
# These are the targets for the GBA build system
################################################################################

all : CFLAGS += $(CRELEASE)
all : $(PROGNAME).gba
	@echo "[FINISH] Created $(PROGNAME).gba"

.PHONY : all clean

$(PROGNAME).gba : $(PROGNAME).elf
	@echo "[LINK] Linking objects together to create $(PROGNAME).gba"
	@$(OBJCOPY) -O binary $(PROGNAME).elf $(PROGNAME).gba

$(PROGNAME).elf : crt0.o $(OFILES)
	@$(LD) $(LDFLAGS) -o $(PROGNAME).elf $^ -lgcc -lc -lgcc $(LDDEBUG)
	@rm -f *.d

crt0.o : $(ARMLIB)/crt0.s
	@$(AS) -mthumb-interwork $^ -o crt0.o

%.o : %.c
	@echo "[COMPILE] Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o : %.s                                                                        
	@echo "[AS] Assembling $<"                                               
	@$(AS) $< -o $@ -mthumb -mthumb-interwork

clean :
	@echo "[CLEAN] Removing all compiled files"
	@rm -f *.o *.elf *.gba *.d *~

vba : CFLAGS += $(CRELEASE)
vba : $(PROGNAME).gba
	@echo "[EXECUTE] Running Emulator VBA-M"
	@vbam $(VBAOPT) $(PROGNAME).gba > /dev/null 2> /dev/null

wxvba : CFLAGS += $(CRELEASE)
wxvba : $(PROGNAME).gba
	@echo "[EXECUTE] Running Emulator WXVBAM"
	@wxvbam $(PROGNAME).gba

med : CFLAGS += $(CRELEASE)
med : $(PROGNAME).gba
	@echo "[EXECUTE] Running Emulator MEDNAFEN"
	@mednafen $(PROGNAME).gba > /dev/null 2> /dev/null

debugvba : CFLAGS += $(CDEBUG)
debugvba : $(PROGNAME).gba
	@echo "[DEBUG] Running game in VBA-M"
	@vbam $(VBAOPT) $(PROGNAME).gba

debugwxvba : CFLAGS += $(CDEBUG)
debugwxvba : $(PROGNAME).gba
	@echo "[DEBUG] Running game in WXVBAM"
	@wxvbam $(PROGNAME).gba

-include $(CFILES:%.c=%.d)

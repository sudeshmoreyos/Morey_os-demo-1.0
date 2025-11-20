PROJECT = led_blink
MOREY_OS_PATH = ../../../..

ARCH = AVR_MEGA
CONTROLLER = MEGA328PB

CDEFS = 
PATHDIR = $(MOREY_OS_PATH)

include $(MOREY_OS_PATH)/arch/Makefile.controllerlist
PATHDIR += $(MOREY_OS_PATH)/arch/$(CPU)/$(CPU_SERIES)/drivers
PATHDIR += $(MOREY_OS_PATH)/arch/$(CPU)/$(CPU_SERIES)/lib
PATHDIR += $(MOREY_OS_PATH)/arch/$(CPU)/$(CPU_SERIES)/boot/$(CONTROLLER)

# List C source files here. (C dependencies are automatically generated.)

SRC = $(PROJECT).c
ifdef INCLUDE_SOURCES
SRC += $(INCLUDE_SOURCES)
endif
SRC += $(MOREY_OS_PATH)/main.c

include $(MOREY_OS_PATH)/os/Makefile.os
PATHDIR += $(MOREY_OS_PATH)/os/drivers
PATHDIR += $(MOREY_OS_PATH)/os/lib
PATHDIR += $(MOREY_OS_PATH)/os/sys
PATHDIR += $(MOREY_OS_PATH)/os/util

SRC += $(wildcard $(MOREY_OS_PATH)/arch/$(CPU)/$(CPU_SERIES)/boot/$(CONTROLLER)/*.c)
SRC += $(wildcard $(MOREY_OS_PATH)/arch/$(CPU)/$(CPU_SERIES)/drivers/*.c)
SRC += $(wildcard $(MOREY_OS_PATH)/arch/$(CPU)/$(CPU_SERIES)/lib/*.c)

# List Object directory to save object files
ifdef ARCH
OBJDIR = build/$(ARCH)/$(CONTROLLER)/obj
else
OBJDIR = build/$(BOARD)/obj
endif

# List Executable directory to save elf, hex, eep, etc. files
ifdef ARCH
EXEDIR = build/$(ARCH)/$(CONTROLLER)/exe
else
EXEDIR = build/$(BOARD)/exe
endif

# List .dep directory to save dependencies
ifdef ARCH
DEPDIR = build/$(ARCH)/$(CONTROLLER)/.dep
else
DEPDIR = build/$(BOARD)/.dep
endif

EXTRAINCDIRS = $(MOREY_OS_PATH) $(MOREY_OS_PATH)/os $(MOREY_OS_PATH)/os/drivers

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
#     AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.
DEBUG = dwarf-2

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99


# Place -D or -U options here for C sources
CDEFS += -DF_CPU=$(F_CPU)UL

#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes

#CFLAGS += -Wa,-adhlns=$(<:%.c=%.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)
#This flag added by Sudesh to Ignore .c files whose functions are not used
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections

MATH_LIB = -lm

#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(EXEDIR)/$(PROJECT).map,--cref

#This flag added by Sudesh to Ignore .c files whose functions are not used
LDFLAGS += -Wl,-gc-sections

LDFLAGS += $(MATH_LIB)


#============================================================================

# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp
WINSHELL = cmd


# Define English Messages

MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_PROGRAM = --------  programing  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_CLEANING = Cleaning project:

#MSG_ERRORS_NONE = Errors: none
#MSG_COFF = Converting to AVR COFF:
#MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
#MSG_EXTENDED_LISTING = Creating Extended Listing:
#MSG_SYMBOL_TABLE = Creating Symbol Table:
#MSG_COMPILING_CPP = Compiling C++:
#MSG_ASSEMBLING = Assembling:
#MSG_CREATING_LIBRARY = Creating library:

vpath %.c $(PATHDIR)
SOURCES := $(notdir $(SRC))
OBJ := $(patsubst %.c,$(OBJDIR)/%.o, $(SOURCES))

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF $(DEPDIR)/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)

# Default target.
#all: begin gccversion sizebefore build sizeafter end
all: begin gccversion build end

program: begin gccversion sizebefore build sizeafter program_text program_avrdude end


# Change the build target to build a HEX file or a library.
build: elf hex eep

elf: $(EXEDIR)/$(PROJECT).elf
hex: $(EXEDIR)/$(PROJECT).hex
eep: $(EXEDIR)/$(PROJECT).eep

# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on
# the following magic strings to be generated by the compile job.
begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo
	
program_text:
	@echo
	@echo $(MSG_PROGRAM)

# Display size of file.
# HEXSIZE = $(SIZE) --target=$(FORMAT) $(EXEDIR)/$(PROJECT).hex
ELFSIZE = $(SIZE) --mcu=$(MCU) --format=avr $(EXEDIR)/$(PROJECT).elf

sizebefore:
	@if test -f $(EXEDIR)/$(PROJECT).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	2>/dev/null; fi

sizeafter:
	@if test -f $(EXEDIR)/$(PROJECT).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	2>/dev/null; fi
	
# Display compiler version information.
gccversion : 
	@$(CC) --version
	
# Program the device. 
ifdef PROGRAMMER
ifdef PORT

ifeq ($(PROGRAMMER),usbasp)

ifndef FORCE
program_avrdude:
	@avrdude -c $(PROGRAMMER) -p $(MCU_AVRDUDE) -P $(PORT) -B 20.96 -U flash:w:"./$(EXEDIR)/$(PROJECT).hex":a
else
program_avrdude:
	@avrdude -c $(PROGRAMMER) -p $(MCU_AVRDUDE) -P $(PORT) -B 20.96 -F -U flash:w:"./$(EXEDIR)/$(PROJECT).hex":a
endif

else 

ifndef FORCE
program_avrdude:
	@avrdude -c $(PROGRAMMER) -p $(MCU_AVRDUDE) -P $(PORT) -U flash:w:"./$(EXEDIR)/$(PROJECT).hex":a
else
program_avrdude:
	@avrdude -c $(PROGRAMMER) -p $(MCU_AVRDUDE) -P $(PORT) -F -U flash:w:"./$(EXEDIR)/$(PROJECT).hex":a
endif

endif

else
program_avrdude:
	@echo Error : PORT not defined
endif
else
program_avrdude:
	@echo Error : PROGRAMMER not defined
endif

# Create final output files (.hex, .eep) from ELF output file.
$(EXEDIR)/%.hex: $(EXEDIR)/%.elf
	@echo $(MSG_FLASH) $@
	@$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock $< $@

$(EXEDIR)/%.eep: $(EXEDIR)/%.elf
	@echo $(MSG_EEPROM) $@
	@-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
	
# Link: create ELF output file from object files.
.SECONDARY : $(PROJECT).elf
.PRECIOUS : $(OBJ)
$(EXEDIR)/%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	@$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)
	
# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c
	@echo $(MSG_COMPILING) $<
	@$(CC) -c $(ALL_CFLAGS) $< -o $@ -include config.h
		
# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVEDIR) $(DEPDIR)
	$(REMOVEDIR) $(OBJDIR)
	$(REMOVEDIR) $(EXEDIR)

$(shell mkdir build 2>/dev/null)

ifdef ARCH
$(shell mkdir build/$(ARCH) 2>/dev/null)
$(shell mkdir build/$(ARCH)/$(CONTROLLER) 2>/dev/null)
else
$(shell mkdir build/$(BOARD) 2>/dev/null)
endif

# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Create object files directory
$(shell mkdir $(EXEDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir $(DEPDIR) 2>/dev/null) $(wildcard $(DEPDIR)/*)


# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep cclean clean_list
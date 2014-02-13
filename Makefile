#
# HDMISnoop Makefile
# 

# Target name
TARGET = hdmisnoop

# Cross compiler
CROSS_COMPILE ?= arm-none-eabi-

# CPU
CPU = cortex-m4

# C Standard
CSTD = gnu99

# Linker script
LINKER_SCRIPT = stm32.ld

# Libs
LIBS +=

# Optimization level
OPT = 0

# Debug options
DEBUG ?= 1

# Flash address
FLASH_ADDR = 0x08000000

# DFU alt
DFU_ALT = 0

# Verbosity
V ?= 0

ifeq ($(V), 0)
_@_=@
else
_@_=
endif

# Defines
DEFS += USART_ADD_CRLF
DEFS += USART_BAUD_RATE=115200

# Includes directories
INC_DIR += .
INC_DIR += sys/inc
INC_DIR += platform/inc
INC_DIR += cec/inc
INC_DIR += usb/inc

# Output dir
OUT_DIR = out

# Objects dir
OBJ_DIR = $(OUT_DIR)/obj/

# Tools
CC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
SIZE=$(CROSS_COMPILE)size
GDB=$(CROSS_COMPILE)gdb
CGDB=cgdb -d $(GDB)
COPY=cp
RM=rm -rf
ECHO=@echo
MKDIR=@mkdir -p
FLASH=scripts/flash.sh 
CREATE_DFU=scripts/dfu.py
DFU_UTIL=dfu-util

# C Source
CSRC += main.c
CSRC += sys/syscalls.c
CSRC += sys/clock.c
CSRC += platform/stm32f4xx.c
CSRC += platform/stm32f4xx_it.c
CSRC += cec/cec.c
CSRC += cec/cec_str.c
CSRC += usb/usb_str.c

# Assembler source
ASRC += platform/stm32f4xx_startup.s

# Output formats
ELF=$(OUT_DIR)/$(TARGET).elf
BIN=$(OUT_DIR)/$(TARGET).bin
HEX=$(OUT_DIR)/$(TARGET).hex
DFU=$(OUT_DIR)/$(TARGET).dfu

# Compiler flags
CFLAGS += -mcpu=$(CPU)
CFLAGS += -mthumb
CFLAGS += -mthumb-interwork
CFLAGS += -msoft-float
CFLAGS += -mlittle-endian
CFLAGS += -std=$(CSTD)

# Debug flags
ifeq ($(DEBUG), 1)
CFLAGS += -g
CFLAGS += -O0
DEFS += DEBUG
else
CLFAGS += -O$(OPT)
endif

# Inlcudes
INC_DIR_F = $(patsubst %,-I%, $(INC_DIR))
CFLAGS += $(INC_DIR_F)

# Defines
DEFS_F = $(patsubst %,-D%, $(DEFS))
CFLAGS += $(DEFS_F)

# Linker flags
LDFLAGS += -T$(LINKER_SCRIPT)
LDFLAGS += -nostartfiles

# Objects
COBJ = $(addprefix $(OBJ_DIR), $(CSRC:.c=.o))
AOBJ = $(addprefix $(OBJ_DIR), $(ASRC:.s=.o))
OBJS = $(COBJ) $(AOBJ)

# Output directories
OBJ_DIRS = $(sort $(dir $(OBJS)))

# Rules

# Default rule
default : all

.PHONY: flash
flash: $(FLASH) $(BIN)
	$(ECHO) "JTAG\tFlashing $(BIN)..."
	$(_@_)$(FLASH) $(BIN) $(FLASH_ADDR)

.PHONY: debug
debug: $(ELF)
	$(ECHO) "CGDB\t$(ELF)"
	$(_@_)$(CGDB) $(ELF)

.PHONY: dfu
dfu: $(DFU)
	$(ECHO) "DFU\tFlashing  $(DFU)..."
	$(_@_)$(DFU_UTIL) -a$(DFU_ALT) -D $(DFU)

all : $(OUT_DIR) $(OBJ_DIRS) $(BIN) $(HEX) $(DFU)

$(OBJ_DIRS) : $(OUT_DIR)
	$(_@_)$(MKDIR) $(OBJ_DIRS)

$(OUT_DIR) :
	$(_@_)$(MKDIR) $(OUT_DIR)

$(DFU) : $(BIN)
	$(ECHO) "DFU\t$@"
	$(_@_)$(CREATE_DFU) -b $(FLASH_ADDR):$(BIN) $(DFU)

$(BIN) : $(ELF)
	$(ECHO) "OBJCOPY\t$@"
	$(_@_)$(OBJCOPY) -Obinary $(ELF) $(BIN)

$(HEX) : $(ELF)
	$(ECHO) "OBJCOPY\t$@"
	$(_@_)$(OBJCOPY) -Oihex $(ELF) $(HEX)

$(ELF) : $(OBJS) $(LIBS)
	$(ECHO) "LD\t$@"
	$(_@_)$(CC) $(CFLAGS) $(LDFLAGS) -o $(ELF) $(OBJS) $(LIBS)

$(OBJ_DIR)%.o : %.c
	$(ECHO) "CC\t$<"
	$(_@_)$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)%.o : %.s
	$(ECHO) "AS\t$<"
	$(_@_)$(CC) $(CFLAGS) -c $< -o $@

.PHONY: size
size: $(ELF)
	$(_@_)$(SIZE) $(ELF)


.PHONY : clean
clean :
	$(ECHO) "RM\t$(OUT_DIR)"
	$(_@_)$(RM) $(OUT_DIR) 


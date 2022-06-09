TARGET = app
BUILD_DIR = build

C_SOURCES =  \
src/machine/machine.c \
src/sv/sv.c \
src/rand.c \
src/string.c \
src/syscall.c \

ASM_SOURCES =  \
src/machine/machine.S \
src/sv/sv.S \
src/userstart.S \
src/userpages.S \


INCLUDES =  \
-Iinclude 

C_FLAGS = -march=rv64gc -mabi=lp64 -static -mcmodel=medany -fvisibility=hidden -nostdlib -nostartfiles -std=gnu99 -O2 -g -Wall

OBJDUMP_FLAGS = -drwS

C_TO_AS_TARGET = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.S)))
AS_TO_O_TARGET = $(addprefix $(BUILD_DIR)/,$(notdir $(C_TO_AS_TARGET:.S=.S.o)))
AS_TO_O_TARGET += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.S.o)))

all:
	$(CELLIFT_GCC) $(C_FLAGS) $(INCLUDES) -Tlink.ld  $(ASM_SOURCES) $(C_SOURCES)  -o build/app.elf
	$(CELLIFT_OBJDUMP) $(OBJDUMP_FLAGS) build/app.elf > build/dump.log

# execute "make ctoas", modify the assembly if needed and then execute "make astoelf"
ctoas: $(C_TO_AS_TARGET)

astoelf:$(BUILD_DIR)/$(TARGET).S.elf 
	

$(BUILD_DIR)/%.S: src/%.c
	$(CELLIFT_GCC) $(C_FLAGS) $(INCLUDES)  -S $<  -o $@

$(BUILD_DIR)/%.S.o: $(BUILD_DIR)/%.S
	$(CELLIFT_GCC) $(C_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.S.o: src/%.S
	$(CELLIFT_GCC) $(C_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/$(TARGET).S.elf: $(AS_TO_O_TARGET) 
	$(CELLIFT_GCC) $(C_FLAGS) -Tlink.ld $(AS_TO_O_TARGET) -o $@
	$(CELLIFT_OBJDUMP) $(OBJDUMP_FLAGS) $@ > $(BUILD_DIR)/dump.S.txt

clean:
	rm -rf build/*


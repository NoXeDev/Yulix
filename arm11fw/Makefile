NAME		:=	$(shell basename $(CURDIR))

CC            =    $(DEVKITARM)/bin/arm-none-eabi-gcc
OBJCOPY        =    $(DEVKITARM)/bin/arm-none-eabi-objcopy

SRCDIR		:=	src
OUTDIR		:=	build
INCLUDEDIR	:=	include

INCLUDE		:=	-I$(SRCDIR) -I$(INCLUDEDIR) -I../common
ARCHFLAGS	:=	-mcpu=mpcore -march=armv6k -mtune=mpcore -mfloat-abi=hard -marm -mno-thumb-interwork \
	-ggdb -ffreestanding -lgcc -flto

ASMFLAGS	:=	$(ARCHFLAGS) $(INCLUDE) -x assembler-with-cpp
CFLAGS := $(ARCHFLAGS) $(INCLUDE) -MMD -MP -std=c++11 -Os -pipe -Wall -Wextra \
			-Wno-unused-variable -Wno-unused-parameter -Wno-unused-function \
			-ffunction-sections -fomit-frame-pointer -ffast-math -DARM11

LDFLAGS := -Tlink.ld -Wl,--gc-sections,--nmagic,-z,max-page-size=4,-Map=output.map  $(ARCHFLAGS)

rwildcard = $(foreach d, $(wildcard $1*), \
            $(filter $(subst *, %, $2), $d) \
            $(call rwildcard, $d/, $2))

SOURCE_OUTPUT := $(patsubst $(SRCDIR)/%.cpp, $(OUTDIR)/%.cpp.o, \
				$(patsubst $(SRCDIR)/%.s, $(OUTDIR)/%.s.o, \
				$(call rwildcard, $(SRCDIR), *.s *.cpp)))

.PHONY: all
all: $(NAME).bin

.PHONY: clean
clean:
	@rm -rf $(OUTDIR) $(NAME).elf $(NAME).bin

# ObjCopy
$(NAME).bin: $(NAME).elf
	@echo === Objcopy ===
	@$(OBJCOPY) -O binary $^ $@

# Linking
$(NAME).elf: $(SOURCE_OUTPUT)
	@echo === Linking ===
	@mkdir -p "$(@D)"
	@$(CC) $(LDFLAGS) $^ -o $@

# CPP compiling
$(OUTDIR)/%.cpp.o: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	@echo "  $<"
	@$(CC) -c $(CFLAGS) -o $@ $<

# Assembly compiling
$(OUTDIR)/%.s.o: $(SRCDIR)/%.s
	@mkdir -p "$(@D)"
	@echo "  $<"
	@$(CC) -c $(ASMFLAGS) -o $@ $<

include $(call rwildcard, $(OUTDIR), *.d)
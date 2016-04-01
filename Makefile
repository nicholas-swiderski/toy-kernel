AUXFILES := grub.cfg

PROJDRS := sources

SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
ASMFILES := $(shell find $(PROJDIRS) -type f -name "*.S" ! -name "crti.S" ! -name "crtn.S")

COBJFILES := $(patsubst %.c,%.o,$(SRCFILES))
SOBJFILES := $(patsubst %.S,%.o,$(ASMFILES))

OBJFILES := $(COBJFILES) $(SOBJFILES)

DEPFILES := $(patsubst %.c,%.d,$(SRCFILES))

ALLFILES := $(SRCFILES) $(HDRFILES) $(AUXFILES)

.PHONY: all clean todo iso debug

INCLUDE := -I./include
WARNINGS := -Wall -Wextra -pedantic -Werror
CFLAGS := -T linker.ld -nostdlib -ffreestanding -std=gnu11 -masm=intel $(WARNINGS) $(INCLUDE)
ifeq ($(MAKECMDGOALS),debug)
CFLAGS += -g
endif

CRTI_OBJ:=sources/kernel/crti.o
CRTBEGIN_OBJ:=$(shell ./x-tools/i686-unknown-elf/bin/i686-unknown-elf-gcc $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell ./x-tools/i686-unknown-elf/bin/i686-unknown-elf-gcc $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=sources/kernel/crtn.o

OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(OBJFILES) \
$(CRTEND_OBJ) \
$(CRTN_OBJ) \

all: bin

debug: bin

bin: $(OBJFILES)
	./x-tools/i686-unknown-elf/bin/i686-unknown-elf-gcc -o xX420n0sc0p3OSXx.bin $(CFLAGS) $(OBJ_LINK_LIST) -lgcc

iso:
	cp grub.cfg ./iso/boot/grub/
	cp xX420n0sc0p3OSXx.bin ./iso/boot/
	./grub/grub-mkrescue -o xX420n0sc0p3OSXx.iso ./iso

clean:
	-@$(RM) $(wildcard $(OBJFILES) $(DEPFILES) xX420n0sc0p3OSXx.iso xX420n0sc0p3OSXx.bin)

-include $(DEPFILES)

todo:
	-@for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO -e FIXME $$file; done; true

%.o: %.c Makefile
	./x-tools/i686-unknown-elf/bin/i686-unknown-elf-gcc $(CFLAGS) -MD -MP -c $< -o $@

%.o : %.S
	./x-tools/i686-unknown-elf/bin/i686-unknown-elf-gcc $(CFLAGS) -MD -MP -c $< -o $@

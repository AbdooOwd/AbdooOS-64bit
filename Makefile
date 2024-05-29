# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

# This is the name that our final kernel executable will have.
# Change as needed.
override KERNEL := AbdooOS

override BUILD_DIR	:= bin
override SRC_DIR	:= src

override IMAGE_NAME := image.iso

# Convenience macro to reliably declare user overridable variables.
define DEFAULT_VAR =
    ifeq ($(origin $1),default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1),undefined)
        override $(1) := $(2)
    endif
endef

# It is suggested to use a custom built cross toolchain to build a kernel.
# We are using the standard "cc" here, it may work by using
# the host system's toolchain, but this is not guaranteed.
override DEFAULT_KCC := cc
$(eval $(call DEFAULT_VAR,KCC,$(DEFAULT_KCC)))

# Same thing for "ld" (the linker).
override DEFAULT_KLD := ld
$(eval $(call DEFAULT_VAR,KLD,$(DEFAULT_KLD)))

# User controllable C flags.
override DEFAULT_KCFLAGS := -g -O2 -pipe
$(eval $(call DEFAULT_VAR,KCFLAGS,$(DEFAULT_KCFLAGS)))

# User controllable C preprocessor flags. We set none by default.
override DEFAULT_KCPPFLAGS :=
$(eval $(call DEFAULT_VAR,KCPPFLAGS,$(DEFAULT_KCPPFLAGS)))

# User controllable nasm flags.
override DEFAULT_KNASMFLAGS := -F dwarf -g
$(eval $(call DEFAULT_VAR,KNASMFLAGS,$(DEFAULT_KNASMFLAGS)))

# User controllable linker flags. We set none by default.
override DEFAULT_KLDFLAGS :=
$(eval $(call DEFAULT_VAR,KLDFLAGS,$(DEFAULT_KLDFLAGS)))

# Internal C flags that should not be changed by the user.
override KCFLAGS += \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fPIE \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone

# Internal C preprocessor flags that should not be changed by the user.
override KCPPFLAGS := \
    -I src \
    $(KCPPFLAGS) \
    -MMD \
    -MP

# Internal linker flags that should not be changed by the user.
override KLDFLAGS += \
    -m elf_x86_64 \
    -nostdlib \
    -static \
    -pie \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
    -T linker.ld

# Internal nasm flags that should not be changed by the user.
override KNASMFLAGS += \
    -Wall \
    -f elf64

# Use "find" to glob all *.c, *.S, and *.asm files in the tree and obtain the
# object and header dependency file names.
override CFILES := $(shell cd src && find -L * -type f -name '*.c')
override ASFILES := $(shell cd src && find -L * -type f -name '*.S')
override NASMFILES := $(shell cd src && find -L * -type f -name '*.asm')
override OBJ := $(addprefix $(BUILD_DIR)/obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
override HEADER_DEPS := $(addprefix $(BUILD_DIR)/obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))


# Default target.
.PHONY: all
all: os-image run

build: os-image

kernel: $(BUILD_DIR)/$(KERNEL)
os-image: $(BUILD_DIR)/$(IMAGE_NAME)



$(BUILD_DIR)/$(IMAGE_NAME): always kernel
	# The following line was missing a leading tab
	make -C limine

	# Create a directory which will be our ISO root.
	mkdir -p $(BUILD_DIR)/iso_root

	# Copy the relevant files over.
	mkdir -p $(BUILD_DIR)/iso_root/boot
	cp -v bin/$(KERNEL) $(BUILD_DIR)/iso_root/boot/
	mkdir -p $(BUILD_DIR)/iso_root/boot/limine
	cp -v limine.cfg limine/limine-bios.sys limine/limine-bios-cd.bin \
	      limine/limine-uefi-cd.bin $(BUILD_DIR)/iso_root/boot/limine/

	# Create the EFI boot tree and copy Limine's EFI executables over.
	mkdir -p $(BUILD_DIR)/iso_root/EFI/BOOT
	cp -v limine/BOOTX64.EFI $(BUILD_DIR)/iso_root/EFI/BOOT/
	cp -v limine/BOOTIA32.EFI $(BUILD_DIR)/iso_root/EFI/BOOT/

	# Create the bootable ISO.
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
	        -no-emul-boot -boot-load-size 4 -boot-info-table \
	        --efi-boot boot/limine/limine-uefi-cd.bin \
	        -efi-boot-part --efi-boot-image --protective-msdos-label \
	        $(BUILD_DIR)/iso_root -o $@

	# Install Limine stage 1 and 2 for legacy BIOS boot.
	./limine/limine bios-install $@


# Link rules for the final kernel executable.
$(BUILD_DIR)/$(KERNEL): Makefile linker.ld $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(KLD) $(OBJ) $(KLDFLAGS) -o $@

# Include header dependencies.
-include $(HEADER_DEPS)

# Compilation rules for *.c files.
$(BUILD_DIR)/obj/%.c.o: src/%.c Makefile
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@

# Compilation rules for *.S files.
$(BUILD_DIR)/obj/%.S.o: src/%.S Makefile
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
$(BUILD_DIR)/obj/%.asm.o: src/%.asm Makefile
	mkdir -p "$$(dirname $@)"
	nasm $(KNASMFLAGS) $< -o $@



always:
	mkdir -p $(BUILD_DIR)

debug:
	qemu-system-x86_64 -M smm=off -d int -no-reboot -no-shutdown -cdrom $(BUILD_DIR)/$(IMAGE_NAME)

run: os-image
	qemu-system-x86_64 -cdrom $(BUILD_DIR)/$(IMAGE_NAME)

# Remove object files and the final executable.
.PHONY: clean
clean clear:
	rm -rf $(BUILD_DIR)/* $(BUILD_DIR)/obj
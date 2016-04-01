#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/segment.h>
#include <libk/string.h>
#include <libk/stdio.h>
#include <libk/stdlib.h>
#include <acpi/rsdp.h>
#include <acpi/sdt.h>
#include <interrupts/interrupt.h>
#include <pci/pci.h>

#include <grub/multiboot.h>

multiboot_memory_map_t *highest_region;

extern void set_gdt(uint32_t, uint16_t);
extern void reload_segments();
extern void set_idt(uint32_t, uint16_t);
//TODO interrupt testing code
extern void do_int_69();

//various basic system setup tasks, run before global constructors
void kernel_early(unsigned int magic, multiboot_info_t* mbt) {
	highest_region = (multiboot_memory_map_t *)mbt->mmap_addr;

	terminal_initialize();

	//TODO all this grub stuff should probably be moved to a seperate file
	//check magic value passed is valid
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Something has gone horribly wrong (invalid magic number)");
		abort();
	}

	if (~mbt->flags & 1) {
		printf("Something has gone horribly wrong (mem_lower and mem_upper are invalid)");
		abort();
	}

	if (~mbt->flags & 1 << 6) {
		printf("Something has gone horribly wrong (mmap_addr is invalid)");
		abort();
	}

	for (multiboot_memory_map_t* grub_memmap_pointer = (multiboot_memory_map_t*) mbt->mmap_addr;
			(unsigned long) grub_memmap_pointer < mbt->mmap_addr + mbt->mmap_length;
			grub_memmap_pointer = (multiboot_memory_map_t*)((unsigned int)grub_memmap_pointer + grub_memmap_pointer->size + sizeof(grub_memmap_pointer->size))) {
		//TODO need to come up with a good format to handle this

		if(highest_region->addr < grub_memmap_pointer->addr && grub_memmap_pointer->type == 1 && grub_memmap_pointer->len > 0xFFFF) {
			highest_region = grub_memmap_pointer;
		}
		//TODO debug
//		printf("Address: 0x%llX\nLength: 0x%llX\nType: 0x%lX\n", (uint64_t)grub_memmap_pointer->addr, (uint64_t)grub_memmap_pointer->len, (uint32_t)grub_memmap_pointer->type);
	}

	//set up the GDT
	//TODO maybe move this to a seperate file? keeping it here for now
	gdt_entry_t gdt[3];
	//null descriptor
	gdt[0] = make_gdt_entry(0, 0, 0, 0);
	//kernel code segment
	gdt[1] = make_gdt_entry((uint16_t)0xFFFFFFFF, 0, 0x9A, 0xCF);
	//kernel data segment
	gdt[2] = make_gdt_entry((uint16_t)0xFFFFFFFF, 0, 0x92, 0xCF);
	//TODO code segment - DPL 3
	//TODO data segment - DPL 3
	//TODO TSS segment or whatever

	//call the assembly to actually set the GDT
	set_gdt((uint32_t)gdt, sizeof(gdt));
	reload_segments();

	//set up the IDT
	//TODO maybe move this to a seperate file? keeping it here for now
	idt_entry_t idt[256];
	//fill with null entries for now
	for (int i = 0; i < 256; i++) {
		idt[i] = make_idt_entry(0, make_dt_selector(0, 0, 0), 0);
	}

	//TODO interrupt handling test
	idt[69] = make_idt_entry((uint32_t)&int_69_wrapper, make_dt_selector(0, 0, 1), (uint8_t)0x8E);

	set_idt((uint32_t)idt, sizeof(idt));

	//RSDP
	//TODO this should probably be in a seperate file
	unsigned char *rsdp_ptr = find_rsdp();
	rsdp_t *rsdp;
	if (rsdp_ptr == 0) {
		printf("Cannot verify system supports ACPI.\n");
		//TODO what do we do if we're running on a toaster from 1835 and we don't have ACPI?
		abort();
	} else if (verify_rsdp(rsdp_ptr)) {
		rsdp = (rsdp_t *)rsdp_ptr;
	} else {
		//TODO maybe instead of panic-ing we should continue as if we had no ACPI support?
		printf("Something has gone horribly wrong (unable to validate RSDP)\n");
		abort();
	}
	if(rsdp->revision == 0) {
		//TODO debug
		printf("ACPI v1.0\n");
	}else if(rsdp->revision == 1) {
		//TODO some day we should implement ACPI v2 support
		//TODO debug
		printf("ACPI v2.0\n");
	}

	//RSDT
	rsdt_t *rsdt = (rsdt_t *)rsdp->rsdt_address;

	if(!verify_sdt((unsigned char *)rsdt)) {
		//something is probably genuinely wrong at this point so we err on the side of caution and panic
		printf("Something has gone horribly wrong (unable to validate RSDT)\n");
		abort();
	}

	//TODO debug
	printf("RSDT is at 0x%lX\n", (uint32_t)rsdt);

	//FADT
    fadt_t *fadt;
    char fadt_text[5] = "FACP";
    for(unsigned int i = 0; i < (rsdt->h.length - sizeof(rsdt->h)) / 4; i++) {
		acpi_sdt_header_t *head = (acpi_sdt_header_t *)rsdt->table_pointers[i];
		if(memcmp(&head->signature, fadt_text, 4) == 0 && verify_sdt((unsigned char *)head)) {
			fadt = (fadt_t *)head;
			printf("FADT found at 0x%lX\n", (uint32_t)fadt);
		}
    }
}

//run after global constructors
__attribute__((noreturn))
void kernel_main() {

	//set up a new stack wayyyyy up near the top of the memory we can access now
	//yes it's hackish but memory is hard
	__asm__(".intel_syntax noprefix");
	__asm__("mov esp, %0":: "a" ((uint32_t)(highest_region->addr + highest_region->len - 1)));

	//PCI


	//TODO interrupt handling test/making sure we get to this point
	do_int_69();

	__asm__("cli");
	__asm__("hlt");
	while(1) { }
	__builtin_unreachable();
}

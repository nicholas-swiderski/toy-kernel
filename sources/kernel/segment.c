#include <stdint.h>

#include <kernel/segment.h>

gdt_entry_t make_gdt_entry(uint16_t limit_low, uint32_t base, uint8_t type_access, uint8_t limit_high_granularity) {
	gdt_entry_t gdt_e;

	gdt_e.base_low = base & 0xFFFF;
	gdt_e.base_mid = (base >> 16) & 0xFF;
	gdt_e.base_high = (base >> 24) & 0xFF;

	gdt_e.limit_low = limit_low;

	gdt_e.type_access = type_access;

	gdt_e.limit_high_granularity = limit_high_granularity;

	return gdt_e;
}

idt_entry_t make_idt_entry(uint32_t offset, dt_selector_t selector, uint8_t type) {
	idt_entry_t idt_e;

	idt_e.offset_low = offset & 0xFFFF;
	idt_e.offset_high = (offset >> 16) & 0xFFFF;

	idt_e.selector = selector;

	idt_e.type = type;

	idt_e.zero = (uint8_t) 0;

	return idt_e;
}

dt_selector_t make_dt_selector(uint8_t rpl, uint8_t ti, uint16_t index) {
	dt_selector_t dt_sel;

	dt_sel.rpl_ti_index_low = (rpl & 0x3);
	dt_sel.rpl_ti_index_low |= (ti & 0x1) << 2;
	dt_sel.rpl_ti_index_low |= (index & 0xFF) << 3;
	dt_sel.index_high = (index >> 8) & 0xFF;

	return dt_sel;
}

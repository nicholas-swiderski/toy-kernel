#include <stdint.h>

#pragma pack(1)

struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t type_access;
	uint8_t limit_high_granularity;
	uint8_t base_high;
};
typedef struct gdt_entry gdt_entry_t;

struct dt_selector {
	uint8_t rpl_ti_index_low;
	uint8_t index_high;
};
typedef struct dt_selector dt_selector_t;

struct idt_entry {
	uint16_t offset_low;
	dt_selector_t selector;
	uint8_t zero;
	uint8_t type;
	uint16_t offset_high;
};
typedef struct idt_entry idt_entry_t;

gdt_entry_t make_gdt_entry(uint16_t, uint32_t, uint8_t, uint8_t);
idt_entry_t make_idt_entry(uint32_t, dt_selector_t, uint8_t);
dt_selector_t make_dt_selector(uint8_t, uint8_t, uint16_t);

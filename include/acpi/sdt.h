#include <stdint.h>

#pragma pack(1)

//standard header for all ACPI SDTs
struct acpi_sdt_header {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oem_id[6];
	char oem_table_id[8];
	uint32_t oem_revision;
	uint32_t creator_id;
	uint32_t creator_revision;
};
typedef struct acpi_sdt_header acpi_sdt_header_t;

struct rsdt {
	acpi_sdt_header_t h;
	uint32_t table_pointers[1];
};
typedef struct rsdt rsdt_t;

struct generic_address_structure {
	uint8_t address_space;
	uint8_t bit_width;
	uint8_t bit_offset;
	uint8_t access_size;
	uint64_t address;
};

struct fadt {
	struct acpi_sdt_header h;
	uint32_t firmwarectrl;
	uint32_t dsdt;

	uint8_t  reserved;

	uint8_t preferred_power_management_profile;
	uint16_t sci_interrupt;
	uint32_t smi_command_port;
	uint8_t acpi_enable;
	uint8_t acpi_disable;
	uint8_t s4_bios_req;
	uint8_t pstate_control;
	uint32_t pm1a_event_block;
	uint32_t pm1b_event_block;
	uint32_t pm1a_control_block;
	uint32_t pm1b_control_block;
	uint32_t pm2_control_block;
	uint32_t pm_timer_block;
	uint32_t gpe0_block;
	uint32_t gpe1_block;
	uint8_t pm1_event_length;
	uint8_t pm1_control_length;
	uint8_t pm2_control_length;
	uint8_t pm_timer_length;
	uint8_t gpe0_length;
	uint8_t gpe1_length;
	uint8_t gpe1_base;
	uint8_t cstate_control;
	uint16_t worst_c2_latency;
	uint16_t worst_c3_latency;
	uint16_t flush_size;
	uint16_t flush_stride;
	uint8_t duty_offset;
	uint8_t duty_width;
	uint8_t day_alarm;
	uint8_t month_alarm;
	uint8_t century;

	uint16_t ia32_boot_flags;

	uint8_t reserved2;
	uint32_t flags;

	struct generic_address_structure resetreg;

	uint8_t resetvalue;
	uint8_t reserved3[3];

	// 64bit pointers - available on acpi 2.0+
	uint64_t x_firmwarecontrol;
	uint64_t x_dsdt;

	struct generic_address_structure x_pm1aeventblock;
	struct generic_address_structure x_pm1beventblock;
	struct generic_address_structure x_pm1acontrolblock;
	struct generic_address_structure x_pm1bcontrolblock;
	struct generic_address_structure x_pm2controlblock;
	struct generic_address_structure x_pmtimerblock;
	struct generic_address_structure x_gpe0block;
	struct generic_address_structure x_gpe1block;
};
typedef struct fadt fadt_t;

bool verify_sdt(unsigned char*);

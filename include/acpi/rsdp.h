#include <stdint.h>
#include <stdbool.h>

#pragma pack(1)

struct rsdp {
	char signature[8];
	uint8_t checksum;
	char oemid[6];
	uint8_t revision;
	uint32_t rsdt_address;
	uint32_t length;
	uint64_t xsdt_address;
	uint8_t extended_checksum;
	uint8_t reserved[3];
};

typedef struct rsdp rsdp_t;

unsigned char* find_rsdp();
bool verify_rsdp(unsigned char*);
//rsdp_t make_rsdp(unsigned char*);

#include <stdint.h>
#include <stdbool.h>

#include <acpi/rsdp.h>
#include <libk/string.h>
#include <libk/stdio.h>

//yes, i know, but this REALLY IS the recommended (only?) way of finding the RSDT
unsigned char* find_rsdp() {
	char sig[8] = "RSD PTR ";
	for (unsigned char *sig_check = (unsigned char *)0x000E0000; sig_check <= (unsigned char *)0x000FFFFF; sig_check += 0x10) {
		if (memcmp(sig_check, sig, 8) == 0) {
			return sig_check;
		}
	}
	return 0;
}

bool verify_rsdp(unsigned char* rsdp_ptr) {
	uint8_t sum = 0;
	for (int i = 0; i <= 20; i++) {
		sum += *(rsdp_ptr + i);
	}

	if (sum != 0) {
		return false;
	}

	if (*(rsdp_ptr + 15) == 1) {
		sum = 0;
		for (int i = 20; i <= 32; i++) {
			sum += *(rsdp_ptr + i);
		}
	}

	return sum == 0;
}

//rsdp_t make_rsdp(unsigned char* rsdp_ptr) {
//	rsdp_t rsdp;
//
//	memcpy(rsdp.signature, rsdp_ptr, 8);
//
//	rsdp.checksum = *(rsdp_ptr + 8);
//
//	memcpy(rsdp.oemid, (rsdp_ptr + 9), 6);
//
//	rsdp.revision = *(rsdp_ptr + 15);
//
//	rsdp.rsdt_address = *((uint32_t *)(rsdp_ptr + 16));
//
//	if (rsdp.revision == 1) {
//		rsdp.length = *((uint32_t *)(rsdp_ptr + 20));
//
//		rsdp.xsdt_address = *((uint64_t *)(rsdp_ptr + 24));
//
//		rsdp.extended_checksum = *(rsdp_ptr + 32);
//	}
//
//	return rsdp;
//}

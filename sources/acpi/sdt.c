#include <stdint.h>
#include <stdbool.h>

//TODO debug
//#include <libk/stdio.h>

bool verify_sdt(unsigned char* sdtp) {
	uint8_t sum = 0;

//	if((uint32_t) sdtp[4] == 0) {
//		//TODO debug
//		printf("SDT size == 0\n");
//		return false;
//	}

	for (unsigned int i = 0; i < (uint32_t) sdtp[4]; i++) {
		sum += *(sdtp + i);
	}

	return sum == 0;
 }

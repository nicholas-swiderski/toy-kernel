#include <stddef.h>
#include <stdint.h>

#include <libk/string.h>

//converts an integer to an ascii character array
void itoa(int integer, char buffer[]) {
	int i, sign;

	if ((sign = integer) < 0) { /* record sign */
		integer = -integer;    /* make n positive */
	}
	i = 0;
	do {       /* generate digits in reverse order */
		buffer[i++] = integer % 10 + '0';   /* get next digit */
	} while ((integer /= 10) > 0);     /* delete it */
	if (sign < 0) {
		buffer[i++] = '-';
	}
	buffer[i] = '\0';

	//reverse the array (cos it's backwards now)
	reverse(buffer);
}

void uinttoa(uint64_t integer, char buffer[]) {
	uint64_t i = 0;

	do {       /* generate digits in reverse order */
		buffer[i++] = integer % 10 + '0';   /* get next digit */
	} while ((integer /= 10) > 0);     /* delete it */

	buffer[i] = '\0';

	//reverse the array (cos it's backwards now)
	reverse(buffer);
}

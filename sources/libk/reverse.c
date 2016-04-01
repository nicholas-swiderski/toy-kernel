#include <libk/string.h>

void reverse(char buffer[]) {
	int i, j;
	char c;
	for (i = 0, j = strlen(buffer)-1; i<j; i++, j--) {
		c = buffer[i];
		buffer[i] = buffer[j];
		buffer[j] = c;
	}
}

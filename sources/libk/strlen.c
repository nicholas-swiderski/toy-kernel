#include <libk/string.h>

//returns the size of a null-terminated char array (string)
size_t strlen(const char* str) {
	size_t ret = 0;
	while (str[ret] != '\0') {
		ret++;
	}
	return ret;
}

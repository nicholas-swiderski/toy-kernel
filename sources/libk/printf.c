#include <stdbool.h>
#include <stdarg.h>

#include <libk/stdio.h>
#include <libk/string.h>

static void print(const char* data, size_t data_length) {
	for (size_t i = 0; i < data_length; i++) {
		putchar((int)((const unsigned char*) data)[i]);
	}
}

#define UINT32FLAG 		1
#define UINT64FLAG	 	1 << 1
#define UINT16FLAG		1 << 2
#define UINT8FLAG		1 << 3

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

	while (*format != '\0') {

		uint16_t flags = 0;
		bool hex_cap = false;

		if (*format != '%') {
print_c:
			amount = 1;
			while (format[amount] && format[amount] != '%') {
				amount++;
			}
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format;

next_format:
		if (*(++format) == '%') {
			goto print_c;
		}

		if (rejected_bad_specifier) {
incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}

		switch (*format) {
		case 'c':
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
			break;

		case 's':
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
			break;

		case 'l':
			if (flags & UINT32FLAG) {
				flags |= UINT64FLAG;
			} else {
				flags |= UINT32FLAG;
			}
			goto next_format;

		case 'h':
			if (flags & UINT16FLAG) {
				flags |= UINT8FLAG;
			} else {
				flags |= UINT16FLAG;
			}
			goto next_format;

		case 'X':
			hex_cap = true;
			//fall through

		case 'x':
			format++;
			//smaller unsigned integer types get expanded to unsigned int when they're run through va_arg
			uint64_t quotient = (flags & UINT64FLAG) ? va_arg(parameters, uint64_t) :
								(flags & UINT32FLAG) ? va_arg(parameters, uint32_t) :
								(flags & UINT8FLAG) ? va_arg(parameters, unsigned int) :
								(flags & UINT16FLAG) ? va_arg(parameters, unsigned int) :
								va_arg(parameters, unsigned int);

			char hex_num[22] = {0};
			unsigned int i = 0, temp;

			do {

				temp = quotient % 16;

				//To convert integer into character
				if (temp < 10) {
					temp += 48;
				} else if(hex_cap){
					temp += 55;
				} else {
					temp += 87;
				}

				hex_num[i++] = temp;
				quotient /= 16;
			} while (quotient != 0);
			reverse(hex_num);
			print((const char*)hex_num, i * sizeof(char));
			break;

		default:
			goto incomprehensible_conversion;
		}

	}

	va_end(parameters);

	return written;
}

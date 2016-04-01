#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <libk/string.h>
#include <kernel/vga.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void scroll_terminal_output() {
	for (uint8_t x = 0; x < VGA_HEIGHT-1; x++) {
		for (uint8_t y = 0; y < VGA_WIDTH; y++) {
			terminal_buffer[x * VGA_WIDTH + y] = terminal_buffer[(x+1) * VGA_WIDTH + y];
		}
	}

	for (uint8_t x = 0; x < VGA_WIDTH; x++) {
		terminal_buffer[(VGA_HEIGHT-1) * VGA_WIDTH + x] = make_vgaentry(' ', terminal_color);
	}

	terminal_row--;
	terminal_column = 0;
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c) {
	if (c == '\n') {
		terminal_row++;
		terminal_column = 0;
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (terminal_column++ >= VGA_WIDTH) {
			terminal_row++;
			terminal_column = 0;

			if (terminal_row++ > VGA_HEIGHT) {
				scroll_terminal_output();
			}
		}
	}

	if (terminal_row >= VGA_HEIGHT) {
		scroll_terminal_output();
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++) {
		terminal_putchar(data[i]);
	}
}

#include <stddef.h>
#include <stdint.h>

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);
void itoa(int, char[]);
void uinttoa(uint64_t, char[]);
void reverse(char[]);

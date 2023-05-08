#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;

extern const char *ROM_TYPES[35];

void print_bytes(void *p, size_t len);
void read_file(const char *path, u8 *dst);

#define PRINT_BYTES(x) print_bytes(&x, sizeof(x))

#endif // UTIL_H

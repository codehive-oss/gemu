#ifndef UTIL_H
#define UTIL_H

#include "./types.h"
#include <stddef.h>

#define rLY 0xFF44

extern const char *ROM_TYPES[35];

void print_bytes(void *p, size_t len);
void read_file(const char *path, u8 *dst);

EmulationState *emu_init();
void emu_free(EmulationState *emu);

#define PRINT_BYTES(x) print_bytes(&x, sizeof(x))
#define LENGTH(x) sizeof(x) / sizeof(x[0])

#endif // UTIL_H

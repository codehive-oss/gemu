#ifndef UTIL_H
#define UTIL_H

#include "./types.h"
#include <stddef.h>

#define rLY 0xFF44

extern const char *ROM_TYPES[35];

void print_bytes(void *p, size_t len);
void read_file(const char *path, u8 *dst);
int getch(void);

u8 get_palette_idx(u8 *tile_data, u8 i);

EmulationState *emu_init();
void emu_print(EmulationState *emu);
void emu_free(EmulationState *emu);

void set_flags(EmulationState *emu, char z, char n, char h, char c);

#define BIT_SET(a, n, x) \
  {                      \
    if (x)               \
      a |= (1 << n);     \
    else                 \
      a &= ~(1 << n);    \
  }
#define PRINT_BYTES(x) print_bytes(&x, sizeof(x))
#define LENGTH(x) sizeof(x) / sizeof(x[0])

#endif // UTIL_H

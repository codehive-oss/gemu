#ifndef UTIL_H
#define UTIL_H

#include "./types.h"
#include <stddef.h>

#define rLY 0xFF44

extern const char *ROM_TYPES[35];

// https://gbdev.io/pandocs/The_Cartridge_Header.html
typedef struct RomHeader {
  u8 entrypoint[4];
  u8 logo[0x30];
  u8 title[16];
  u16 new_lic_code;
  u8 sgb_flag;
  u8 type;
  u8 rom_size;
  u8 ram_size;
  u8 dest_code;
  u8 lic_code;
  u8 version;
  u8 checksum;
  u16 global_checksum;

} RomHeader;

typedef struct EmulationState {
  // Data
  u8 *mem;
  u8 *reg;
  u16 mcycle;

  // Pointer
  u8 *rom;
  RomHeader *header;
  u8 *tilemaps;

  u8 *vram;
  u8 *sram;
  u8 *wram;
  u8 *oam;
  u8 *io;
  u8 *hram;
  u8 *ie;

  // General Purpose Registers
  u8 *a;
  u8 *f;
  u16 *af;

  u8 *b;
  u8 *c;
  u16 *bc;

  u8 *d;
  u8 *e;
  u16 *de;

  u8 *h;
  u8 *l;
  u16 *hl;

  u16 *sp;
  u16 *pc;
} EmulationState;

void print_bytes(void *p, size_t len);
void read_file(const char *path, u8 *dst);
void dump_file(const char *path, u8 *data, size_t length);
int getch(void);

u8 get_palette_idx(u8 *tile_data, u8 i);

EmulationState *emu_init();
void emu_print(EmulationState *emu);
void emu_free(EmulationState *emu);

void set_flags(EmulationState *emu, char z, char n, char h, char c);

#define BIT_SET(a, n, x)                                                       \
  {                                                                            \
    if (x)                                                                     \
      a |= (1 << n);                                                           \
    else                                                                       \
      a &= ~(1 << n);                                                          \
  }
#define PRINT_BYTES(x) print_bytes(&x, sizeof(x))
#define LENGTH(x) sizeof(x) / sizeof(x[0])

#endif // UTIL_H

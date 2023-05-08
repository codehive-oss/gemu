#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct RomHeader // https://gbdev.io/pandocs/The_Cartridge_Header.html
{
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
  bool running;

  // Data
  u8 *mem;
  u8 *reg;
  u16 mcycle;

  // Pointer
  u8 *rom;
  RomHeader *header;

  u8 *vram;
  u8 *sram;
  u8 *wram;
  u8 *oam;
  u8 *io;
  u8 *hram;
  u8 *ie;

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

#endif // TYPES_H

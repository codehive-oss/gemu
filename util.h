#ifndef UTIL_H
#define UTIL_H

#include "./types.h"
#include <stddef.h>

// https://gbdev.io/pandocs/STAT.html
#define LY   0xFF44
#define LYC  0xFF45
#define STAT 0xFF41

// https://gbdev.io/pandocs/Timer_and_Divider_Registers.html#ff04--div-divider-register
#define DIV 0xFF04

#define Z_MASK 0b10000000
#define N_MASK 0b01000000
#define H_MASK 0b00100000
#define C_MASK 0b00010000

// https://gbdev.io/pandocs/LCDC.html
#define LCDC 0xFF40

#define LCD_ENABLE          0b10000000
#define WINDOW_TILEMAP_AREA 0b01000000
#define WINDOW_ENABLE       0b00100000
#define BG_WINDOW_DATA_AREA 0b00010000
#define BG_TILEMAP_AREA     0b00001000
#define OBJ_SIZE            0b00000100
#define OBJ_ENABLE          0b00000010
#define BG_WINDOW_ENABLE    0b00000001

// https://gbdev.io/pandocs/OAM.html#byte-3--attributesflags
#define BG_OVER_OBJ         0b10000000
#define Y_flip              0b01000000
#define X_flip              0b00100000
#define Palette_NON_CGB_NUM 0b00010000
#define TILE_VRAM_BANK      0b00001000
#define Palette_CGB_NUM     0b00000111

#define IE 0xFFFF
#define IF 0xFF0F

#define IF_VBLANK   0b00000001
#define IF_LCD_STAT 0b00000010
#define IF_TIMER    0b00000100
#define IF_SERIAL   0b00001000
#define IF_JOYPAD   0b00010000

extern const char *ROM_TYPES[256];
extern const char *NEW_LICENSEE_CODE[256];
extern const char *DESTINATION_CODE[2];
extern const char *OLD_LICENSEE_CODE[256];

// https://gbdev.io/pandocs/The_Cartridge_Header.html
typedef struct RomHeader {
  u8  entrypoint[4];
  u8  logo[0x30];
  u8  title[16];
  u16 new_lic_code;
  u8  sgb_flag;
  u8  type;
  u8  rom_size;
  u8  ram_size;
  u8  dest_code;
  u8  lic_code;
  u8  version;
  u8  checksum;
  u16 global_checksum;

} RomHeader;

// https://gbdev.io/pandocs/OAM.html
typedef struct SpriteAttribute {
  u8 yPos;
  u8 xPos;
  u8 tileIdx;
  u8 flags;
} SpriteAttribute;

typedef struct EmulationState {
  // Data
  u8 *mem;
  u8 *reg;
  u16 mcycle;

  // Pointer
  RomHeader       *header;
  u8              *tilemaps;
  SpriteAttribute *sprites;

  u8 *rom;
  u8 *vram;
  u8 *sram;
  u8 *wram;
  u8 *oam;
  u8 *io;
  u8 *hram;

  bool ime;

  // General Purpose Registers
  u8  *a;
  u8  *f;
  u16 *af;

  u8  *b;
  u8  *c;
  u16 *bc;

  u8  *d;
  u8  *e;
  u16 *de;

  u8  *h;
  u8  *l;
  u16 *hl;

  u16 *sp;
  u16 *pc;
} EmulationState;

void   print_bytes(void *p, size_t len);
size_t read_rom(const char *path, u8 *dst);
void   dump_file(const char *path, u8 *data, size_t length);
int    getch(void);

u8 get_palette_idx(u8 *tile_data, u8 i);

EmulationState *emu_init();
void            emu_print(EmulationState *emu);
void            emu_free(EmulationState *emu);

void set_flags(EmulationState *emu, char z, char n, char h, char c);

#define BIT_SET(a, n, x) \
  {                      \
    if (x)               \
      a |= (1 << n);     \
    else                 \
      a &= ~(1 << n);    \
  }
#define PRINT_BYTES(x) print_bytes(&x, sizeof(x))
#define LENGTH(x)      sizeof(x) / sizeof(x[0])

#endif // UTIL_H

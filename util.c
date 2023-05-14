#include "./util.h"
#include "./types.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// https://gbdev.io/pandocs/The_Cartridge_Header.html#0147--cartridge-type
const char *ROM_TYPES[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "",
    "MBC2",
    "MBC2+BATTERY",
    "",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "",
    "",
    "",
    "",
    "",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "",
    "MBC6",
    "",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

void print_bytes(void *p, size_t len) {
  for (size_t i = len; i > 0; i--) {
    printf("%02X", ((u8 *)p)[i - 1]);
  }
  printf("\n");
}

void read_file(const char *path, u8 *dst) {
  FILE *file = fopen(path, "rb");

  if (file == NULL) {
    printf("Could not open file: %s\n", path);
  }

  fseek(file, 0, SEEK_END);
  unsigned long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (fread(dst, 1, length, file) > 0) {
    printf("Failed to read file: %s\n", path);
  }

  fclose(file);
}

void dump_file(const char *path, u8 *data, size_t length) {
  FILE *f = fopen(path, "wb");
  if (!f) {
    printf("Failed to open file.\n");
  }

  printf("Dumping %zu bytes\n", sizeof(u8) * length);
  fwrite(data, sizeof(u8), length, f);

  fclose(f);
}

int getch(void) {
  int ch;
  struct termios oldt, newt;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

u8 get_palette_idx(u8 *tile_data, u8 i) {
  assert(i < 64);

  u8 memIdx = 2 * (i / 8);
  u8 memOffset = 7 - i % 8;

  // 01010101
  // 01010101
  u8 p1 = (tile_data[memIdx] & (1 << memOffset)) >> memOffset;
  u8 p2 = (tile_data[memIdx + 1] & (1 << memOffset)) >> memOffset << 1;

  u8 idx = p1 | p2;

  return idx;
}

EmulationState *emu_init() {
  EmulationState *emu = (EmulationState *)malloc(sizeof(EmulationState));

  emu->mem = (u8 *)malloc(65536);
  emu->reg = (u8 *)malloc(12);
  emu->mcycle = 0;

  emu->rom = emu->mem;
  emu->header = (RomHeader *)(emu->rom + 0x100);
  emu->tilemaps = emu->mem + 0x9800;

  emu->vram = emu->mem + 0x8000;
  emu->sram = emu->mem + 0xA000;
  emu->wram = emu->mem + 0xC000;
  emu->oam = emu->mem + 0xFE00;
  emu->io = emu->mem + 0xFF00;
  emu->hram = emu->mem + 0xFF80;
  emu->ie = emu->mem + 0xFFFF;

  emu->a = emu->reg;
  emu->f = emu->reg + 1;
  emu->af = (u16 *)emu->a;

  emu->b = emu->reg + 2;
  emu->c = emu->reg + 3;
  emu->bc = (u16 *)emu->b;

  emu->d = emu->reg + 4;
  emu->e = emu->reg + 5;
  emu->de = (u16 *)emu->d;

  emu->h = emu->reg + 6;
  emu->l = emu->reg + 7;
  emu->hl = (u16 *)emu->h;

  emu->sp = (u16 *)emu->reg + 8;
  emu->pc = (u16 *)emu->reg + 10;

  return emu;
}

void emu_print(EmulationState *emu) {
  printf("AF: ");
  PRINT_BYTES(*emu->af);

  printf("BC: ");
  PRINT_BYTES(*emu->bc);

  printf("DE: ");
  PRINT_BYTES(*emu->de);

  printf("HL: ");
  PRINT_BYTES(*emu->hl);

  printf("SP: ");
  PRINT_BYTES(*emu->sp);

  printf("PC: ");
  PRINT_BYTES(*emu->pc);
}

void emu_free(EmulationState *emu) {
  free(emu->mem);
  free(emu->reg);
  free(emu);
}

void set_flags(EmulationState *emu, char z, char n, char h, char c) {
  if (z != -1) {
    BIT_SET(*emu->f, 7, z);
  }
  if (n != -1) {
    BIT_SET(*emu->f, 6, n);
  }
  if (h != -1) {
    BIT_SET(*emu->f, 5, h);
  }
  if (c != -1) {
    BIT_SET(*emu->f, 4, c);
  }
}

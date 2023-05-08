#include "./types.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
  size_t i;
  for (i = 0; i < len; ++i)
    printf("%02X ", ((u8 *)p)[i]);
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

  fread(dst, 1, length, file);
  fclose(file);
}

EmulationState *emu_init() {
  EmulationState *emu = (EmulationState *)malloc(sizeof(EmulationState));
  emu->mem = (u8 *)malloc(65536);
  emu->reg = (u8 *)malloc(12);
  emu->mcycle = 0;

  emu->rom = emu->mem;
  emu->header = (RomHeader *)(emu->rom + 0x100);

  emu->vram = emu->mem + 0x8000;
  emu->sram = emu->mem + 0xA000;
  emu->wram = emu->mem + 0xC000;
  emu->oam = emu->mem + 0xFE00;
  emu->io = emu->mem + 0xFF00;
  emu->hram = emu->mem + 0xFF80;
  emu->ie = emu->mem + 0xFFFF;

  emu->a = emu->reg;
  emu->f = emu->reg + 1;
  emu->b = emu->reg + 2;
  emu->c = emu->reg + 3;
  emu->d = emu->reg + 4;
  emu->e = emu->reg + 5;
  emu->h = emu->reg + 6;
  emu->l = emu->reg + 7;
  emu->sp = (u16 *)emu->reg + 8;
  emu->pc = (u16 *)emu->reg + 10;

  return emu;
}

void emu_free(EmulationState *emu) {
  free(emu->mem);
  free(emu->reg);
  free(emu);
}

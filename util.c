#include "./util.h"
#include <stddef.h>
#include <stdio.h>

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

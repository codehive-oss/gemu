#include "./util.h"
#include <stdio.h>

u8 mem[65536];
u8 *rom = mem;
u8 *vram = mem + 0x8000;
u8 *sram = mem + 0xA000;
u8 *wram = mem + 0xC000;
u8 *oam = mem + 0xFE00;
u8 *io = mem + 0xFF00;
u8 *hram = mem + 0xFF80;
u8 *ie = mem + 0xFFFF;

u8 reg[12];
u8 *a = reg;
u8 *f = reg + 1;
u8 *b = reg + 2;
u8 *c = reg + 3;
u8 *d = reg + 4;
u8 *e = reg + 5;
u8 *h = reg + 6;
u8 *l = reg + 7;
u16 *sp = (u16 *)reg + 8;
u16 *pc = (u16 *)reg + 10;

void jp() {
  (*pc)++;
  u16 target = *(u16 *)&mem[*pc];
  *pc = target;
  // printf("Jump Instruction Found\n");
  // print_short(target);
}

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

int main(void) {
  read_file("main.gb", mem);
  RomHeader *headers = (RomHeader *)(rom + 0x100);
  printf("START HEADERS\n");
  printf("Title: %s\n", headers->title);
  printf("Rom Type: %s\n", ROM_TYPES[(u8)headers->type]);
  printf("Rom Size: %dKB\n", 32 * (1 << headers->rom_size));
  u8 checksum =
      0; // https://gbdev.io/pandocs/The_Cartridge_Header.html#014d--header-checksum
  for (u16 address = 0x0134; address <= 0x014C; address++)
    checksum = checksum - rom[address] - 1;

  printf("Header Checksum: %d\n", headers->checksum);
  printf("Computed Value:  %d\n", checksum);
  printf("END HEADERS\n");

  *pc = 0x0100;
  u8 instruction = mem[*pc];
  printf("Instruction: %hhx\n", instruction);

  switch (instruction) {
  case 0xc3:
    jp();
    break;
  }

  printf("Instruction pointer\n");
  PRINT_BYTES(*pc);

  return 0;
}

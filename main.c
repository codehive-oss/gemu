#include <stdio.h>
#include <stdint.h>
#include "./rom_types.h"
#include "./common.h"

unsigned char mem[65536];
unsigned char *rom = mem;
unsigned char *vram = mem + 0x8000;
unsigned char *sram = mem + 0xA000;
unsigned char *wram = mem + 0xC000;
unsigned char *oam = mem + 0xFE00;
unsigned char *io = mem + 0xFF00;
unsigned char *hram = mem + 0xFF80;
unsigned char *ie = mem + 0xFFFF;

unsigned char reg[12];
unsigned char *a = reg;
unsigned char *f = reg + 1;
unsigned char *b = reg + 2;
unsigned char *c = reg + 3;
unsigned char *d = reg + 4;
unsigned char *e = reg + 5;
unsigned char *h = reg + 6;
unsigned char *l = reg + 7;
unsigned short *sp = (unsigned short *)reg + 8;
unsigned short *pc = (unsigned short *)reg + 10;

void print_bytes(void *p, size_t len)
{
  size_t i;
  for (i = 0; i < len; ++i)
    printf("%02X\n", ((unsigned char *)p)[i]);
}

void print_short(short x) { print_bytes(&x, sizeof(x)); }
void print_double(double x) { print_bytes(&x, sizeof(x)); }

void read_file(const char *path, unsigned char *dst)
{
  FILE *file = fopen(path, "rb");

  if (file == NULL)
  {
    printf("Could not open file: %s\n", path);
  }

  fseek(file, 0, SEEK_END);
  unsigned long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  fread(dst, 1, length, file);
  fclose(file);
}

void jp()
{
  (*pc)++;
  unsigned short target = *(unsigned short *)&mem[*pc];
  *pc = target;
  // printf("Jump Instruction Found\n");
  // print_short(target);
}

typedef struct RomHeader // https://gbdev.io/pandocs/The_Cartridge_Header.html
{
  unsigned char entrypoint[4];
  unsigned char logo[0x30];
  unsigned char title[16];
  unsigned short new_lic_code;
  unsigned char sgb_flag;
  unsigned char type;
  unsigned char rom_size;
  unsigned char ram_size;
  unsigned char dest_code;
  unsigned char lic_code;
  unsigned char version;
  unsigned char checksum;
  unsigned short global_checksum;

} RomHeader;

int main(void)
{
  read_file("main.gb", mem);
  RomHeader *headers = (RomHeader *)(rom + 0x100);
  printf("START HEADERS\n");
  printf("Title: %s\n", headers->title);
  printf("Rom Type: %s\n", ROM_TYPES[(u8)headers->type]);
  printf("Rom Size: %dKB\n", 32 * (1 << headers->rom_size));
  uint8_t checksum = 0; // https://gbdev.io/pandocs/The_Cartridge_Header.html#014d--header-checksum
  for (uint16_t address = 0x0134; address <= 0x014C; address++)
  {
    checksum = checksum - rom[address] - 1;
  }
  printf("Header Checksum: %d\n", headers->checksum);
  printf("Computed Value:  %d\n", checksum);
  printf("END HEADERS\n");

  *pc = 0x0100;
  unsigned char instruction = mem[*pc];
  printf("Instruction: %hhx\n", instruction);

  switch (instruction)
  {
  case 0xc3:
    jp();
    break;
  }

  printf("Instruction pointer\n");
  print_short(*pc);

  return 0;
}

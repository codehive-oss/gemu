#include <stdio.h>

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

void print_bytes(void *p, size_t len) {
  size_t i;
  for (i = 0; i < len; ++i)
    printf("%02X\n", ((unsigned char *)p)[i]);
}

void print_short(short x) { print_bytes(&x, sizeof(x)); }
void print_double(double x) { print_bytes(&x, sizeof(x)); }

void read_file(const char *path, unsigned char *dst) {
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

void jp() {
  (*pc)++;
  unsigned short target = *(unsigned short *)&mem[*pc];
  *pc = target;
  // printf("Jump Instruction Found\n");
  // print_short(target);
}

void ld() {}

int main(void) {
  read_file("main.gb", mem);
  *pc = 0x0100;
  unsigned char instruction = mem[*pc];
  printf("Instruction: %hhx\n", instruction);

  switch (instruction) {
  case 0xc3:
    jp();
    break;
  }

  printf("Instruction pointer\n");
  print_short(*pc);

  return 0;
}

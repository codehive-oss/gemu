#include <stdio.h>

char mem[65536];
char *rom = mem;
char *vram = mem + 0x8000;
char *sram = mem + 0xA000;
char *wram = mem + 0xC000;
char *oam = mem + 0xFE00;
char *io = mem + 0xFF00;
char *hram = mem + 0xFF80;
char *ie = mem + 0xFFFF;

char reg[12];
char *a = reg;
char *f = reg + 1;
char *b = reg + 2;
char *c = reg + 3;
char *d = reg + 4;
char *e = reg + 5;
char *h = reg + 6;
char *l = reg + 7;
short *sp = (short*)reg + 8;
short *pc = (short*)reg + 10;

void read_file(const char *path, char *dst) {
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

int main(void) {
  read_file("main.gb", mem);

  printf("%hhx\n", mem[0x14F]);
  return 0;
}

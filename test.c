#include "./types.h"
#include "./util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_palette_idx() {
  u8 b[] = {
      0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
      0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
  };

  for (u8 i = 0; i < 64; i++) {
    u8 idx = get_palette_idx(b, i);
    printf("%2d: %02X\n", i, idx);
  }
}

int main(void) { test_palette_idx(); }

#include "./types.h"
#include "./util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_palette_idx() {
  u8 b = 0b00110110;
  for (u8 i = 0; i < 4; i++) {
    printf("%02X\n", get_palette_idx(&b, i));
  }
}

int main(void) { test_palette_idx(); }

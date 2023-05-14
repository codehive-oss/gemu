#include "./types.h"
#include "./util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check(const char *description, bool condition) {
  printf("%s\n", description);
  assert(condition);

  printf("Passed!\n\n");
}

void test_palette_idx() {
  u8 b[] = {
      0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
      0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
  };

  bool passed = true;
  for (u8 i = 0; i < 64; i++) {
    u8 idx = get_palette_idx(b, i);
    if (idx != 0x2) {
      passed = false;
    }
  }

  check("Test palette index", passed);
}

int main(void) { test_palette_idx(); }

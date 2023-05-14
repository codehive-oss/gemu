#include "./instruction.h"
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

#define SUITE_START(description) printf("%s\n", description)
#define CHECK(condition) assert(condition)
#define SUITE_END() printf("Passed\n\n")

void test_palette_idx() {
  SUITE_START("Test palette index");

  u8 b[] = {
      0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
      0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
  };

  for (u8 i = 0; i < 64; i++) {
    u8 idx = get_palette_idx(b, i);
    CHECK(idx == 0x2);
  }

  SUITE_END();
}

void test_add_sub_reg() {
  EmulationState *emu = emu_init();

  SUITE_START("Test add_reg8_reg8");
  {
    u8 target = 0x0F;
    u8 from = 0x01;
    add_reg8_reg8(emu, &target, &from);
    CHECK(target == 0x10);
    CHECK(!(*emu->f & Z_MASK));
    CHECK(!(*emu->f & N_MASK));
    CHECK(*emu->f & H_MASK);
    CHECK(!(*emu->f & C_MASK));

    target = 0xFF;
    from = 0x01;
    add_reg8_reg8(emu, &target, &from);
    CHECK(target == 0x00);
    CHECK(*emu->f & Z_MASK);
    CHECK(!(*emu->f & N_MASK));
    CHECK(*emu->f & H_MASK);
    CHECK(*emu->f & C_MASK);
  }
  SUITE_END();

  SUITE_START("Test sub_reg8_reg8");
  {
    u8 target = 0x0F;
    u8 from = 0x01;
    sub_reg8_reg8(emu, &target, &from);
    CHECK(target == 0x0E);
    CHECK(!(*emu->f & Z_MASK));
    CHECK(*emu->f & N_MASK);
    CHECK(!(*emu->f & H_MASK));
    CHECK(!(*emu->f & C_MASK));

    target = 0x01;
    from = 0x02;
    sub_reg8_reg8(emu, &target, &from);
    CHECK(target == 0xFF);
    CHECK(!(*emu->f & Z_MASK));
    CHECK(*emu->f & N_MASK);
    CHECK(*emu->f & H_MASK);
    CHECK(*emu->f & C_MASK);
  }
  SUITE_END();

  emu_free(emu);
}

int main(void) {
  test_palette_idx();
  test_add_sub_reg();
}

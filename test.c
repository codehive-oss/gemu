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
#define CHECK_MASK_TRUE(value, mask) CHECK(value &mask)
#define CHECK_MASK_FALSE(value, mask) CHECK(!(value & mask))
#define SUITE_END() printf("Passed\n\n")

void test_palette_idx() {
  SUITE_START("Test palette index");

  u8 b[] = {
      0x00,
      0xFF,
      0x00,
      0xFF,
      0x00,
      0xFF,
      0x00,
      0xFF,

      0x00,
      0xFF,
      0x00,
      0xFF,
      0x00,
      0xFF,
      0x00,
      0xFF,
  };

  for (u8 i = 0; i < 64; i++) {
    u8 idx = get_palette_idx(b, i);
    CHECK(idx == 0x2);
  }

  SUITE_END();
}

void test_add_sub() {
  EmulationState *emu = emu_init();

  SUITE_START("Test add_regd8");
  {
    *emu->a = 0x0F;
    u8 from = 0x01;
    add_regd8(emu, &from);
    CHECK(*emu->a == 0x10);
    CHECK_MASK_FALSE(*emu->f, Z_MASK);
    CHECK_MASK_FALSE(*emu->f, N_MASK);
    CHECK_MASK_TRUE(*emu->f, H_MASK);
    CHECK_MASK_FALSE(*emu->f, C_MASK);

    *emu->a = 0xFF;
    from = 0x01;
    add_regd8(emu, &from);
    CHECK(*emu->a == 0x00);
    CHECK(*emu->f & Z_MASK);
    CHECK(!(*emu->f & N_MASK));
    CHECK(*emu->f & H_MASK);
    CHECK(*emu->f & C_MASK);
  }
  SUITE_END();

  SUITE_START("Test sub_regd8");
  {
    *emu->a = 0x0F;
    u8 from = 0x01;
    sub_regd8(emu, &from);
    CHECK(*emu->a == 0x0E);
    CHECK(!(*emu->f & Z_MASK));
    CHECK(*emu->f & N_MASK);
    CHECK(!(*emu->f & H_MASK));
    CHECK(!(*emu->f & C_MASK));

    *emu->a = 0x01;
    from = 0x02;
    sub_regd8(emu, &from);
    CHECK(*emu->a == 0xFF);
    CHECK(!(*emu->f & Z_MASK));
    CHECK(*emu->f & N_MASK);
    CHECK(*emu->f & H_MASK);
    CHECK(*emu->f & C_MASK);
  }
  SUITE_END();

  emu_free(emu);
}

void test_inc_dec() {
  EmulationState *emu = emu_init();

  SUITE_START("Testing inc_regd8");

  u8 reg = 0x00;
  inc_regd8(emu, &reg);
  CHECK(reg == 0x01);
  CHECK_MASK_FALSE(*emu->f, Z_MASK);
  CHECK_MASK_FALSE(*emu->f, N_MASK);
  CHECK_MASK_FALSE(*emu->f, H_MASK);

  reg = 0xFF;
  inc_regd8(emu, &reg);
  CHECK(reg == 0x00);
  CHECK_MASK_TRUE(*emu->f, Z_MASK);
  CHECK_MASK_FALSE(*emu->f, N_MASK);
  CHECK_MASK_TRUE(*emu->f, H_MASK);

  reg = 0x0F;
  inc_regd8(emu, &reg);
  CHECK(reg == 0x10);
  CHECK_MASK_FALSE(*emu->f, Z_MASK);
  CHECK_MASK_FALSE(*emu->f, N_MASK);
  CHECK_MASK_TRUE(*emu->f, H_MASK);

  SUITE_END();

  SUITE_START("Testing dec_regd8");

  reg = 0x01;
  dec_regd8(emu, &reg);
  CHECK(reg == 0x00);
  CHECK_MASK_TRUE(*emu->f, Z_MASK);
  CHECK_MASK_TRUE(*emu->f, N_MASK);
  CHECK_MASK_FALSE(*emu->f, H_MASK);

  reg = 0x00;
  dec_regd8(emu, &reg);
  CHECK(reg == 0xFF);
  CHECK_MASK_FALSE(*emu->f, Z_MASK);
  CHECK_MASK_TRUE(*emu->f, N_MASK);

  reg = 0x10;
  dec_regd8(emu, &reg);
  CHECK(reg == 0x0F);
  CHECK_MASK_FALSE(*emu->f, Z_MASK);
  CHECK_MASK_TRUE(*emu->f, N_MASK);
  CHECK_MASK_TRUE(*emu->f, H_MASK);
  CHECK_MASK_TRUE(*emu->f, H_MASK);

  SUITE_END();

  emu_free(emu);
}

int main(void) {
  test_palette_idx();
  test_add_sub();
  test_inc_dec();
}

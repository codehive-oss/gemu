#include "./instruction.h"
#include "./types.h"
#include "./util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "greatest.h"

SUITE(palette_idx);
SUITE(add);
SUITE(sub);
SUITE(inc);
SUITE(dec);
SUITE(and);
SUITE(adc);

TEST palette_idx_test(void) {
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
    ASSERT_EQ(idx, 0x2);
  }

  PASS();
}

TEST add_test1() {
  EmulationState *emu = emu_init();
  *emu->a             = 0x0F;
  u8 from             = 0x01;
  add_regd8(emu, from);
  ASSERT_EQ(*emu->a, 0x10);
  ASSERT_FALSE(*emu->f & Z_MASK);
  ASSERT_FALSE(*emu->f & N_MASK);
  ASSERT(*emu->f & H_MASK);
  ASSERT_FALSE(*emu->f & C_MASK);

  emu_free(emu);
  PASS();
}

TEST add_test2() {
  EmulationState *emu = emu_init();
  *emu->a             = 0xFF;
  u8 from             = 0x01;
  add_regd8(emu, from);
  ASSERT_EQ(*emu->a, 0x00);
  ASSERT(*emu->f & Z_MASK);
  ASSERT_FALSE(*emu->f & N_MASK);
  ASSERT(*emu->f & H_MASK);
  ASSERT(*emu->f & C_MASK);

  emu_free(emu);
  PASS();
}

TEST sub_test1() {
  EmulationState *emu = emu_init();
  *emu->a             = 0x0F;
  u8 from             = 0x01;
  sub_regd8(emu, from);
  ASSERT_EQ(*emu->a, 0x0E);
  ASSERT_FALSE(*emu->f & Z_MASK);
  ASSERT(*emu->f & N_MASK);
  ASSERT_FALSE(*emu->f & H_MASK);
  ASSERT_FALSE(*emu->f & C_MASK);

  emu_free(emu);
  PASS();
}

TEST sub_test2() {
  EmulationState *emu = emu_init();
  *emu->a             = 0x01;
  u8 from             = 0x02;
  sub_regd8(emu, from);
  ASSERT_EQ(*emu->a, 0xFF);
  ASSERT_FALSE(*emu->f & Z_MASK);
  ASSERT(*emu->f & N_MASK);
  ASSERT(*emu->f & H_MASK);
  ASSERT(*emu->f & C_MASK);

  emu_free(emu);
  PASS();
}

TEST inc_test1() {
  EmulationState *emu = emu_init();

  u8 reg = 0x00;
  inc_regd8(emu, &reg);
  ASSERT_EQ(reg, 0x01);
  ASSERT_FALSE(*emu->f & Z_MASK);
  ASSERT_FALSE(*emu->f & N_MASK);
  ASSERT_FALSE(*emu->f & H_MASK);

  emu_free(emu);
  PASS();
}

TEST inc_test2() {
  EmulationState *emu = emu_init();

  u8 reg = 0xFF;
  inc_regd8(emu, &reg);
  ASSERT_EQ(reg, 0x00);
  ASSERT(*emu->f & Z_MASK);
  ASSERT_FALSE(*emu->f & N_MASK);
  ASSERT(*emu->f & H_MASK);

  emu_free(emu);
  PASS();
}

TEST inc_test3() {
  EmulationState *emu = emu_init();

  u8 reg = 0x0F;
  inc_regd8(emu, &reg);
  ASSERT_EQ(reg, 0x10);
  ASSERT_FALSE(*emu->f & Z_MASK);
  ASSERT_FALSE(*emu->f & N_MASK);
  ASSERT(*emu->f & H_MASK);

  emu_free(emu);
  PASS();
}

TEST dec_test1() {
  EmulationState *emu = emu_init();

  u8 reg = 0x01;
  dec_regd8(emu, &reg);
  ASSERT_EQ(reg, 0x00);
  ASSERT(*emu->f & Z_MASK);
  ASSERT(*emu->f & N_MASK);
  ASSERT_FALSE(*emu->f & H_MASK);

  emu_free(emu);
  PASS();
}

TEST dec_test2() {
  EmulationState *emu = emu_init();

  u8 reg = 0x00;
  dec_regd8(emu, &reg);
  ASSERT_EQ(reg, 0xFF);
  ASSERT_FALSE(*emu->f & Z_MASK);
  ASSERT(*emu->f & N_MASK);

  emu_free(emu);
  PASS();
}

TEST dec_test3() {
  EmulationState *emu = emu_init();

  u8 reg = 0x10;
  dec_regd8(emu, &reg);
  ASSERT_EQ(reg, 0x0F);
  ASSERT_FALSE(*emu->f & Z_MASK);
  ASSERT(*emu->f & N_MASK);
  ASSERT(*emu->f & H_MASK);
  ASSERT(*emu->f & H_MASK);

  emu_free(emu);
  PASS();
}

TEST and_test1() {
  EmulationState *emu = emu_init();

  // Test case 1: Perform AND operation with non-zero value
  *emu->a     = 0x0F;
  emu->rom[0] = 0x3C; // AND with 0x3C
  *emu->pc    = 0;
  and_d8(emu);
  ASSERT_EQ(*emu->a, 0x0C);       // Result of 0x0F AND 0x3C
  ASSERT_FALSE(*emu->f & Z_MASK); // Zero flag should be clear
  ASSERT_FALSE(*emu->f & N_MASK); // Subtraction flag should be clear
  ASSERT(*emu->f & H_MASK);       // Half-carry flag should be set
  ASSERT_FALSE(*emu->f & C_MASK); // Carry flag should be clear
  ASSERT_EQ(*emu->pc, 1);         // Program counter should be incremented by 1

  emu_free(emu);
  PASS();
}

TEST and_test2() {
  EmulationState *emu = emu_init();

  // Test case 2: Perform AND operation with zero value
  *emu->a     = 0x00;
  emu->rom[0] = 0x00; // AND with 0x00
  *emu->pc    = 0;
  and_d8(emu);
  ASSERT_EQ(*emu->a, 0x00);       // Result of 0x00 AND 0x00
  ASSERT(*emu->f & Z_MASK);       // Zero flag should be set
  ASSERT_FALSE(*emu->f & N_MASK); // Subtraction flag should be clear
  ASSERT(*emu->f & H_MASK);       // Half-carry flag should be set
  ASSERT_FALSE(*emu->f & C_MASK); // Carry flag should be clear
  ASSERT_EQ(*emu->pc, 1);         // Program counter should be incremented by 1

  emu_free(emu);
  PASS();
}

TEST adc_without_carry() {
  EmulationState *emu = emu_init();

  *emu->a = 0x0F;
  adc_regd8(emu, 0x10);
  ASSERT_EQ(*emu->a, 0x1F);
  ASSERT_FALSE(*emu->f & Z_MASK);
  ASSERT_FALSE(*emu->f & N_MASK);
  ASSERT_FALSE(*emu->f & H_MASK);
  ASSERT_FALSE(*emu->f & C_MASK);

  emu_free(emu);
  PASS();
}

TEST adc_with_half_carry() {
  EmulationState *emu = emu_init();

  *emu->a = 0x0F;
  adc_regd8(emu, 0x1F);
  ASSERT_EQ(*emu->a, 0x2E);
  ASSERT_FALSE(*emu->f & Z_MASK);
  ASSERT_FALSE(*emu->f & N_MASK);
  ASSERT(*emu->f & H_MASK);
  ASSERT_FALSE(*emu->f & C_MASK);

  emu_free(emu);
  PASS();
}

TEST adc_with_carry() {
  EmulationState *emu = emu_init();

  *emu->a = 0xFF;
  adc_regd8(emu, 0x01);
  ASSERT_EQ(*emu->a, 0x00);
  ASSERT(*emu->f & Z_MASK);
  ASSERT_FALSE(*emu->f & N_MASK);
  ASSERT(*emu->f & H_MASK);
  ASSERT(*emu->f & C_MASK);

  emu_free(emu);
  PASS();
}

SUITE(palette_idx) {
  RUN_TEST(palette_idx_test);
}

SUITE(add) {
  RUN_TEST(add_test1);
  RUN_TEST(add_test2);
}

SUITE(sub) {
  RUN_TEST(sub_test1);
  RUN_TEST(sub_test2);
}

SUITE(inc) {
  RUN_TEST(inc_test1);
  RUN_TEST(inc_test2);
  RUN_TEST(inc_test3);
}

SUITE(dec) {
  RUN_TEST(dec_test1);
  RUN_TEST(dec_test2);
  RUN_TEST(dec_test3);
}

SUITE(and) {
  RUN_TEST(and_test1);
  RUN_TEST(and_test2);
}

SUITE(adc) {
  RUN_TEST(adc_without_carry);
  RUN_TEST(adc_with_half_carry);
  RUN_TEST(adc_with_carry);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();

  RUN_SUITE(palette_idx);
  RUN_SUITE(add);
  RUN_SUITE(sub);
  RUN_SUITE(inc);
  RUN_SUITE(dec);
  RUN_SUITE(and);
  RUN_SUITE(adc);

  GREATEST_MAIN_END();
}

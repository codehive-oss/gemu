#include "./instruction.h"
#include "./types.h"
#include "./util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "greatest.h"

SUITE(palette_idx_suite);
SUITE(add_suite);
SUITE(sub_suite);
SUITE(inc_suite);
SUITE(dec_suite);
SUITE(and_suite);
SUITE(adc_suite);

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

TEST and_with_nonzero() {
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

TEST and_with_zero() {
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

TEST ccf_carry_flag_set() {
  EmulationState *emu = emu_init();
  *emu->f |= C_MASK; // Set carry flag
  ccf(emu);
  ASSERT_FALSE(*emu->f & C_MASK); // Carry flag should be cleared
  ASSERT_FALSE(*emu->f & N_MASK); // Subtraction flag should be cleared
  ASSERT_FALSE(*emu->f & H_MASK); // Half-carry flag should be cleared
  PASS();
}

TEST ccf_carry_flag_cleared() {
  EmulationState *emu = emu_init();
  *emu->f &= ~C_MASK; // Clear carry flag
  ccf(emu);
  ASSERT(*emu->f & C_MASK);       // Carry flag should be set
  ASSERT_FALSE(*emu->f & N_MASK); // Subtraction flag should be cleared
  ASSERT_FALSE(*emu->f & H_MASK); // Half-carry flag should be cleared
  PASS();
}

TEST scf_carry_flag_set() {
  EmulationState *emu = emu_init();
  scf(emu);
  ASSERT(*emu->f & C_MASK);       // Carry flag should be set
  ASSERT_FALSE(*emu->f & N_MASK); // Subtraction flag should be cleared
  ASSERT_FALSE(*emu->f & H_MASK); // Half-carry flag should be cleared
  PASS();
}

TEST scf_carry_flag_cleared() {
  EmulationState *emu = emu_init();
  *emu->f |= C_MASK; // Set carry flag
  scf(emu);
  ASSERT(*emu->f & C_MASK);       // Carry flag should still be set
  ASSERT_FALSE(*emu->f & N_MASK); // Subtraction flag should be cleared
  ASSERT_FALSE(*emu->f & H_MASK); // Half-carry flag should be cleared
  PASS();
}

SUITE(palette_idx_suite) {
  RUN_TEST(palette_idx_test);
}

SUITE(add_suite) {
  RUN_TEST(add_test1);
  RUN_TEST(add_test2);
}

SUITE(sub_suite) {
  RUN_TEST(sub_test1);
  RUN_TEST(sub_test2);
}

SUITE(inc_suite) {
  RUN_TEST(inc_test1);
  RUN_TEST(inc_test2);
  RUN_TEST(inc_test3);
}

SUITE(dec_suite) {
  RUN_TEST(dec_test1);
  RUN_TEST(dec_test2);
  RUN_TEST(dec_test3);
}

SUITE(and_suite) {
  RUN_TEST(and_with_nonzero);
  RUN_TEST(and_with_zero);
}

SUITE(adc_suite) {
  RUN_TEST(adc_without_carry);
  RUN_TEST(adc_with_half_carry);
  RUN_TEST(adc_with_carry);
}

SUITE(ccf_suite) {
  RUN_TEST(ccf_carry_flag_set);
  RUN_TEST(ccf_carry_flag_cleared);
}

SUITE(scf_suite) {
  RUN_TEST(scf_carry_flag_set);
  RUN_TEST(scf_carry_flag_cleared);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();

  RUN_SUITE(palette_idx_suite);
  RUN_SUITE(add_suite);
  RUN_SUITE(sub_suite);
  RUN_SUITE(inc_suite);
  RUN_SUITE(dec_suite);
  RUN_SUITE(and_suite);
  RUN_SUITE(adc_suite);
  RUN_SUITE(ccf_suite);
  RUN_SUITE(scf_suite);

  GREATEST_MAIN_END();
}

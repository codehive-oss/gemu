#include "./instruction.h"
#include "types.h"
#include "util.h"
#include <stdio.h>

// Loads 8-bit data to any 8-bit register
void ld_regd8_d8(EmulationState *emu, u8 *reg) {
  u8 target = emu->rom[*emu->pc];
  *reg = target;
  *emu->pc += 1;
}

// Loads 16-bit data to any 16-bit register
void ld_regd16_d16(EmulationState *emu, u16 *reg) {
  u16 target = *(u16 *)&emu->rom[*emu->pc];
  *reg = target;
  *emu->pc += 2;
}

// Loads 8-bit data from any the memory address pointed by any 16-bit register
void ld_regd8_rega16(EmulationState *emu, u8 *target, u16 *from) {
  *target = emu->mem[*from];
}

// Loads 8-bit register data to any the memory address pointed by any 16-bit
// register
void ld_rega16_regd8(EmulationState *emu, u16 *target, u8 *from) {
  emu->mem[*target] = *from;
}

// Loads 8-bit data to any the memory address pointed by any 16-bit register
void ld_rega16_d8(EmulationState *emu, u16 *target) {
  u8 from = emu->rom[*emu->pc];
  emu->mem[*target] = from;
  *emu->pc += 1;
}

// AND with 8-bit register
void and_regd8(EmulationState *emu, u8 *reg) {
  *emu->a &= *reg;
  set_flags(emu, *emu->a == 0, 0, 1, 0);
}

// OR with 8-bit register
void or_regd8(EmulationState *emu, u8 *reg) {
  *emu->a |= *reg;
  set_flags(emu, *emu->a == 0, 0, 0, 0);
}

// XOR with 8-bit register
void xor_regd8(EmulationState *emu, u8 *reg) {
  *emu->a ^= *reg;
  set_flags(emu, *emu->a == 0, 0, 0, 0);
}

// Decrement 1 from 8-bit register
void dec_regd8(EmulationState *emu, u8 *reg) {
  *reg -= 1;
  set_flags(emu, *reg == 0, 1, (*reg & 0x0F) == 0x0F, -1);
}

// Increment 1 from 8-bit register
void inc_regd8(EmulationState *emu, u8 *reg) {
  *reg += 1;
  set_flags(emu, *reg == 0, 1, (*reg & 0x0F) == 0x00, -1);
}

void add_regd8(EmulationState *emu, u8 *from) {
  bool h = (*emu->a & 0x0F) + (*from & 0x0F) > 0x0F;
  bool c = *emu->a + *from > 0xFF;

  *emu->a += *from;
  set_flags(emu, *emu->a == 0, 0, h, c);
}

void sub_regd8(EmulationState *emu, u8 *from) {
  bool h = (*emu->a & 0x0F) - (*from & 0x0F) < 0;
  bool c = *emu->a - *from < 0;

  *emu->a -= *from;
  set_flags(emu, *emu->a == 0, 1, h, c);
}

void nop(EmulationState *emu) {}

// https://gbdev.io/pandocs/Interrupts.html
void di(EmulationState *emu) { *emu->ie = 0; }

void jp_a16(EmulationState *emu) {
  u16 target = *(u16 *)&emu->rom[*emu->pc];
  *emu->pc = target;
}

void jr_d8(EmulationState *emu) {
  i8 target = emu->rom[*emu->pc];
  *emu->pc += target;
}

void jr_nz_d8(EmulationState *emu) {
  if (!(*emu->f & 0b10000000)) { // Z is not set
    jr_d8(emu);
  } else {
    *emu->pc += 1;
  }
}

void jp_c_a16(EmulationState *emu) {
  if (*emu->f & 0b00010000) { // C is set
    jp_a16(emu);
  } else {
    *emu->pc += 2;
  }
}

void jp_nz_a16(EmulationState *emu) {
  if (!(*emu->f & 0b10000000)) { // Z is not set
    jp_a16(emu);
  } else {
    *emu->pc += 2;
  }
}

void ldh_a8_a(EmulationState *emu) {
  u8 target = emu->rom[*emu->pc];
  emu->io[target] = *emu->a;
  *emu->pc += 1;
}

void ldh_a_a8(EmulationState *emu) {
  u8 target = emu->rom[*emu->pc];
  *emu->a = emu->io[target];
  *emu->pc += 1;
}

void ld_hl_d8(EmulationState *emu) { ld_rega16_d8(emu, emu->hl); }

void inc_bc(EmulationState *emu) { *emu->bc += 1; }
void inc_de(EmulationState *emu) { *emu->de += 1; }
void inc_hl(EmulationState *emu) { *emu->hl += 1; }
void inc_sp(EmulationState *emu) { *emu->sp += 1; }

void inc_b(EmulationState *emu) { inc_regd8(emu, emu->b); }
void inc_d(EmulationState *emu) { inc_regd8(emu, emu->d); }
void inc_h(EmulationState *emu) { inc_regd8(emu, emu->h); }

void inc_c(EmulationState *emu) { inc_regd8(emu, emu->c); }
void inc_e(EmulationState *emu) { inc_regd8(emu, emu->e); }
void inc_l(EmulationState *emu) { inc_regd8(emu, emu->l); }
void inc_a(EmulationState *emu) { inc_regd8(emu, emu->a); }

void dec_bc(EmulationState *emu) { *emu->bc -= 1; }
void dec_de(EmulationState *emu) { *emu->de -= 1; }
void dec_hl(EmulationState *emu) { *emu->hl -= 1; }
void dec_sp(EmulationState *emu) { *emu->sp -= 1; }

void dec_b(EmulationState *emu) { dec_regd8(emu, emu->b); }
void dec_d(EmulationState *emu) { dec_regd8(emu, emu->d); }
void dec_h(EmulationState *emu) { dec_regd8(emu, emu->h); }

void dec_c(EmulationState *emu) { dec_regd8(emu, emu->c); }
void dec_e(EmulationState *emu) { dec_regd8(emu, emu->e); }
void dec_l(EmulationState *emu) { dec_regd8(emu, emu->l); }
void dec_a(EmulationState *emu) { dec_regd8(emu, emu->a); }

void ld_b_b(EmulationState *emu) { *emu->b = *emu->b; }
void ld_d_b(EmulationState *emu) { *emu->d = *emu->b; }
void ld_h_b(EmulationState *emu) { *emu->h = *emu->b; }

void ld_b_c(EmulationState *emu) { *emu->b = *emu->c; }
void ld_d_c(EmulationState *emu) { *emu->d = *emu->c; }
void ld_h_c(EmulationState *emu) { *emu->h = *emu->c; }

void ld_b_d(EmulationState *emu) { *emu->b = *emu->d; }
void ld_d_d(EmulationState *emu) { *emu->d = *emu->d; }
void ld_h_d(EmulationState *emu) { *emu->h = *emu->d; }

void ld_b_e(EmulationState *emu) { *emu->b = *emu->e; }
void ld_d_e(EmulationState *emu) { *emu->d = *emu->e; }
void ld_h_e(EmulationState *emu) { *emu->h = *emu->e; }

void ld_b_h(EmulationState *emu) { *emu->b = *emu->h; }
void ld_d_h(EmulationState *emu) { *emu->d = *emu->h; }
void ld_h_h(EmulationState *emu) { *emu->h = *emu->h; }

void ld_b_l(EmulationState *emu) { *emu->b = *emu->l; }
void ld_d_l(EmulationState *emu) { *emu->d = *emu->l; }
void ld_h_l(EmulationState *emu) { *emu->h = *emu->l; }

void ld_b_a(EmulationState *emu) { *emu->b = *emu->a; }
void ld_d_a(EmulationState *emu) { *emu->d = *emu->a; }
void ld_h_a(EmulationState *emu) { *emu->h = *emu->a; }

void ld_c_b(EmulationState *emu) { *emu->c = *emu->b; }
void ld_e_b(EmulationState *emu) { *emu->e = *emu->b; }
void ld_l_b(EmulationState *emu) { *emu->l = *emu->b; }
void ld_a_b(EmulationState *emu) { *emu->a = *emu->b; }

void ld_c_c(EmulationState *emu) { *emu->c = *emu->c; }
void ld_e_c(EmulationState *emu) { *emu->e = *emu->c; }
void ld_l_c(EmulationState *emu) { *emu->l = *emu->c; }
void ld_a_c(EmulationState *emu) { *emu->a = *emu->c; }

void ld_c_d(EmulationState *emu) { *emu->c = *emu->d; }
void ld_e_d(EmulationState *emu) { *emu->e = *emu->d; }
void ld_l_d(EmulationState *emu) { *emu->l = *emu->d; }
void ld_a_d(EmulationState *emu) { *emu->a = *emu->d; }

void ld_c_e(EmulationState *emu) { *emu->c = *emu->e; }
void ld_e_e(EmulationState *emu) { *emu->e = *emu->e; }
void ld_l_e(EmulationState *emu) { *emu->l = *emu->e; }
void ld_a_e(EmulationState *emu) { *emu->a = *emu->e; }

void ld_c_h(EmulationState *emu) { *emu->c = *emu->h; }
void ld_e_h(EmulationState *emu) { *emu->e = *emu->h; }
void ld_l_h(EmulationState *emu) { *emu->l = *emu->h; }
void ld_a_h(EmulationState *emu) { *emu->a = *emu->h; }

void ld_c_l(EmulationState *emu) { *emu->c = *emu->l; }
void ld_e_l(EmulationState *emu) { *emu->e = *emu->l; }
void ld_l_l(EmulationState *emu) { *emu->l = *emu->l; }
void ld_a_l(EmulationState *emu) { *emu->a = *emu->l; }

void ld_c_a(EmulationState *emu) { *emu->c = *emu->a; }
void ld_e_a(EmulationState *emu) { *emu->e = *emu->a; }
void ld_l_a(EmulationState *emu) { *emu->l = *emu->a; }
void ld_a_a(EmulationState *emu) { *emu->a = *emu->a; }

void ld_a16_a(EmulationState *emu) {
  u16 target_addr = *(u16 *)&emu->rom[*emu->pc];
  emu->mem[target_addr] = *emu->a;
  *emu->pc += 2;
}

void ld_a_a16(EmulationState *emu) {
  u16 target_addr = *(u16 *)&emu->rom[*emu->pc];
  *emu->a = emu->mem[target_addr];
  *emu->pc += 2;
}

void ld_b_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->b); }
void ld_d_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->d); }
void ld_h_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->h); }

void ld_c_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->c); }
void ld_e_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->e); }
void ld_l_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->l); }
void ld_a_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->a); }

void ld_bc_d16(EmulationState *emu) { ld_regd16_d16(emu, emu->bc); }
void ld_de_d16(EmulationState *emu) { ld_regd16_d16(emu, emu->de); }
void ld_hl_d16(EmulationState *emu) { ld_regd16_d16(emu, emu->hl); }
void ld_sp_d16(EmulationState *emu) { ld_regd16_d16(emu, emu->sp); }

void ld_a_de(EmulationState *emu) { ld_regd8_rega16(emu, emu->a, emu->de); }

void ld_bc_a(EmulationState *emu) { ld_rega16_regd8(emu, emu->bc, emu->a); }
void ld_de_a(EmulationState *emu) { ld_rega16_regd8(emu, emu->de, emu->a); }
void ld_hli_a(EmulationState *emu) {
  ld_rega16_regd8(emu, emu->hl, emu->a);
  inc_hl(emu);
}
void ld_hld_a(EmulationState *emu) {
  ld_rega16_regd8(emu, emu->hl, emu->a);
  dec_hl(emu);
}

void add_b(EmulationState *emu) { add_regd8(emu, emu->b); }
void add_c(EmulationState *emu) { add_regd8(emu, emu->c); }
void add_d(EmulationState *emu) { add_regd8(emu, emu->d); }
void add_e(EmulationState *emu) { add_regd8(emu, emu->e); }
void add_h(EmulationState *emu) { add_regd8(emu, emu->h); }
void add_l(EmulationState *emu) { add_regd8(emu, emu->l); }
void add_a(EmulationState *emu) { add_regd8(emu, emu->a); }

void sub_b(EmulationState *emu) { sub_regd8(emu, emu->b); }
void sub_c(EmulationState *emu) { sub_regd8(emu, emu->c); }
void sub_d(EmulationState *emu) { sub_regd8(emu, emu->d); }
void sub_e(EmulationState *emu) { sub_regd8(emu, emu->e); }
void sub_h(EmulationState *emu) { sub_regd8(emu, emu->h); }
void sub_l(EmulationState *emu) { sub_regd8(emu, emu->l); }
void sub_a(EmulationState *emu) { sub_regd8(emu, emu->a); }

void and_b(EmulationState *emu) { and_regd8(emu, emu->b); }
void and_c(EmulationState *emu) { and_regd8(emu, emu->c); }
void and_d(EmulationState *emu) { and_regd8(emu, emu->d); }
void and_e(EmulationState *emu) { and_regd8(emu, emu->e); }
void and_h(EmulationState *emu) { and_regd8(emu, emu->h); }
void and_l(EmulationState *emu) { and_regd8(emu, emu->l); }
void and_a(EmulationState *emu) { and_regd8(emu, emu->a); }

void or_b(EmulationState *emu) { or_regd8(emu, emu->b); }
void or_c(EmulationState *emu) { or_regd8(emu, emu->c); }
void or_d(EmulationState *emu) { or_regd8(emu, emu->d); }
void or_e(EmulationState *emu) { or_regd8(emu, emu->e); }
void or_h(EmulationState *emu) { or_regd8(emu, emu->h); }
void or_l(EmulationState *emu) { or_regd8(emu, emu->l); }
void or_a(EmulationState *emu) { or_regd8(emu, emu->a); }

void xor_b(EmulationState *emu) { xor_regd8(emu, emu->b); }
void xor_c(EmulationState *emu) { xor_regd8(emu, emu->c); }
void xor_d(EmulationState *emu) { xor_regd8(emu, emu->d); }
void xor_e(EmulationState *emu) { xor_regd8(emu, emu->e); }
void xor_h(EmulationState *emu) { xor_regd8(emu, emu->h); }
void xor_l(EmulationState *emu) { xor_regd8(emu, emu->l); }
void xor_a(EmulationState *emu) { xor_regd8(emu, emu->a); }

void cp_d8(EmulationState *emu) {
  u8 value = emu->rom[*emu->pc];
  if (*emu->a == value) {
    set_flags(emu, 1, 1, 0, 0);
  } else if (*emu->a > value) {
    set_flags(emu, 0, 1, 0, 0);
  } else {
    set_flags(emu, 0, 1, 1, 1);
  }
  *emu->pc += 1;
}

// TODO: Put the Instructions in encoding order
// TODO: Fix mcycle, because it depends on the operation executed
Instruction GB_INSTRUCTIONS[GB_INSTRUCTIONS_LENGTH] = {
    {.encoding = 0x00, .mcycle = 1, .execute = &nop},
    {.encoding = 0xF3, .mcycle = 1, .execute = &di},

    {.encoding = 0x01, .mcycle = 3, .execute = &ld_bc_d16},
    {.encoding = 0x11, .mcycle = 3, .execute = &ld_de_d16},
    {.encoding = 0x21, .mcycle = 3, .execute = &ld_hl_d16},
    {.encoding = 0x31, .mcycle = 3, .execute = &ld_sp_d16},

    {.encoding = 0x40, .mcycle = 1, .execute = &ld_b_b},
    {.encoding = 0x50, .mcycle = 1, .execute = &ld_d_b},
    {.encoding = 0x60, .mcycle = 1, .execute = &ld_h_b},

    {.encoding = 0x41, .mcycle = 1, .execute = &ld_b_c},
    {.encoding = 0x51, .mcycle = 1, .execute = &ld_d_c},
    {.encoding = 0x61, .mcycle = 1, .execute = &ld_h_c},

    {.encoding = 0x42, .mcycle = 1, .execute = &ld_b_d},
    {.encoding = 0x52, .mcycle = 1, .execute = &ld_d_d},
    {.encoding = 0x62, .mcycle = 1, .execute = &ld_h_d},

    {.encoding = 0x43, .mcycle = 1, .execute = &ld_b_e},
    {.encoding = 0x53, .mcycle = 1, .execute = &ld_d_e},
    {.encoding = 0x63, .mcycle = 1, .execute = &ld_h_e},

    {.encoding = 0x44, .mcycle = 1, .execute = &ld_b_h},
    {.encoding = 0x54, .mcycle = 1, .execute = &ld_d_h},
    {.encoding = 0x64, .mcycle = 1, .execute = &ld_h_h},

    {.encoding = 0x45, .mcycle = 1, .execute = &ld_b_l},
    {.encoding = 0x55, .mcycle = 1, .execute = &ld_d_l},
    {.encoding = 0x65, .mcycle = 1, .execute = &ld_h_l},

    {.encoding = 0x47, .mcycle = 1, .execute = &ld_b_a},
    {.encoding = 0x57, .mcycle = 1, .execute = &ld_d_a},
    {.encoding = 0x67, .mcycle = 1, .execute = &ld_h_a},

    {.encoding = 0x48, .mcycle = 1, .execute = &ld_c_b},
    {.encoding = 0x58, .mcycle = 1, .execute = &ld_e_b},
    {.encoding = 0x68, .mcycle = 1, .execute = &ld_l_b},
    {.encoding = 0x78, .mcycle = 1, .execute = &ld_a_b},

    {.encoding = 0x49, .mcycle = 1, .execute = &ld_c_c},
    {.encoding = 0x59, .mcycle = 1, .execute = &ld_e_c},
    {.encoding = 0x69, .mcycle = 1, .execute = &ld_l_c},
    {.encoding = 0x79, .mcycle = 1, .execute = &ld_a_c},

    {.encoding = 0x4A, .mcycle = 1, .execute = &ld_c_d},
    {.encoding = 0x5A, .mcycle = 1, .execute = &ld_e_d},
    {.encoding = 0x6A, .mcycle = 1, .execute = &ld_l_d},
    {.encoding = 0x7A, .mcycle = 1, .execute = &ld_a_d},

    {.encoding = 0x4B, .mcycle = 1, .execute = &ld_c_e},
    {.encoding = 0x5B, .mcycle = 1, .execute = &ld_e_e},
    {.encoding = 0x6B, .mcycle = 1, .execute = &ld_l_e},
    {.encoding = 0x7B, .mcycle = 1, .execute = &ld_a_e},

    {.encoding = 0x4C, .mcycle = 1, .execute = &ld_c_h},
    {.encoding = 0x5C, .mcycle = 1, .execute = &ld_e_h},
    {.encoding = 0x6C, .mcycle = 1, .execute = &ld_l_h},
    {.encoding = 0x7C, .mcycle = 1, .execute = &ld_a_h},

    {.encoding = 0x4D, .mcycle = 1, .execute = &ld_c_l},
    {.encoding = 0x5D, .mcycle = 1, .execute = &ld_e_l},
    {.encoding = 0x6D, .mcycle = 1, .execute = &ld_l_l},
    {.encoding = 0x7D, .mcycle = 1, .execute = &ld_a_l},

    {.encoding = 0x4F, .mcycle = 1, .execute = &ld_c_a},
    {.encoding = 0x5F, .mcycle = 1, .execute = &ld_e_a},
    {.encoding = 0x6F, .mcycle = 1, .execute = &ld_l_a},
    {.encoding = 0x7F, .mcycle = 1, .execute = &ld_a_a},

    {.encoding = 0x02, .mcycle = 2, .execute = &ld_bc_a},
    {.encoding = 0x12, .mcycle = 2, .execute = &ld_de_a},
    {.encoding = 0x22, .mcycle = 2, .execute = &ld_hli_a},
    {.encoding = 0x32, .mcycle = 2, .execute = &ld_hld_a},

    {.encoding = 0x03, .mcycle = 2, .execute = &inc_bc},
    {.encoding = 0x13, .mcycle = 2, .execute = &inc_de},
    {.encoding = 0x23, .mcycle = 2, .execute = &inc_hl},
    {.encoding = 0x33, .mcycle = 2, .execute = &inc_sp},

    {.encoding = 0x04, .mcycle = 1, .execute = &inc_b},
    {.encoding = 0x14, .mcycle = 1, .execute = &inc_d},
    {.encoding = 0x24, .mcycle = 1, .execute = &inc_h},

    {.encoding = 0x0C, .mcycle = 1, .execute = &inc_c},
    {.encoding = 0x1C, .mcycle = 1, .execute = &inc_e},
    {.encoding = 0x2C, .mcycle = 1, .execute = &inc_l},
    {.encoding = 0x3C, .mcycle = 1, .execute = &inc_a},

    {.encoding = 0x05, .mcycle = 1, .execute = &dec_b},
    {.encoding = 0x15, .mcycle = 1, .execute = &dec_d},
    {.encoding = 0x25, .mcycle = 1, .execute = &dec_h},

    {.encoding = 0x0D, .mcycle = 1, .execute = &dec_c},
    {.encoding = 0x1D, .mcycle = 1, .execute = &dec_e},
    {.encoding = 0x2D, .mcycle = 1, .execute = &dec_l},
    {.encoding = 0x3D, .mcycle = 1, .execute = &dec_a},

    {.encoding = 0x0B, .mcycle = 2, .execute = &dec_bc},
    {.encoding = 0x1B, .mcycle = 2, .execute = &dec_de},
    {.encoding = 0x2B, .mcycle = 2, .execute = &dec_hl},
    {.encoding = 0x3B, .mcycle = 2, .execute = &dec_sp},

    {.encoding = 0x1A, .mcycle = 2, .execute = &ld_a_de},

    {.encoding = 0xC3, .mcycle = 4, .execute = &jp_a16},

    {.encoding = 0x06, .mcycle = 2, .execute = &ld_b_d8},
    {.encoding = 0x16, .mcycle = 2, .execute = &ld_d_d8},
    {.encoding = 0x26, .mcycle = 2, .execute = &ld_h_d8},
    {.encoding = 0x36, .mcycle = 2, .execute = &ld_hl_d8},

    {.encoding = 0x0E, .mcycle = 2, .execute = &ld_c_d8},
    {.encoding = 0x1E, .mcycle = 2, .execute = &ld_e_d8},
    {.encoding = 0x2E, .mcycle = 2, .execute = &ld_l_d8},
    {.encoding = 0x3E, .mcycle = 2, .execute = &ld_a_d8},

    {.encoding = 0xEA, .mcycle = 4, .execute = &ld_a16_a},
    {.encoding = 0xFA, .mcycle = 4, .execute = &ld_a_a16},

    {.encoding = 0xE0, .mcycle = 3, .execute = &ldh_a8_a},
    {.encoding = 0xF0, .mcycle = 3, .execute = &ldh_a_a8},

    {.encoding = 0x80, .mcycle = 1, .execute = &add_b},
    {.encoding = 0x81, .mcycle = 1, .execute = &add_c},
    {.encoding = 0x82, .mcycle = 1, .execute = &add_d},
    {.encoding = 0x83, .mcycle = 1, .execute = &add_e},
    {.encoding = 0x84, .mcycle = 1, .execute = &add_h},
    {.encoding = 0x85, .mcycle = 1, .execute = &add_l},
    {.encoding = 0x87, .mcycle = 1, .execute = &add_a},

    {.encoding = 0x90, .mcycle = 1, .execute = &sub_b},
    {.encoding = 0x91, .mcycle = 1, .execute = &sub_c},
    {.encoding = 0x92, .mcycle = 1, .execute = &sub_d},
    {.encoding = 0x93, .mcycle = 1, .execute = &sub_e},
    {.encoding = 0x94, .mcycle = 1, .execute = &sub_h},
    {.encoding = 0x95, .mcycle = 1, .execute = &sub_l},
    {.encoding = 0x97, .mcycle = 1, .execute = &sub_a},

    {.encoding = 0xA0, .mcycle = 1, .execute = &and_b},
    {.encoding = 0xA1, .mcycle = 1, .execute = &and_c},
    {.encoding = 0xA2, .mcycle = 1, .execute = &and_d},
    {.encoding = 0xA3, .mcycle = 1, .execute = &and_e},
    {.encoding = 0xA4, .mcycle = 1, .execute = &and_h},
    {.encoding = 0xA5, .mcycle = 1, .execute = &and_l},
    {.encoding = 0xA7, .mcycle = 1, .execute = &and_a},

    {.encoding = 0xA8, .mcycle = 1, .execute = &xor_b},
    {.encoding = 0xA9, .mcycle = 1, .execute = &xor_c},
    {.encoding = 0xAA, .mcycle = 1, .execute = &xor_d},
    {.encoding = 0xAB, .mcycle = 1, .execute = &xor_e},
    {.encoding = 0xAC, .mcycle = 1, .execute = &xor_h},
    {.encoding = 0xAD, .mcycle = 1, .execute = &xor_l},
    {.encoding = 0xAF, .mcycle = 1, .execute = &xor_a},

    {.encoding = 0xB0, .mcycle = 1, .execute = &or_b},
    {.encoding = 0xB1, .mcycle = 1, .execute = &or_c},
    {.encoding = 0xB2, .mcycle = 1, .execute = &or_d},
    {.encoding = 0xB3, .mcycle = 1, .execute = &or_e},
    {.encoding = 0xB4, .mcycle = 1, .execute = &or_h},
    {.encoding = 0xB5, .mcycle = 1, .execute = &or_l},
    {.encoding = 0xB7, .mcycle = 1, .execute = &or_a},

    {.encoding = 0xFE, .mcycle = 2, .execute = &cp_d8},

    {.encoding = 0xDA, .mcycle = 4, .execute = &jp_c_a16},
    {.encoding = 0xC2, .mcycle = 4, .execute = &jp_nz_a16},

    {.encoding = 0xB1, .mcycle = 4, .execute = &or_c},
    {.encoding = 0xAF, .mcycle = 1, .execute = &xor_a},

    {.encoding = 0x18, .mcycle = 3, .execute = &jr_d8},
    {.encoding = 0x20, .mcycle = 3, .execute = &jr_nz_d8},
};

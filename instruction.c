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

// Loads 8-bit data to any the memory address pointed by any 16-bit register
void ld_rega16_regd8(EmulationState *emu, u16 *target, u8 *from) {
  emu->mem[*target] = *from;
}

void nop(EmulationState *emu) {}

void jp(EmulationState *emu) {
  u16 target = *(u16 *)&emu->rom[*emu->pc];
  *emu->pc = target;
}

void jp_c_a16(EmulationState *emu) {
  if (*emu->f & 0b00010000) { // C is set
    u16 target = *(u16 *)&emu->rom[*emu->pc];
    *emu->pc = target;
  } else {
    *emu->pc += 2;
  }
}

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
  *emu->hl += 1;
}
void ld_hld_a(EmulationState *emu) {
  ld_rega16_regd8(emu, emu->hl, emu->a);
  *emu->hl -= 1;
}

void inc_bc(EmulationState *emu) { *emu->bc += 1; }
void inc_de(EmulationState *emu) { *emu->de += 1; }
void inc_hl(EmulationState *emu) { *emu->hl += 1; }
void inc_sp(EmulationState *emu) { *emu->sp += 1; }

void dec_bc(EmulationState *emu) { *emu->bc -= 1; }
void dec_de(EmulationState *emu) { *emu->de -= 1; }
void dec_hl(EmulationState *emu) { *emu->hl -= 1; }
void dec_sp(EmulationState *emu) { *emu->sp -= 1; }

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
Instruction GB_INSTRUCTIONS[GB_INSTRUCTIONS_LENGTH] = {
    {.encoding = 0x00, .mcycle = 1, .execute = &nop},

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

    {.encoding = 0x0B, .mcycle = 2, .execute = &dec_bc},
    {.encoding = 0x1B, .mcycle = 2, .execute = &dec_de},
    {.encoding = 0x2B, .mcycle = 2, .execute = &dec_hl},
    {.encoding = 0x3B, .mcycle = 2, .execute = &dec_sp},

    {.encoding = 0x1A, .mcycle = 2, .execute = &ld_a_de},

    {.encoding = 0xC3, .mcycle = 4, .execute = &jp},

    {.encoding = 0x3E, .mcycle = 2, .execute = &ld_a_d8},

    {.encoding = 0xEA, .mcycle = 4, .execute = &ld_a16_a},

    {.encoding = 0xFE, .mcycle = 2, .execute = &cp_d8},

    {.encoding = 0xFA, .mcycle = 4, .execute = &ld_a_a16},

    {.encoding = 0xDA, .mcycle = 4, .execute = &jp_c_a16},
};

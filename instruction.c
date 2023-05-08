#include "./instruction.h"
#include "types.h"
#include "util.h"

// Loads 8-bit data to any 8-bit register
void ld_reg8_d8(EmulationState *emu, u8 *reg) {
  u8 target = emu->rom[*emu->pc];
  *reg = target;
  *emu->pc += 1;
}

// Loads 16-bit data to any 16-bit register
void ld_reg16_d16(EmulationState *emu, u16 *reg) {
  u16 target = *(u16 *)&emu->rom[*emu->pc];
  *reg = target;
  *emu->pc += 2;
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

void ld_a_d8(EmulationState *emu) { ld_reg8_d8(emu, emu->a); }

void ld_bc_d16(EmulationState *emu) { ld_reg16_d16(emu, emu->bc); }
void ld_de_d16(EmulationState *emu) { ld_reg16_d16(emu, emu->de); }
void ld_hl_d16(EmulationState *emu) { ld_reg16_d16(emu, emu->hl); }

void cp_d8(EmulationState *emu) {
  u8 value = emu->rom[*emu->pc];
  if (*emu->a == value) {
    *emu->f |= 0b11000000; // Z=1, N=1
    *emu->f &= 0b11001111; // H=0, C=0
  } else if (*emu->a > value) {
    *emu->f |= 0b01000000; // N=1
    *emu->f &= 0b01001111; // Z=0, H=0, C=0
  } else {
    *emu->f &= 0b01111111; // Z=0
    *emu->f |= 0b01110000; // N=1, H=1, C=1
  }
  *emu->pc += 1;
}

// TODO: Put the Instructions in encoding order
Instruction GB_INSTRUCTIONS[GB_INSTRUCTIONS_LENGTH] = {
    {.encoding = 0x00, .mcycle = 1, .execute = &nop},
    {.encoding = 0x01, .mcycle = 3, .execute = &ld_bc_d16},
    {.encoding = 0x11, .mcycle = 3, .execute = &ld_de_d16},
    {.encoding = 0x21, .mcycle = 3, .execute = &ld_hl_d16},
    {.encoding = 0xC3, .mcycle = 4, .execute = &jp},
    {.encoding = 0x40, .mcycle = 1, .execute = &ld_b_b},
    {.encoding = 0x50, .mcycle = 1, .execute = &ld_d_b},
    {.encoding = 0x60, .mcycle = 1, .execute = &ld_h_b},
    {.encoding = 0x3E, .mcycle = 2, .execute = &ld_a_d8},
    {.encoding = 0xEA, .mcycle = 4, .execute = &ld_a16_a},
    {.encoding = 0xFA, .mcycle = 4, .execute = &ld_a_a16},
    {.encoding = 0xFE, .mcycle = 2, .execute = &cp_d8},
    {.encoding = 0xDA, .mcycle = 4, .execute = &jp_c_a16},
};

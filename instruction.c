#include "./instruction.h"
#include "types.h"
#include "util.h"

static void nop(EmulationState *emu) {
  // Step to next instruction
  *emu->pc += 1;
}

static void jp(EmulationState *emu) {
  (*emu->pc)++;
  u16 target = *(u16 *)&emu->mem[*emu->pc];
  *emu->pc = target;
}

static void ld_b_b(EmulationState *emu) {
  *emu->b = *emu->b;
  *emu->pc += 1;
}

static void ld_d_b(EmulationState *emu) {
  *emu->d = *emu->b;
  *emu->pc += 1;
}

void ld_h_b(EmulationState *emu) {
  *emu->h = *emu->b;
  *emu->pc += 1;
}

Instruction GB_INSTRUCTIONS[GB_INSTRUCTIONS_LENGTH] = {
    // nop
    {
        .encoding = 0x00,
        .mcycle = 1,
        .execute = &nop,
    },
    // jp
    {
        .encoding = 0xc3,
        .mcycle = 4,
        .execute = &jp,
    },
    // ld b, b
    {
        .encoding = 0x40,
        .mcycle = 1,
        .execute = &ld_b_b,
    },
    // ld d, b
    {
        .encoding = 0x50,
        .mcycle = 1,
        .execute = &ld_d_b,
    },
    // ld h, b
    {
        .encoding = 0x60,
        .mcycle = 1,
        .execute = &ld_h_b,
    },
};

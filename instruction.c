#include "./instruction.h"
#include "util.h"

void nop(EmulationState *emu) {
  // Step to next instruction
  (*emu->pc)++;
}

void jp(EmulationState *emu) {
  (*emu->pc)++;
  u16 target = *(u16 *)&emu->mem[*emu->pc];
  *emu->pc = target;
}

Instruction GB_INSTRUCTIONS[GB_INSTRUCTIONS_LENGTH] = {
    // nop
    {
        .encoding = 0x00,
        .mcycle = 1,
        .execute = &nop,
    },
    {
        .encoding = 0xc3,
        .mcycle = 4,
        .execute = &jp,
    }};

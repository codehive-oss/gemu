#include "./instruction.h"
#include "util.h"

void nop(EmulationState *emu) {}

void jp(EmulationState *emu) {
  (*emu->pc)++;
  u16 target = *(u16 *)&emu->mem[*emu->pc];
  *emu->pc = target;

  emu->mcycle += 4;
}

Instruction GB_INSTRUCTIONS[GB_INSTRUCTIONS_LENGTH] = {
    // nop
    {
        .encoding = 0x00,
        .length = 1,
        .mcycle = 1,
        .execute = &nop,
    },
    {
        .encoding = 0xc3,
        .length = 3,
        .mcycle = 4,
        .execute = &jp,
    }};

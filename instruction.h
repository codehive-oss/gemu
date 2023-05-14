#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "./types.h"
#include "util.h"

typedef struct Instruction {
  u8 encoding;
  u8 mcycle;
  void (*execute)(EmulationState *);
} Instruction;

#define GB_INSTRUCTIONS_LENGTH 137

extern Instruction GB_INSTRUCTIONS[GB_INSTRUCTIONS_LENGTH];

void add_regd8(EmulationState *emu, u8 *from);
void sub_regd8(EmulationState *emu, u8 *from);

#endif // INSTRUCTION_H

#ifndef EMULATION_H
#define EMULATION_H
#include "./util.h"

#define START_WITH_STEP true

bool handle_interrupt(EmulationState *emu);
bool handle_instruction(EmulationState *emu, u8 inst);
void handle_gameinput(EmulationState *emu, GameInput *input);
void print_header(EmulationState *emu);

#endif // EMULATION_H

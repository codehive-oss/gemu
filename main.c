#include "./instruction.h"
#include "./util.h"
#include "types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void handle_instruction(EmulationState *emu, u8 instructionEncoding) {
  printf("Running Instruction: %hhX\n", instructionEncoding);

  for (size_t i = 0; i < GB_INSTRUCTIONS_LENGTH; i++) {
    Instruction instruction = GB_INSTRUCTIONS[i];
    if (instruction.encoding == instructionEncoding) {
      instruction.execute(emu);
      emu->mcycle += instruction.mcycle;
      return;
    }
  }

  printf("Instruction not found: %hhX\n", instructionEncoding);
  emu->running = false;
}

int main(void) {
  EmulationState *emu = emu_init();

  read_file("main.gb", emu->rom);

  printf("-------------START HEADERS--------------\n");
  printf("Title: %s\n", emu->header->title);
  printf("Rom Type: %s\n", ROM_TYPES[(u8)emu->header->type]);
  printf("Rom Size: %dKB\n", 32 * (1 << emu->header->rom_size));
  u8 checksum =
      0; // https://gbdev.io/pandocs/The_Cartridge_Header.html#014d--header-checksum
  for (u16 address = 0x0134; address <= 0x014C; address++)
    checksum = checksum - emu->rom[address] - 1;

  printf("Header Checksum: %d\n", emu->header->checksum);
  printf("Computed Value:  %d\n", checksum);
  printf("--------------END HEADERS---------------\n\n");

  printf("-------------START PROGRAM--------------\n");
  *emu->pc = 0x0100;
  while (emu->running) {
    u8 nextInst = emu->mem[*emu->pc];
    handle_instruction(emu, nextInst);
  }
  printf("--------------END PROGRAM---------------\n\n");

  printf("Instruction pointer\n");
  PRINT_BYTES(*emu->pc);

  emu_free(emu);

  return 0;
}

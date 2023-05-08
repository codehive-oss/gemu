#include "./instruction.h"
#include "./util.h"
#include "types.h"
#include <stdbool.h>
#include <stdio.h>

void handle_instruction(EmulationState *emu, u8 inst) {
  *emu->pc += 1;
  for (size_t i = 0; i < GB_INSTRUCTIONS_LENGTH; i++) {
    Instruction instruction = GB_INSTRUCTIONS[i];
    if (instruction.encoding == inst) {
      instruction.execute(emu);
      emu->mcycle += instruction.mcycle;
      emu->mem[rLY] = emu->mcycle % 154;
      return;
    }
  }

  printf("Instruction not found: %2X\n", inst);
  printf("Terminating...\n");
  emu->running = false;
}

int main(void) {
  bool wait_for_step = false;
  EmulationState *emu = emu_init();

  read_file("main.gb", emu->rom);

  printf("-------------START HEADERS--------------\n");
  printf("Title: %s\n", emu->header->title);
  printf("Rom Type: %s\n", ROM_TYPES[(u8)emu->header->type]);
  printf("Rom Size: %dKB\n", 32 * (1 << emu->header->rom_size));

  // https://gbdev.io/pandocs/The_Cartridge_Header.html#014d--header-checksum
  u8 checksum = 0;
  for (u16 address = 0x0134; address <= 0x014C; address++)
    checksum = checksum - emu->rom[address] - 1;

  printf("Header Checksum: %d\n", emu->header->checksum);
  printf("Computed Value:  %d\n", checksum);
  printf("--------------END HEADERS---------------\n\n");

  printf("-------------START PROGRAM--------------\n");
  *emu->pc = 0x0100;
  while (emu->running) {
    u8 inst = emu->rom[*emu->pc];
    printf("Inst: %02X\tAt: ", inst);
    PRINT_BYTES(*emu->pc);

    if (wait_for_step) {
      getch();
      emu_print(emu);
    }

    handle_instruction(emu, inst);
  }
  printf("--------------END PROGRAM---------------\n\n");

  printf("-------------EmulationState-------------\n");
  emu_print(emu);

  emu_free(emu);

  return 0;
}

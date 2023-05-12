#include "./instruction.h"
#include "./util.h"
#include "types.h"
#include "ui.h"
#include <stdbool.h>
#include <stdio.h>

#define WAIT_FOR_STEP false

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

  printf("Instruction not found: %02X\n", inst);
  printf("Terminating...\n");
  emu->running = false;
}

int main(int argc, char **argv) {
  const char *path = "main.gb";
  if (argc > 1) {
    path = argv[1];
  }

  EmulationState *emu = emu_init();

  read_file(path, emu->rom);

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
  Window *win = win_init();

  *emu->pc = 0x0100;
  for (int frame = 0; emu->running; frame++) {
    u8 inst = emu->rom[*emu->pc];
    printf("Inst: %02X\tAt: ", inst);
    PRINT_BYTES(*emu->pc);

    if (WAIT_FOR_STEP) {
      emu_print(emu);
      getch();
    }

    if (frame % 128 == 0) {
      emu->running = win_update(win);
			win_clear(win);
      win_render_bg(win, emu->vram, emu->tilemaps);
      win_render_tiles(win, emu->vram);
			win_render(win);
    }

    handle_instruction(emu, inst);
  }

  win_destroy(win);

  printf("--------------END PROGRAM---------------\n\n");

  printf("-------------EmulationState-------------\n");
  emu_print(emu);

  emu_free(emu);

  return 0;
}

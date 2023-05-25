#include "./instruction.h"
#include "./util.h"
#include "types.h"
#include "ui.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define START_WITH_STEP true

// https://gbdev.io/pandocs/Interrupts.html
void handle_interrupt(EmulationState *emu) {
  // printf("%02X\n", emu->mem[IF]);
  u8 interrupt = *emu->ie & emu->mem[IF];
  if (emu->ime && interrupt) {
    if (interrupt & IF_VBLANK) {
      nop(emu);
      call(emu, 0x0040);
      emu->ime = false;
    } else if (interrupt & IF_LCD_STAT) {
      nop(emu);
      call(emu, 0x0048);
      emu->ime = false;
    } else if (interrupt & IF_TIMER) {
      nop(emu);
      call(emu, 0x0050);
      emu->ime = false;
    } else if (interrupt & IF_SERIAL) {
      nop(emu);
      call(emu, 0x0058);
      emu->ime = false;
    } else if (interrupt & IF_JOYPAD) {
      nop(emu);
      call(emu, 0x0060);
      emu->ime = false;
    }
  }
}

bool handle_instruction(EmulationState *emu, u8 inst) {
  *emu->pc += 1;
  Instruction instruction = GB_INSTRUCTIONS[inst];

  // if (emu->mem[LY] == 144) {
  //   emu->mem[IF] |= IF_VBLANK;
  // }

  // TODO: Check if instruction exists
  // if (*(u8 *)&instruction == 0) { // Uninitialized memory
  //   printf("Instruction not found: %02X\n", inst);
  //   printf("Terminating...\n");

  //   return false;
  // }

  instruction.execute(emu);
  // TODO: Fix this with real mcycle
  emu->mcycle += 1;
  emu->mem[LY] = ((emu->mem[LY] + rand() % 5) % 154);
  return true;
}

int main(int argc, char **argv) {
  const char *path = "main.gb";
  if (argc > 1) {
    path = argv[1];
  }
  printf("Loading ROM from %s\n", path);

  EmulationState *emu = emu_init();

  size_t bytes_read = read_rom(path, emu->rom);
  printf("Read %zu bytes\n", bytes_read);

  printf("-------------START HEADERS--------------\n");
  printf("Title: %s\n", emu->header->title);
  printf("Rom Type: %s\n", ROM_TYPES[(u8)emu->header->type]);
  printf("Rom Size: %dKB\n", 32 * (1 << emu->header->rom_size));
  const char *license;
  if (emu->header->lic_code == 0x33) {
    license = NEW_LICENSEE_CODE[emu->header->new_lic_code];
  } else {
    license = OLD_LICENSEE_CODE[emu->header->lic_code];
  }
  printf("License: %s\n", license);
  printf("Destination: %s\n", DESTINATION_CODE[emu->header->dest_code]);
  printf("Version: %d\n", emu->header->version);
  printf("SGB support: %s\n", emu->header->sgb_flag == 0x03 ? "true" : "false");

  // https://gbdev.io/pandocs/The_Cartridge_Header.html#014d--header-checksum
  u8 checksum = 0;
  for (u16 address = 0x0134; address <= 0x014C; address++)
    checksum = checksum - emu->rom[address] - 1;

  printf("Header Checksum: %d\n", emu->header->checksum);
  printf("Computed Value:  %d\n", checksum);
  printf("--------------END HEADERS---------------\n\n");

  bool running = true;
  bool step    = START_WITH_STEP;

  printf("-------------START PROGRAM--------------\n");
  Window *win = win_init();

  *emu->af = 0x01B0;
  *emu->bc = 0x0013;
  *emu->de = 0x00D8;
  *emu->hl = 0x014D;
  *emu->sp = 0xFFFE;
  *emu->pc = 0x0100;

  for (int frame = 0; running; frame++) {
    if (!step) {
      handle_interrupt(emu);
      u8 inst = emu->rom[*emu->pc];
      printf("Inst: %02X\tAt: ", inst);
      PRINT_BYTES(*emu->pc);
      running = handle_instruction(emu, inst);
    }

    // TODO: Use a custom input struct to set values
    bool spacedown = false;
    bool enterdown = false;
    win_update_input(win, &running, &spacedown, &enterdown);

    // TODO: Load input
    emu->io[0] |= 0x0F;

    if (enterdown) {
      step = !step;
    }

    if (step && spacedown) {
      handle_interrupt(emu);
      u8 inst = emu->rom[*emu->pc];
      printf("Inst: %02X\n", inst);
      emu_print(emu);
      printf("\n");
      running = handle_instruction(emu, inst);
    }

    // Render every 128th frame
    if (frame % 128 == 0) {
      win_clear(win);
      win_draw_tiles(win, emu->vram);

      win_draw_bg(win, emu->vram, emu->tilemaps, emu->mem[LCDC] & BG_WINDOW_DATA_AREA);

      if (emu->mem[LCDC] & OBJ_ENABLE)
        win_draw_objs(win, emu->sprites, emu->vram);

      win_render(win);
    }
  }

  win_destroy(win);

  printf("--------------END PROGRAM---------------\n\n");

  printf("-------------EmulationState-------------\n");
  emu_print(emu);

  emu_free(emu);

  return 0;
}

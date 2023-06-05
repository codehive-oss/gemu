#include "./instruction.h"
#include "./util.h"
#include "emulation.h"
#include "types.h"
#include "ui.h"
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char **argv) {
  const char *path = "main.gb";
  if (argc > 1) {
    path = argv[1];
  }

  u16 breakpoint = 0x0100;
  if (argc > 2) {
    breakpoint = (u16)strtod(argv[2], NULL);
  }

  printf("Loading ROM from %s\n", path);

  EmulationState *emu   = emu_init();
  GameInput      *input = gameinput_init();

  size_t bytes_read = read_rom(path, emu->rom);
  printf("Read %zu bytes\n", bytes_read);

  print_header(emu);

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
    if (*emu->pc == breakpoint) {
      step = true;
    }

    // TODO: Use a custom input struct to set values
    bool spacedown = false;
    bool enterdown = false;
    win_update_input(win, input, &running, &spacedown, &enterdown);
    handle_gameinput(emu, input);

    if (enterdown) {
      step = !step;
    }

    if (!step || spacedown) {
      bool interrupted = handle_interrupt(emu);
      if (interrupted)
        continue;
      u8 inst = emu->rom[*emu->pc];
      if (!step) {
        printf("Inst: %02X\tAt: ", inst);
        PRINT_BYTES(*emu->pc);
        printf("\n");
      } else {
        printf("Inst: %02X\n", inst);
        emu_print(emu);
        printf("\n");
      }
      bool success = handle_instruction(emu, inst);

      if (!success) {
        running = false;
      }
    }

    // Render every 512th frame
    if (frame % 512 == 0) {
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

  gameinput_free(input);
  emu_free(emu);

  return 0;
}

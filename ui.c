#include "./ui.h"
#include "types.h"
#include "util.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>

// ---------
// |       |
// ---------
// |       |
// ---------
// |       |
// ---------

Window *win_init() {
  Window *win = (Window *)malloc(sizeof(Window));

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win->window, &win->renderer);

  return win;
}

bool win_update(Window *win) {
  SDL_Event event;
  if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
    return false;

  return true;
}

void win_render_tiles(Window *win, u8 *data) {
  SDL_SetRenderDrawColor(win->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(win->renderer);

  for (u8 b = 0; b < 3; b++) {
    int blockOffset = b * 8 * 8;

    for (u8 y = 0; y < 8; y++) {
      int yOffset = y * 8;
      for (u8 x = 0; x < 16; x++) {
        int xOffset = x * 8;

        for (u8 i = 0; i < 64; i++) {
          u8 idx = get_palette_idx(data, i);

          SDL_SetRenderDrawColor(win->renderer, idx * 85, idx * 85, idx * 85,
                                 255);

          SDL_RenderDrawPoint(win->renderer, xOffset + i % 8, blockOffset + yOffset + i / 8);
        }
      }
    }
  }

  SDL_RenderPresent(win->renderer);
}

void win_destroy(Window *win) {
  SDL_DestroyRenderer(win->renderer);
  SDL_DestroyWindow(win->window);
  SDL_Quit();
}

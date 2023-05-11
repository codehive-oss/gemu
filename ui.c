#include "./ui.h"
#include "types.h"
#include "util.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
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

void win_render_tile(Window *win, Tile tile, u8 x, u8 y) {
  for (u8 i = 0; i < 64; i++) {
    u8 idx = get_palette_idx(tile, i);

    SDL_SetRenderDrawColor(win->renderer, 0xFF - idx * 85, 0xFF - idx * 85,
                           0xFF - idx * 85, 0xFF);

    SDL_Rect rect;
    rect.x = (x + i % 8) * SCALE_FACTOR;
    rect.y = (y + i / 8) * SCALE_FACTOR;
    rect.w = SCALE_FACTOR;
    rect.h = SCALE_FACTOR;

    SDL_RenderFillRect(win->renderer, &rect);
  }
}

void win_render_bg(Window *win, Tile *tiles, u8 *tileMap) {
  for (u8 x = 0; x < 32; x++) {
    for (u8 y = 0; y < 32; y++) {
    }
  }
}

void win_render_tiles(Window *win, Tile *data) {
  SDL_SetRenderDrawColor(win->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(win->renderer);

  for (u8 y = 0; y < 24; y++) {
    int yOffset = y * 8;
    for (u8 x = 0; x < 16; x++) {
      int xOffset = x * 8;

      int dataOffset = (x + (y * 16)) * 16;

      win_render_tile(win, data[dataOffset], xOffset, yOffset);
    }
  }

  SDL_RenderPresent(win->renderer);
}

void win_destroy(Window *win) {
  SDL_DestroyRenderer(win->renderer);
  SDL_DestroyWindow(win->window);
  SDL_Quit();
}

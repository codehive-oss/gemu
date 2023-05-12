#include "./ui.h"
#include "types.h"
#include "util.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
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
  win->window =
      SDL_CreateWindow("GEMU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED);

  SDL_SetRenderDrawColor(win->renderer, 0x00, 0x00, 0x00, 0x00);

  win->screen = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_RGBA8888,
                                  SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

  win->tileview = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_STREAMING, TILEVIEW_WIDTH,
                                    TILEVIEW_HEIGHT);
  return win;
}

bool win_update(Window *win) {
  SDL_Event event;
  if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
    return false;

  return true;
}

void win_clear(Window *win) { SDL_RenderClear(win->renderer); }
void win_render(Window *win) { SDL_RenderPresent(win->renderer); }

// TODO: Preload all textures
void win_draw_tile(u32 *target, u8 *tile, int pitch, u8 x, u8 y) {
  for (u8 i = 0; i < 64; i++) {
    u8 idx = get_palette_idx(tile, i);

    u32 color = (0xFF - idx * 85) << (8 * 3) | (0xFF - idx * 85) << (8 * 2) |
                (0xFF - idx * 85) << (8 * 1) | (0x000000FF);

    u8 ix = i % 8;
    u8 iy = i / 8;

    target[y * pitch + x + (iy * pitch) + ix] = color;
  }
}

void win_draw_bg(Window *win, u8 *tiles, u8 *tileMap) {
  u32 *winpixel;
  int pitch;

  SDL_LockTexture(win->screen, NULL, (void *)&winpixel, &pitch);

  for (u8 y = 0; y < 32; y++) {
    for (u8 x = 0; x < 32; x++) {
      int offset = (y * 32) + x;
      u8 tileIndex = tileMap[offset];
      win_draw_tile(winpixel, tiles + 0x1000 + (tileIndex * 16), pitch / 4, x * 8,
                      y * 8);
    }
  }
  SDL_UnlockTexture(win->screen);

  SDL_Rect scrRect = {
      .x = 0,
      .y = 0,
      .w = WIDTH * SCALE_FACTOR,
      .h = HEIGHT * SCALE_FACTOR,
  };

  SDL_RenderCopy(win->renderer, win->screen, NULL, &scrRect);
}

void win_draw_tiles(Window *win, u8 *data) {
  u32 *winpixel;
  int pitch;

  SDL_LockTexture(win->tileview, NULL, (void *)&winpixel, &pitch);

  for (u8 y = 0; y < 24; y++) {
    for (u8 x = 0; x < 16; x++) {
      int dataOffset = (x + (y * 16)) * 16;
      win_draw_tile(winpixel, data + dataOffset, pitch / 4, x * 8, y * 8);
    }
  }

  SDL_UnlockTexture(win->tileview);

  SDL_Rect tileviewRect = {
      .x = WIDTH * SCALE_FACTOR,
      .y = 0,
      .w = TILEVIEW_WIDTH * SCALE_FACTOR,
      .h = TILEVIEW_HEIGHT * SCALE_FACTOR,
  };

  SDL_RenderCopy(win->renderer, win->tileview, NULL, &tileviewRect);
}

void win_destroy(Window *win) {
  SDL_DestroyRenderer(win->renderer);
  SDL_DestroyTexture(win->screen);
  SDL_DestroyTexture(win->tileview);
  SDL_DestroyWindow(win->window);
  SDL_Quit();
}

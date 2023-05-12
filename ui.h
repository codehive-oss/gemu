#ifndef UI_H
#define UI_H

#include "types.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#define SCALE_FACTOR 4

// #define WIDTH 16 * 8 * SCALE_FACTOR
// #define HEIGHT 8 * 8 * 3 * SCALE_FACTOR

#define WIDTH 32 * 8
#define HEIGHT 32 * 8

#define WINDOW_WIDTH WIDTH * SCALE_FACTOR
#define WINDOW_HEIGHT HEIGHT * SCALE_FACTOR

typedef struct Window {
  SDL_Window *window;
  SDL_Renderer *renderer;
	SDL_Texture *texture;
} Window;

Window *win_init();
bool win_update(Window *win);
void win_render_bg(Window *win, u8 *tiles, u8 *tileMap);
void win_render_tiles(Window *win, u8 *data);
void win_destroy(Window *win);

#endif // UI_H

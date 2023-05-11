#ifndef UI_H
#define UI_H

#include "types.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#define SCALE_FACTOR 4

#define WIDTH 16 * 8 * SCALE_FACTOR
#define HEIGHT 8 * 8 * 3 * SCALE_FACTOR

typedef struct Window {
  SDL_Window *window;
  SDL_Renderer *renderer;
} Window;

Window *win_init();
bool win_update(Window *win);
void win_render_tiles(Window *win, u8 *data);
void win_destroy(Window *win);

#endif // UI_H

#ifndef UI_H
#define UI_H

#include "types.h"
#include "util.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#define SCALE_FACTOR 2

#define WIDTH  (32 * 8)
#define HEIGHT (32 * 8)

#define TILEVIEW_WIDTH  (16 * 8)
#define TILEVIEW_HEIGHT (24 * 8)

#define WINDOW_WIDTH  (WIDTH + TILEVIEW_WIDTH) * SCALE_FACTOR
#define WINDOW_HEIGHT HEIGHT *SCALE_FACTOR

typedef struct Window {
  SDL_Window   *window;
  SDL_Renderer *renderer;
  SDL_Texture  *screen;
  SDL_Texture  *tileview;
} Window;

Window *win_init();
void    win_update_input(Window *win, GameInput *input, bool *running, bool *spacedown, bool *enterdown);

void win_clear(Window *win);
void win_render(Window *win);
void win_draw_tile(u32 *target, u8 *tile, int pitch, u8 x, u8 y);
void win_draw_objs(Window *win, SpriteAttribute *sprites, u8 *vram);
void win_draw_bg(Window *win, u8 *vram, u8 *tileMap, bool data_area);
void win_draw_tiles(Window *win, u8 *data);
void win_destroy(Window *win);

#endif // UI_H

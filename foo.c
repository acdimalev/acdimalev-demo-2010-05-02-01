#include "SDL.h"
#include <cairo.h>

#include <math.h>

int fps = 15;

int main(int argc, char **argv) {
  SDL_Surface *sdl_surface;
  cairo_surface_t *surface;
  cairo_t *cr;

  SDL_Event event;
  Uint8 *keystate;
  Uint32 next_frame, now;

  int running;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
  SDL_SetVideoMode(320, 240, 32, 0);
  sdl_surface = SDL_GetVideoSurface();

  surface = cairo_image_surface_create_for_data(
    sdl_surface->pixels,
    CAIRO_FORMAT_RGB24,
    sdl_surface->w,
    sdl_surface->h,
    sdl_surface->pitch
    );
  cr = cairo_create(surface);
  cairo_surface_destroy(surface);

  next_frame = 1024.0L / fps;
  running = 1;

  SDL_LockSurface(sdl_surface);
  while (running) {

    /* Render Frame */
    cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cr);
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

    /* Update Display */
    SDL_UnlockSurface(sdl_surface);
    SDL_Flip(sdl_surface);
    SDL_LockSurface(sdl_surface);

    /* Delay */
    now = SDL_GetTicks();
    if (now < next_frame) {
      SDL_Delay( next_frame - SDL_GetTicks() );
    }
    next_frame = next_frame + 1024.0L / fps;

    /* Game Logic */
    SDL_PumpEvents();
    keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_q]) {
      running = 0;
    }

  }
  SDL_UnlockSurface(sdl_surface);

  cairo_destroy(cr);

  SDL_Quit();

  return 0;
}

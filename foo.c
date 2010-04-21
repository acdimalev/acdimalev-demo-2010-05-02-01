#include "SDL.h"
#include <cairo.h>

#include <math.h>

int fps = 30;
int w = 320;
int h = 240;
float scale = 16.0;

void mat_id(float *mat) {
  mat[0] = 1; mat[1] = 0; mat[2] = 0;
  mat[3] = 0; mat[4] = 1; mat[5] = 0;
  mat[6] = 0; mat[7] = 0; mat[8] = 1;
}

void vmmul(float *vert, float *mat) {
  float x, y, z;
  x = vert[0] * mat[0] + vert[1] * mat[1] + vert[2] * mat[2];
  y = vert[0] * mat[3] + vert[1] * mat[4] + vert[2] * mat[5];
  z = vert[0] * mat[6] + vert[1] * mat[7] + vert[2] * mat[8];
  vert[0] = x; vert[1] = y; vert[2] = z;
}

int main(int argc, char **argv) {
  SDL_Surface *sdl_surface;
  cairo_surface_t *surface;
  cairo_t *cr;

  SDL_Event event;
  Uint8 *keystate;
  Uint32 next_frame, now;

  float theta, t, taccel, tvel, tdrag;
  float accel, drag;

  float pos[3], vel[3];

  float vert[3], mat[3 * 3];

  int running;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
  SDL_SetVideoMode(w, h, 32, 0);
  sdl_surface = SDL_GetVideoSurface();

  /* Initialize Canvas */
  surface = cairo_image_surface_create_for_data(
    sdl_surface->pixels,
    CAIRO_FORMAT_RGB24,
    sdl_surface->w,
    sdl_surface->h,
    sdl_surface->pitch
    );
  cr = cairo_create(surface);
  cairo_surface_destroy(surface);

  cairo_scale(cr, 1, -1);
  cairo_translate(cr, w/2.0, -h/2.0);

  cairo_scale(cr, h/scale, h/scale);

  /* Initialize Delay */
  next_frame = 1024.0 / fps;

  /* Game Logic */
  running = 1;
  t = M_PI_4;
  pos[0] = 1; pos[1] = 1; pos[2] = 0;
  vel[0] = 0; vel[1] = 0; vel[2] = 0;
  taccel = 4;
  tdrag = 63/64.0;
  accel = 1 << 7;
  drag = 63/64.0;

  SDL_LockSurface(sdl_surface);
  while (running) {

    /* Render Frame */
    cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cr);
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

    mat_id(mat);
    mat[0] = cos(t); mat[1] = -sin(t); mat[2] = pos[0];
    mat[3] = sin(t); mat[4] =  cos(t); mat[5] = pos[1];

    theta = 2*M_PI * (0/3.0);
    vert[0] = sin(theta); vert[1] = cos(theta); vert[2] = 1; vmmul(vert, mat);
    cairo_move_to(cr, vert[0], vert[1]);
    theta = 2*M_PI * (1/3.0);
    vert[0] = sin(theta); vert[1] = cos(theta); vert[2] = 1; vmmul(vert, mat);
    cairo_line_to(cr, vert[0], vert[1]);
    theta = 2*M_PI * (2/3.0);
    vert[0] = sin(theta); vert[1] = cos(theta); vert[2] = 1; vmmul(vert, mat);
    cairo_line_to(cr, vert[0], vert[1]);
    cairo_close_path(cr);

    theta = 2*M_PI * (1/3.0);
    vert[0] =  0.1; vert[1] = cos(theta) / 2.0; vert[2] = 1; vmmul(vert, mat);
    cairo_move_to(cr, vert[0], vert[1]);
    theta = 2*M_PI * (1/3.0);
    vert[0] = -0.1; vert[1] = cos(theta) / 2.0; vert[2] = 1; vmmul(vert, mat);
    cairo_line_to(cr, vert[0], vert[1]);
    theta = 2*M_PI * (1/3.0);
    vert[0] = -0.1; vert[1] = cos(theta); vert[2] = 1; vmmul(vert, mat);
    cairo_line_to(cr, vert[0], vert[1]);
    theta = 2*M_PI * (1/3.0);
    vert[0] =  0.1; vert[1] = cos(theta); vert[2] = 1; vmmul(vert, mat);
    cairo_line_to(cr, vert[0], vert[1]);
    cairo_close_path(cr);

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_fill(cr);

    /* Update Display */
    SDL_UnlockSurface(sdl_surface);
    SDL_Flip(sdl_surface);
    SDL_LockSurface(sdl_surface);

    /* Delay */
    now = SDL_GetTicks();
    if (now < next_frame) {
      SDL_Delay( next_frame - SDL_GetTicks() );
    }
    next_frame = next_frame + 1024.0 / fps;

    /* Game Logic */
    SDL_PumpEvents();
    keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_q]) {
      running = 0;
    }
    if (keystate[SDLK_LEFT]) {
      tvel = tvel + taccel*2*M_PI / fps;
    }
    if (keystate[SDLK_RIGHT]) {
      tvel = tvel - taccel*2*M_PI / fps;
    }
    if (keystate[SDLK_UP]) {
      vel[0] = accel * -sin(t) / fps;
      vel[1] = accel * cos(t) / fps;
    }

    t = t + tvel / fps;
    pos[0] = pos[0] + vel[0] / fps;
    pos[1] = pos[1] + vel[1] / fps;
    tvel = tvel * pow(1.0 - tdrag, 1.0/fps);
    vel[0] = vel[0] * pow(1.0 - drag, 1.0/fps);
    vel[1] = vel[1] * pow(1.0 - drag, 1.0/fps);

  }
  SDL_UnlockSurface(sdl_surface);

  cairo_destroy(cr);

  SDL_Quit();

  return 0;
}

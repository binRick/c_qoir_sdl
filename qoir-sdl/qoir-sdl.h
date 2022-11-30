#pragma once
#ifndef QOIR_SDL_H
#define QOIR_SDL_H
//////////////////////////////////////
#include "submodules/qoir/src/qoir.h"
#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <unistd.h>
//////////////////////////////////////
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
//////////////////////////////////////

module(qoir_sdl) {
  define(qoir_sdl, CLIB_MODULE);
  struct {
    int (*loop)(void);
    struct {
      bool
      (*path)(const char *path),
      (*buf)(const unsigned char *buf, size_t len);
    } load;
  } ops;
  struct {
    qoir_decode_result *decoded;
    struct {
      qoir_decode_result
      *(*buf)(unsigned char *buf, size_t len),
      *(*path)(const char *path);
      void *(*pixels)(SDL_RWops *src, qoir_decode_options *opts, Uint32 *format, int freesrc);
    } decode;
    struct {
      struct qoir_decode_pixel_configuration_result_struct
        (*path)(const char *path),
      (*buf)(unsigned char *buf, size_t len);
    } config;
    struct {
      qoir_rectangle (*rect)(unsigned char *buf, size_t len);
    } get;
    struct {
      qoir_rectangle (*make)(int x0, int y0, int x1, int y1);
      uint32_t       (*width)(qoir_rectangle rect);
      uint32_t       (*height)(qoir_rectangle rect);
      bool           (*empty)(qoir_rectangle rect);
      qoir_rectangle (*intersect)(qoir_rectangle r1, qoir_rectangle r2);
    } rect;
    struct {
      bool (*empty)(qoir_pixel_buffer buf);
    } is;
  } qoir;
  struct {
    struct {
      SDL_Window *data;
    }                    window;
    struct {
      SDL_Surface *data;
    }                    surface;
    struct {
      SDL_Texture *data;
    }                    texture;
    struct {
      SDL_Renderer *data;
    }                    renderer;
    struct { int w, h; } size;
    struct { int x, y; } pos;
    char                 *title;
    bool                 resizable, top;
  } sdl;
};

int qoir_sdl_init(module(qoir_sdl) * exports);
bool  __qoir_sdl_load_path(const char *path);
bool  __qoir_sdl_load_buf(const unsigned char *buf, size_t len);
void qoir_sdl_deinit(module(qoir_sdl) * exports);
int __qoir_sdl_loop(void);
qoir_decode_result *__qoir_sdl_decode_buf(unsigned char *buf, size_t len);
qoir_decode_result *__qoir_sdl_decode_path(const char *path);
void *__qoir_sdl_decode_pixels(SDL_RWops *src, qoir_decode_options *opts, Uint32 *format, int freesrc);
SDL_Surface * __qoir_sdl_get_surface_rw(SDL_RWops *src, int freesrc);
SDL_Texture * SDL_LoadQOIR_Texture(SDL_Renderer *ren, char *path);
SDL_Texture * SDL_LoadQOIR_Texture_RW(SDL_Renderer *ren, SDL_RWops *src, int freesrc);
int SDL_SaveQOIR_RW(SDL_Surface *sur, SDL_RWops *dst, int freedst);

#define SDL_SaveQOIR(sur, dst) \
  SDL_SaveQOIR_RW(sur, SDL_RWFromFile(dst, "wb"), 1)

exports(qoir_sdl) {
  .init      = qoir_sdl_init,
  .deinit    = qoir_sdl_deinit,
  .qoir      = {
    .is      = {
      .empty = qoir_pixel_buffer__is_zero,
    },
    .rect        = {
      .make      = qoir_make_rectangle,
      .empty     = qoir_rectangle__is_empty,
      .intersect = qoir_rectangle__intersect,
      .width     = qoir_rectangle__width,
      .height    = qoir_rectangle__height,
    },
    .decode   = {
      .buf    = __qoir_sdl_decode_buf,
      .path   = __qoir_sdl_decode_path,
      .pixels = __qoir_sdl_decode_pixels,
    },
  },
  .sdl       = {
    .surface = {
      .data  = 0,
    },
    .texture = {
      .data  = 0,
    },
    .window = {
      .data = 0,
    },
    .renderer = {
      .data   = 0,
    },
    .pos = {
      .x = 100,
      .y = 50,
    },
    .size = {
      .w  = 800,
      .h  = 400,
    },
  },
  .ops      = {
    .loop   = __qoir_sdl_loop,
    .load   = {
      .buf  = __qoir_sdl_load_buf,
      .path = __qoir_sdl_load_path,
    },
  },
};
#endif

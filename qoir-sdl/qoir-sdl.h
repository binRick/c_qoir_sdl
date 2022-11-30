#pragma once
#ifndef QOIR_SDL_H
#define QOIR_SDL_H
//////////////////////////////////////
#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <unistd.h>
//////////////////////////////////////
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
//////////////////////////////////////
module(qoir_sdl) {
  define(qoir_sdl, CLIB_MODULE);
  char *n;
};
int  qoir_sdl_init(module(qoir_sdl) * exports);
void qoir_sdl_deinit(module(qoir_sdl) * exports);

SDL_Surface * SDL_LoadQOIR_RW(SDL_RWops *src, int freesrc);
SDL_Texture * SDL_LoadQOIR_Texture_RW(SDL_Renderer *ren, SDL_RWops *src, int freesrc);
int SDL_SaveQOIR_RW(SDL_Surface *sur, SDL_RWops *dst, int freedst);
#define SDL_LoadQOIR(file) \
  SDL_LoadQOIR_RW(SDL_RWFromFile(file, "rb"), 1)
#define SDL_LoadQOIR_Texture(ren, file) \
  SDL_LoadQOIR_Texture_RW(ren, SDL_RWFromFile(file, "rb"), 1)
#define SDL_SaveQOIR(sur, dst) \
  SDL_SaveQOIR_RW(sur, SDL_RWFromFile(dst, "wb"), 1)

exports(qoir_sdl) {
  .init   = qoir_sdl_init,
  .deinit = qoir_sdl_deinit,
  .n      = "xxx",
};
#endif

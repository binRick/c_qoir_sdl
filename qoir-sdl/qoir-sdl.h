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
#include <unistd.h>
//////////////////////////////////////
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
//////////////////////////////////////
module(qoir_sdl) {
  define(qoir_sdl, CLIB_MODULE);
};
int  qoir_sdl_init(module(qoir_sdl) * exports);
void qoir_sdl_deinit(module(qoir_sdl) * exports);
exports(qoir_sdl) {
  .init   = qoir_sdl_init,
  .deinit = qoir_sdl_deinit,
};
#endif

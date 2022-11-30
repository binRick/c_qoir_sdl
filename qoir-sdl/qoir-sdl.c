#ifndef QOIR_SDL_C
#define QOIR_SDL_C
////////////////////////////////////////////
#include "qoir-sdl/qoir-sdl.h"
////////////////////////////////////////////
#include "fsio.h"
#include "stringbuffer.h"
#include "stringfn.h"
#include "submodules/qoir/src/qoir.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

////////////////////////////////////////////
int qoir_sdl_init(module(qoir_sdl) *exports) {
  clib_module_init(qoir_sdl, exports);
  return(EXIT_SUCCESS);
}

void qoir_sdl_deinit(module(qoir_sdl) *exports) {
  clib_module_deinit(qoir_sdl);
}
////////////////////////////////////////////
#endif

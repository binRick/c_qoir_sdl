#include "c_greatest/greatest/greatest.h"
#include "fsio.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
#include "qoir-sdl-test/qoir-sdl-test.h"
#include "qoir-sdl/qoir-sdl.h"
#include "stringbuffer.h"
#include "stringfn.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#define WIN_WIDTH     640
#define WIN_HEIGHT    480

int __qoir_sdl_test(){
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialise SDL2: %s", SDL_GetError());
    return(1);
  }

  SDL_Window *window = SDL_CreateWindow("SDL_QOI example",
                                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT,
                                        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  if (window == NULL) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    return(1);
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    SDL_Log("Unable to create renderer: %s", SDL_GetError());
    return(1);
  }
  SDL_RenderSetLogicalSize(renderer, WIN_WIDTH, WIN_HEIGHT);

  SDL_Texture *tex = SDL_LoadQOIR_Texture(renderer, "./SDL_logo.qoir");
  if (tex == NULL) {
    SDL_Log("Unable to load file: %s", SDL_GetError());
    return(1);
  }

  int       shouldQuit = 0;
  SDL_Event event;
  while (!shouldQuit) {
    while (SDL_PollEvent(&event))
      switch (event.type) {
      case SDL_QUIT: shouldQuit = 1; break;
      }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return(0);
} /* __qoir_sdl_test */

TEST t_qoir_sdl_test2(){
  printf("n:%s\n", require(qoir_sdl)->n);
  PASS();
}

TEST t_qoir_sdl_test1(){
  PASS();
}

SUITE(s_qoir_sdl_test) {
  RUN_TEST(t_qoir_sdl_test1);
  if (isatty(STDOUT_FILENO))
    RUN_TEST(t_qoir_sdl_test2);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(s_qoir_sdl_test);
  GREATEST_MAIN_END();
}

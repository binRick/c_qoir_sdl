#include "c_greatest/greatest/greatest.h"
#include "fsio.h"
#include "incbin/incbin.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
#include "qoir-sdl-test/qoir-sdl-test.h"
#include "qoir-sdl/qoir-sdl.h"
#include "stringbuffer.h"
#include "stringfn.h"
INCBIN(sdl_logo_png, "assets/SDL_logo.png");
INCBIN(sdl_logo_qoir, "assets/SDL_logo.qoir");
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
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

static module(qoir_sdl) * qs;

TEST t_qoir_sdl_test_load_buf(){
  char               *s;
  qoir_decode_result *res;

  res = qs->qoir.decode.buf((unsigned char *)gsdl_logo_qoirData, gsdl_logo_qoirSize);
  ASSERT_NEQm("invalid decoding", res->dst_pixbuf.data, 0);
  printf("%dx%d\n",
         res->dst_pixbuf.pixcfg.width_in_pixels,
         res->dst_pixbuf.pixcfg.height_in_pixels
         );
  asprintf(&s, "Decoded %dx%d %d byte buffer",
           res->dst_pixbuf.pixcfg.width_in_pixels,
           res->dst_pixbuf.pixcfg.height_in_pixels,
           gsdl_logo_qoirSize
           );
  PASSm(s);
}

TEST t_qoir_sdl_test_get_pixels(){
  PASS();
}

TEST t_qoir_sdl_test_load_path(){
  char               *s;
  qoir_decode_result *res;

  qs = require(qoir_sdl);
  char *tf;
  bool ok;

  tf = "/tmp/test-loop.qoir";
  ok = fsio_write_binary_file(tf, (char *)gsdl_logo_qoirData, gsdl_logo_qoirSize);
  ASSERT_EQm("write qoir file failed", ok, true);
  res = qs->qoir.decode.path(tf);
  ASSERT_NEQm("invalid decoding", res->dst_pixbuf.data, 0);
  printf("%dx%d\n",
         res->dst_pixbuf.pixcfg.width_in_pixels,
         res->dst_pixbuf.pixcfg.height_in_pixels
         );
  asprintf(&s, "Decoded %dx%d %lu byte file",
           res->dst_pixbuf.pixcfg.width_in_pixels,
           res->dst_pixbuf.pixcfg.height_in_pixels,
           fsio_file_size(tf)
           );
  fsio_remove(tf);
  PASSm(s);
}

TEST t_qoir_sdl_test_loop(){
  qoir_decode_result *res;

  qs = require(qoir_sdl);
  char *tf;
  bool ok;

  tf = "/tmp/test-loop.qoir";
  ok = fsio_write_binary_file(tf, (char *)gsdl_logo_qoirData, gsdl_logo_qoirSize);
  ASSERT_EQm("write qoir file failed", ok, true);
  res = qs->qoir.decode.path(tf);
  ASSERT_NEQm("invalid decoding", res->dst_pixbuf.data, 0);
  printf("%dx%d\n",
         res->dst_pixbuf.pixcfg.width_in_pixels,
         res->dst_pixbuf.pixcfg.height_in_pixels
         );
  res = qs->qoir.decode.buf((unsigned char *)fsio_read_binary_file(tf), fsio_file_size(tf));
  ASSERT_NEQm("invalid decoding", res->dst_pixbuf.data, 0);
  printf("%dx%d\n",
         res->dst_pixbuf.pixcfg.width_in_pixels,
         res->dst_pixbuf.pixcfg.height_in_pixels
         );

//  ok = require(qoir_sdl)->ops.load.file(tf);
//  ASSERT_EQm("load file failed",ok,true);
  //require(qoir_sdl)->ops.loop();
  //ASSERT_EQm("loop failed",res,0);
  PASS();
}

SUITE(s_qoir_sdl_test) {
  RUN_TEST(t_qoir_sdl_test_load_path);
  RUN_TEST(t_qoir_sdl_test_load_buf);
  RUN_TEST(t_qoir_sdl_test_get_pixels);
  RUN_TEST(t_qoir_sdl_test_loop);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(s_qoir_sdl_test);
  GREATEST_MAIN_END();
}

#ifndef QOIR_SDL_C
#define QOIR_SDL_C
////////////////////////////////////////////
#include "qoir-sdl/qoir-sdl.h"
////////////////////////////////////////////
#include "fsio.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
#include "stringbuffer.h"
#include "stringfn.h"
#include "submodules/qoir/src/qoir.h"
#define QOIR_MALLOC(sz)    SDL_malloc(sz)
#define QOIR_FREE(p)       SDL_free(p)
#define QOIR_ZEROARR(a)    SDL_memset((a), 0, sizeof(a))
void * __qoir_sdl_decode_pixels(SDL_RWops *src, qoir_decode_options *opts, Uint32 *format, int freesrc);

int SDL_SaveQOIR_RW(SDL_Surface *sur, SDL_RWops *dst, int freedst) {
  SDL_bool hasAlpha = sur->format->Amask != 0;
  Uint32   format   = hasAlpha ? SDL_PIXELFORMAT_RGBA32 : SDL_PIXELFORMAT_RGB24;
//  int channels = qoir_pixel_format__bytes_per_pixel(res.dst_pixbuf.pixcfg.pixfmt);

  SDL_Surface *convertedSur = SDL_ConvertSurfaceFormat(sur, format, 0);

  if (convertedSur == NULL)
    return(1);

  return(0);
/*
   qoi_desc desc = {.width = convertedSur->w,
      .height = convertedSur->h,
      .channels = channels,
      .colorspace = QOI_SRGB};

   int size = 0;
   void* pixels = qoi_encode(convertedSur->pixels, &desc, &size);
   if (pixels == NULL) {
    SDL_SetError("Unable to encode QOI data");
    SDL_FreeSurface(convertedSur);
    if (freedst) {
      SDL_RWclose(dst);
    }
    return 1;
   }
   int res = SDL_RWwrite(dst, pixels, 1, size);

   SDL_free(pixels);
   SDL_FreeSurface(convertedSur);
   if (freedst) {
    SDL_RWclose(dst);
   }

   return res != size;
 */
}

qoir_decode_result *__qoir_sdl_decode_path(const char *path){
  unsigned char *buf = (unsigned char *)fsio_read_binary_file((char *)path);
  size_t        len  = fsio_file_size((char *)path);

  return(__qoir_sdl_decode_buf(buf, len));
}

qoir_decode_result *__qoir_sdl_decode_buf(unsigned char *buf, size_t len){
  qoir_decode_options opts = { 0 };
  qoir_decode_result  res = { 0 }, *r = calloc(1, sizeof(qoir_decode_result));

  res = qoir_decode(buf, len, &opts);
  memcpy(r, &res, sizeof(qoir_decode_result));
  return(r);
}

SDL_Texture *SDL_LoadQOIR_Texture(SDL_Renderer *ren, char *path){
  return(SDL_LoadQOIR_Texture_RW(ren, SDL_RWFromFile(path, "rb"), 1));
}

SDL_Texture * SDL_LoadQOIR_Texture_RW(SDL_Renderer *ren, SDL_RWops *src, int freesrc) {
  qoir_decode_options opts = { 0 };
  Uint32              format;
  void                *pixels = __qoir_sdl_decode_pixels(src, &opts, &format, freesrc);

  if (pixels == NULL)
    return(NULL);

  SDL_Texture *tex = NULL;

//  tex = SDL_CreateTexture(ren, format, SDL_TEXTUREACCESS_STATIC, desc.width, desc.height);
  if (tex == NULL) {
    SDL_free(pixels);
    return(NULL);
  }
  if (SDL_UpdateTexture(tex, NULL, pixels, opts.pixbuf.pixcfg.width_in_pixels * qoir_pixel_format__bytes_per_pixel(opts.pixbuf.pixcfg.pixfmt)) != 0) {
    SDL_free(pixels);
    SDL_DestroyTexture(tex);
    return(NULL);
  }
  if (format == SDL_PIXELFORMAT_RGBA32)
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

  SDL_free(pixels);
  return(tex);
}

SDL_Surface * __qoir_sdl_get_surface_rw(SDL_RWops *src, int freesrc) {
  SDL_Surface         *sur = NULL;
  qoir_decode_options opts = { 0 };
  Uint32              format;
  void                *pixels = __qoir_sdl_decode_pixels(src, &opts, &format, freesrc);

  if (pixels == NULL)
    return(NULL);

  //= SDL_CreateRGBSurfaceWithFormatFrom(pixels, desc.width, desc.height, 8 * desc.channels, desc.width * desc.channels, format);
  return(sur);

//  return sur;
}

void * __qoir_sdl_decode_pixels(SDL_RWops *src, qoir_decode_options *opts, Uint32 *format, int freesrc) {
  if (src == NULL)
    return(NULL);

  size_t size;
  void   *data = SDL_LoadFile_RW(src, &size, freesrc);
  if (data == NULL)
    return(NULL);

  qoir_decode_result *res = require(qoir_sdl)->qoir.decode.buf(data, size);
  if (res->dst_pixbuf.data == NULL) {
    SDL_SetError("Unable to decode QOIR data");
    SDL_free(data);
    return(NULL);
  }
  *format = qoir_pixel_format__bytes_per_pixel(res->dst_pixbuf.pixcfg.pixfmt) == 3 ? SDL_PIXELFORMAT_RGB24 : SDL_PIXELFORMAT_RGBA32;
  SDL_free(data);
  return((void *)res->dst_pixbuf.data);
}

////////////////////////////////////////////
int __qoir_sdl_loop(){
  module(qoir_sdl) * exports = require(qoir_sdl);
  int       shouldQuit = 0;
  SDL_Event event;
  while (!shouldQuit) {
    while (SDL_PollEvent(&event))
      switch (event.type) {
      case SDL_QUIT: shouldQuit = 1; break;
      }
    SDL_RenderClear(exports->sdl.renderer.data);
    SDL_RenderCopy(exports->sdl.renderer.data, exports->sdl.texture.data, NULL, NULL);
    SDL_RenderPresent(exports->sdl.renderer.data);
  }

  SDL_DestroyTexture(exports->sdl.texture.data);
  SDL_DestroyRenderer(exports->sdl.renderer.data);
  SDL_DestroyWindow(exports->sdl.window.data);
  SDL_Quit();
  return(0);
}

int qoir_sdl_init(module(qoir_sdl) *exports) {
  clib_module_init(qoir_sdl, exports);
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialise SDL2: %s", SDL_GetError());
    return(1);
  }
  if ((!(exports->sdl.window.data = SDL_CreateWindow("SDL_QOI example", exports->sdl.pos.x, exports->sdl.pos.y, exports->sdl.size.w, exports->sdl.size.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)))) {
    SDL_Log("Unable to initialise window: %s", SDL_GetError());
    return(1);
  }
  if (!(exports->sdl.renderer.data = SDL_CreateRenderer(exports->sdl.window.data, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) {
    SDL_Log("Unable to initialise renderer: %s", SDL_GetError());
    return(1);
  }
  if (SDL_RenderSetLogicalSize(exports->sdl.renderer.data, exports->sdl.size.w, exports->sdl.size.h)) {
    SDL_Log("Unable to set logical size: %s", SDL_GetError());
    return(1);
  }
  /*
     if(!(exports->sdl.texture = SDL_LoadQOIR_Texture(exports->sdl.renderer, FILE_PATH))){
     }
   */
  return(EXIT_SUCCESS);
}

bool  __qoir_sdl_load_path(const char *path){
  printf("loading file %s\n", path);
  require(qoir_sdl)->qoir.decoded = require(qoir_sdl)->qoir.decode.path(path);
  return((require(qoir_sdl)->qoir.decoded->dst_pixbuf.data) != NULL);
}

bool  __qoir_sdl_load_buf(const unsigned char *buf, size_t len){
  return(true);
}

void qoir_sdl_deinit(module(qoir_sdl) *exports) {
  clib_module_deinit(qoir_sdl);
}
////////////////////////////////////////////
#endif

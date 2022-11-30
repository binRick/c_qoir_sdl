#include "c_greatest/greatest/greatest.h"
#include "fsio.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"
#include "qoir-sdl-test/qoir-sdl-test.h"
#include "qoir-sdl/qoir-sdl.h"
#include "stringbuffer.h"
#include "stringfn.h"

TEST t_qoir_sdl_test2(){
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

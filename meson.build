project('c_qoir_sdl',['c', 'cpp', 'objc', 'objcpp'],
  default_options: ['warning_level=1','c_std=gnu99','default_library=static'],
  version: '0.0.1',
  )
cc = meson.get_compiler('c')
pwd = meson.current_source_dir()
inc = [
  include_directories(
    'src',
    'include',
    'dist','dist/src','dist/include',
    'deps/clibs',
    'submodules',
    'submodules/c_greatest',
    '.',
  ),
]
sdl2_dep = dependency('SDL2',fallback:['sdl2', 'sdl2_dep'], required: true)
sdl2_ttf_dep = dependency('SDL2_ttf', required: true, fallback:['sdl2_ttf','sdl2_ttf_dep'])
sdl2_image_dep = dependency('SDL2_image', required: true, fallback:['sdl2_image','sdl2_image_dep'])
sdl2_gfx_dep = dependency('SDL2_gfx', required: false)

c_qoir_sdl_util_link_args=[
  ]
c_qoir_sdl_link_args=[
]

c_qoir_sdl_util_sources = [
  'dist/src/fsio.c',
  'dist/src/stringbuffer.c',
  'dist/src/stringfn.c',
  'submodules/c_greatest/greatest/greatest.c',
  'submodules/qoir/src/qoir.h',
]

c_qoir_sdl_deps = [
  sdl2_dep,sdl2_ttf_dep,sdl2_image_dep,sdl2_gfx_dep,
]

c_qoir_sdl_sources = [
  'qoir-sdl/qoir-sdl.h',
  'qoir-sdl/qoir-sdl.c',
]

#############################################################################
c_qoir_sdl_util_lib = static_library('c_qoir_sdl_util',
  files(c_qoir_sdl_util_sources),
  install: false,
  dependencies: [c_qoir_sdl_deps],
  c_args: [],
  link_args: c_qoir_sdl_util_link_args,
  include_directories: [inc]
)

c_qoir_sdl_lib = static_library('c_qoir_sdl',
  files(c_qoir_sdl_sources),
  install: false,
  dependencies: [],
  c_args: [],
  link_args: c_qoir_sdl_link_args,
  link_whole: [c_qoir_sdl_util_lib],
  include_directories: [inc]
)

c_qoir_sdl_dep = declare_dependency(
  include_directories: ['src'],
  link_with: c_qoir_sdl_lib,
  link_args: c_qoir_sdl_link_args,
)

subdir('qoir-sdl')
subdir('qoir-sdl-test')

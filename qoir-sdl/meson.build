qoir_sdl_srcs = [
  'qoir-sdl.c',
]

inc = get_variable('inc', join_paths(meson.current_source_dir(),'..'))

qoir_sdl_include_dirs = [
  '.',
  inc,
]

qoir_sdl_deps = [
]

qoir_sdl_c_args = [
]

qoir_sdl_link_args = [
]

qoir_sdl_lib = static_library('qoir-sdl',
  qoir_sdl_srcs,
  install: false,
  dependencies: qoir_sdl_deps,
  c_args: qoir_sdl_c_args,
  include_directories: qoir_sdl_include_dirs
)

qoir_sdl_dep = declare_dependency(
  include_directories: qoir_sdl_include_dirs,
  link_with: qoir_sdl_lib,
  link_args: qoir_sdl_link_args,
)

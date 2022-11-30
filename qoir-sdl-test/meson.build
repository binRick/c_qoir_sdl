qoir_sdl_test_srcs = [
  'qoir-sdl-test.c',
]
inc = get_variable('inc', join_paths(meson.current_source_dir(),'..'))
qoir_sdl_test_include_dirs = [
  '.',
  inc,
]
qoir_sdl_test_deps = [
  qoir_sdl_dep,
]
qoir_sdl_test_c_args = [
]
qoir_sdl_test_link_args = [
]

qoir_sdl_test_test_dir = join_paths(meson.current_source_dir(), '..')

if get_option('enable-binaries')
  qoir_sdl_test_exec = executable('qoir-sdl-test',
     qoir_sdl_test_srcs,
     dependencies: qoir_sdl_test_deps,
     include_directories: qoir_sdl_test_include_dirs,
     link_args: qoir_sdl_test_link_args,
     c_args: qoir_sdl_test_c_args,
     install: false,
  )
  test('qoir-sdl-test', 
    qoir_sdl_test_exec, args: ['-v','-a'],
    workdir: qoir_sdl_test_test_dir,
  )
endif

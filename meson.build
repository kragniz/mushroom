project('mushroom', 'c',
  version : '0.1.0',
  default_options: [
    'c_std=gnu11',
    'warning_level=3',
    'werror=true',
  ],
)

if meson.get_compiler('c').get_id() == 'clang'
  extra_args = ['-Wno-newline-eof']
else
  extra_args = []
endif

add_project_arguments([
  '-Wshadow',
  '-Wno-unused-parameter',
  '-Wstrict-overflow',
  '-fno-strict-aliasing',
] + extra_args, language: 'c')

mushroom_inc = include_directories('include')

sources = files([
  'conf.c',
  'gossip_server.c',
  'jump.c',
  'log.c',
  'node.c',
  'ring.c',
])

libuv = dependency('libuv')
deps = [libuv]

libmushroom = static_library(
  'mushroom',
  sources,
  include_directories: [mushroom_inc],
  dependencies: deps,
)


mushroom = executable(
  'mushroom',
  ['mushroom.c'],
  link_with: libmushroom,
  dependencies: deps,
  include_directories: [mushroom_inc],
  install: true,
)

subdir('test')

run_target(
  'fmt',
  command: 'hack/clang-format.sh',
)

run_target(
  'check',
  command: 'hack/clang-check.sh',
)

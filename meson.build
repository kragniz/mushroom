project('mushroom', 'c',
  version : '0.1.0',
  default_options: [
    'c_std=gnu11',
    'warning_level=3',
    'werror=true',
  ],
)

add_project_arguments([
  '-Wshadow',
  '-Wno-unused-parameter',
  '-Wstrict-overflow',
  '-fno-strict-aliasing',
], language: 'c')

mushroom_inc = include_directories('include')

sources = files([
  'gossip_server.c',
  'jump.c',
  'node.c',
  'ring.c',
  'log.c',
])

libuv = dependency('libuv')

deps = [libuv]

mushroom = executable(
  'mushroom',
  sources + ['mushroom.c'],
  dependencies: deps,
  include_directories: [mushroom_inc],
  install: true,
)

subdir('test')

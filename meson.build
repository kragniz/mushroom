project('mushroom', 'c',
  version : '0.1.0',
  default_options: [
    'c_std=gnu11',
    'warning_level=3',
    'werror=true',
    'default_library=static',
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

libuv = dependency('libuv')
deps = [libuv]

# flatbuffers stuff
schema_dir = meson.current_source_dir() 
flatcc = subproject('flatcc')
flatcc_gen = flatcc.get_variable('flatcc_gen')
libflatccrt = flatcc.get_variable('libflatccrt')
flatcc_include = flatcc.get_variable('flatcc_include')
flatcc_common = flatcc.get_variable('flatcc_common')

gossip_fbs = schema_dir + '/schema/schema.fbs'
gossip_schema = flatcc_gen.process(gossip_fbs)

sources = files([
  'conf.c',
  'gossip_client.c',
  'gossip_server.c',
  'jump.c',
  'log.c',
  'node.c',
  'ring.c',
])

libmushroom = static_library(
  'mushroom',
  sources,
  gossip_schema,
  flatcc_common,
  include_directories: [mushroom_inc, flatcc_include],
  dependencies: deps,
)

mushroom = executable(
  'mushroom',
  ['mushroom.c'],
  link_with: libmushroom,
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

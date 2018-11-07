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

# flatbuffers stuff
schema_dir = meson.current_source_dir() 
flatcc = subproject('flatcc')
flatcc_gen = flatcc.get_variable('flatcc_gen')
libflatccrt = flatcc.get_variable('libflatccrt')
flatcc_include = flatcc.get_variable('flatcc_include')
flatcc_common = flatcc.get_variable('flatcc_common')

gossip_fbs = schema_dir + '/schema/gossip.fbs'
gossip_schema = flatcc_gen.process(gossip_fbs)

git_version_h = declare_dependency(
  sources: vcs_tag(
      input: 'hack/version.h.meson',
      output: 'version.h',
      fallback: meson.project_version(),
      command: ['git', 'describe', '--long', '--always', '--dirty', '--all'],
  )
)

libuv = dependency('libuv')
deps = [libuv, git_version_h]

subdir('src')

mushroom = executable(
  'mushroom',
  ['mushroom.c'],
  link_with: libmushroom,
  include_directories: [mushroom_inc],
  install: true,
  dependencies: deps,
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

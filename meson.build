project('mushroom', 'c',
  version : '0.1.0',
  default_options: [
    'c_std=gnu11',
    'warning_level=3',
    'werror=true',
    'default_library=static',
  ],
)

cc = meson.get_compiler('c')
m = cc.find_library('m', required: false)

if cc.get_id() == 'clang'
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
flatcc_gen = flatcc.get_variable('flatcc_json_gen')
libflatccrt = flatcc.get_variable('libflatccrt')
flatcc_include = flatcc.get_variable('flatcc_include')
flatcc_common = flatcc.get_variable('flatcc_common')

gossip_fbs = schema_dir + '/schema/gossip.fbs'
gossip_schema = flatcc_gen.process(gossip_fbs)

api_request_fbs = schema_dir + '/schema/api_request.fbs'
api_request_schema = flatcc_gen.process(api_request_fbs)

api_response_fbs = schema_dir + '/schema/api_response.fbs'
api_response_schema = flatcc_gen.process(api_response_fbs)

git_version_h = declare_dependency(
  sources: vcs_tag(
      input: 'hack/version.h.meson',
      output: 'version.h',
      fallback: meson.project_version(),
      command: ['git', 'describe', '--long', '--always', '--dirty', '--all'],
  )
)

libuv = dependency('libuv')
http_parser = subproject('http-parser')
libhttp_parser = http_parser.get_variable('http_parser')
libhttp_parser_include = http_parser.get_variable('include')
deps = [libuv, git_version_h, m]

flatcc_all = [
  flatcc_common,
  gossip_schema,
  api_request_schema,
  api_response_schema,
]

subdir('src')

mushroom = executable(
  'mushroom',
  ['src/mushroom.c'],
  flatcc_all,
  link_with: [libmushroom, libflatccrt, libhttp_parser],
  include_directories: [mushroom_inc, libhttp_parser_include, flatcc_include],
  install: true,
  dependencies: deps,
)

libtheft = subproject('theft')
libtheft_dep = libtheft.get_variable('libtheft_dep')
subdir('test')

run_target(
  'fmt',
  command: 'hack/clang-format.sh',
)

run_target(
  'check',
  command: 'hack/clang-check.sh',
  depends: mushroom,
)

run_target(
  'cluster',
  command: ['hack/cluster', mushroom],
  depends: mushroom,
)

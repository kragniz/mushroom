project('theft', 'c')

include = include_directories(['theft/inc', 'theft/src'])

sources = files([
  'theft/src/theft.c',
  'theft/src/theft_autoshrink.c',
  'theft/src/theft_bloom.c',
  'theft/src/theft_call.c',
  'theft/src/theft_hash.c',
  'theft/src/theft_random.c',
  'theft/src/theft_rng.c',
  'theft/src/theft_run.c',
  'theft/src/theft_shrink.c',
  'theft/src/theft_trial.c',
  'theft/src/theft_aux.c',
  'theft/src/theft_aux_builtin.c',
])

libtheft = static_library(
  'theft',
  sources,
  include_directories: include,
)

libtheft_dep = declare_dependency(
  include_directories: include,
  link_with: libtheft,
)

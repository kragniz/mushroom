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
    'jump.c',
    'node.c',
    'ring.c',
])

mushroom = executable(
	'mushroom',
    sources + ['mushroom.c'],
	dependencies: [
	],
	include_directories: [mushroom_inc],
	install: true,
)

subdir('test')
test('basic', mushroom)

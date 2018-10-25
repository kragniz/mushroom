test_units = [
  'jump_test',
  'node_test',
  'ring_test',
]

greatest = include_directories('deps/greatest')

foreach test_unit: test_units
  exe = executable(
    test_unit,
    [test_unit + '.c'],
    link_with: libmushroom,
    include_directories: [mushroom_inc] + [greatest],
    dependencies: deps,
  )

  test(
    test_unit,
    exe
  )
endforeach

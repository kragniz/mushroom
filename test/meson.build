test_units = [
  'conf_test',
  'jump_test',
  'map_property_test',
  'map_test',
  'node_test',
  'ring_test',
  'store_test',
  'workqueue_test',
]

greatest = include_directories('deps/greatest')

foreach test_unit: test_units
  exe = executable(
    test_unit,
    [test_unit + '.c'],
    link_with: libmushroom,
    include_directories: [mushroom_inc] + [greatest],
    dependencies: deps + [libtheft_dep],
  )

  test(
    test_unit,
    exe
  )
endforeach

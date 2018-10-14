test_units = [
  'jump_test',
]

foreach test_unit: test_units
  exe = executable(
    test_unit,
    sources + [test_unit + '.c'],
    include_directories: [mushroom_inc],
  )

  test(
    test_unit,
    exe
  )
endforeach

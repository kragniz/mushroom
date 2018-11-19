project('http-parser', 'c')

include = include_directories('http-parser')

http_parser = static_library(
  'http_parser',
  ['http-parser/http_parser.c'],
  include_directories: include,
)

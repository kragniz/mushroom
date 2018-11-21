sources = files([
  'api.c',
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
  flatcc_all,
  link_with: [libflatccrt, libhttp_parser],
  include_directories: [mushroom_inc, libhttp_parser_include, flatcc_include],
  dependencies: deps,
)

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
  include_directories: [mushroom_inc, flatcc_include],
  dependencies: deps,
)

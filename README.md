mushroom
========

A terrible distributed database that doesn't distribute or store data yet.

[![builds.sr.ht status](https://builds.sr.ht/~kragniz/mushroom.svg)](https://builds.sr.ht/~kragniz/mushroom?)

build
-----

Clone:

    git clone --recursive https://git.sr.ht/~kragniz/mushroom

Build:

    cd mushroom
    meson -Db_sanitize=address,undefined build
    ninja -C build

API
---

Request:

```json
{
  "action_type": "get",
  "action": {
    "key": {
      "space": "foo",
      "name": "bar"
    }
  }
}
```

Response:

```json
{
  "content_type": "str",
  "content": {
    "value": "toot toot"
  }
}
```

Example with curl:

    curl -d '{"action_type":"get","action": {"key": {"space": "foo","name": "bar"}}}' localhost:6969

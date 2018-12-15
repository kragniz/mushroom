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
    meson build
    ninja -C build

API
---

Request:

```json
{
  "action_type":"get",
  "action": {
    "key": {
      "space": "foo",
      "name": "bar"
    }
  }
}
```

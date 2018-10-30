#!/usr/bin/env sh

cd "${MESON_SOURCE_ROOT}"
clang-check -fixit -analyze *.c include/*.h test/*.c -- -Iinclude -Ibuild -Itest/deps/greatest

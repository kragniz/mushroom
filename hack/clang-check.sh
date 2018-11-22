#!/usr/bin/env sh

cd "${MESON_SOURCE_ROOT}"
clang-check -fixit -analyze src/*.c include/*.h test/*.c -- \
    -Iinclude \
    -Ibuild \
    -Itest/deps/greatest \
    -Ibuild/mushroom@exe/ \
    -Isubprojects/http-parser/http-parser/ \
    -Isubprojects/flatcc/rules \
    -Isubprojects/flatcc/include/ \
    -Isubprojects/flatcc/include/flatcc/reflection/

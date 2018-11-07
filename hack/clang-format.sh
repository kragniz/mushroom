#!/usr/bin/env sh

cd "${MESON_SOURCE_ROOT}"
clang-format -style=file -i src/*.c include/*.h test/*.c

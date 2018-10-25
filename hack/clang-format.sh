#!/usr/bin/env sh

cd "${MESON_SOURCE_ROOT}"
clang-format -style=file -i *.c include/*.h test/*.c

#!/bin/sh

out=cmake-build-debug
cmake -S. -B${out} -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=On
[ -f ${out}/compile_commands.json ] && ln -sf ${out}/compile_commands.json compile_commands.json
ninja -C ${out}

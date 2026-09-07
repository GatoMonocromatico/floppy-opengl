#!/bin/bash
# Build (Debug) then run the game from the project root so shaders/textures
# resolve relative to the working directory. Runs under gdb for a crash backtrace.
set -e
cd "$(dirname "$0")"

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

gdb -batch -ex run -ex "bt full" ./build/tetris_roguelike

#!/bin/bash

cmake -S ./ -B ./cmake-build-debug && cmake --build ./cmake-build-debug
# cmake -S ./ -B ./cmake-build-debug && cmake --build ./cmake-build-debug --target simulator
# cmake -S ./ -B ./cmake-build-debug && cmake --build ./cmake-build-debug --target unit-tests && ./cmake-build-debug/test/unit-tests

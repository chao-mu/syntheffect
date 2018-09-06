#!/bin/bash

cppcheck -Isrc/ --quiet --std=c++14 --language=c++  --enable=all `find src/ -name "*.cpp"` |& sed 's/\[//' | sed 's/\]//' | grep -v '^(information'
clang-tidy -p compile_commands.json -quiet -header-filter "syntheffect" -system-headers=0 `find src/syntheffect/ -name "*.cpp"` |& grep -E "warning:|error:" | grep "src/syntheffect"

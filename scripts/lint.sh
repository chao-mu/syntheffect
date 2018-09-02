#!/bin/bash

cppcheck -Isrc/ --quiet --std=c++14 --language=c++  --enable=all `find src/ -name "*.cpp"` |& sed 's/\[//' | sed 's/\]//' | grep -v '^(information'

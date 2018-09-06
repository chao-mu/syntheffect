#!/bin/bash

#make -j12
bear --append make -j12
jq '.[] | select(.file | startswith("src"))' compile_commands.json  | jq -s '.' > compile_commands.json.2 && mv compile_commands.json.2 compile_commands.json

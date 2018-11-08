#!/usr/bin/env python3

import sys
import os
import re
import glob

import yaml

def main():

    if len(sys.argv) == 1:
        sys.exit("ERROR: Expected arguments to be config destination dir and then series of shader files")

    config_dir = sys.argv[1]

    for path in sys.argv[2:]:
        base = os.path.basename(path)
        name = os.path.splitext(base)[0]

        doc = {}

        doc["frag"] = name
        doc["vert"] = "Passthrough"

        doc["inputs"] = ["syncH", "syncV"]
        doc["outputs"] = []
        with open(path) as f:
            for line in f:
                m = re.match("DEFINE_INPUT\((\w+),", line)
                if m is not None:
                    doc["inputs"].append(m.group(1))
                    continue

                m = re.match("DEFINE_OUTPUT_\d+\((\w+),", line)
                if m is not None:
                    doc["outputs"].append(m.group(1))
                    continue


        with open(os.path.join(config_dir, "{}.yml".format(underscore(name))), "w") as f:
            yaml.dump(doc, f, default_flow_style=False)

# https://stackoverflow.com/questions/1175208/elegant-python-function-to-convert-camelcase-to-snake-case
def underscore(name):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()


if __name__ == "__main__":
    main()
